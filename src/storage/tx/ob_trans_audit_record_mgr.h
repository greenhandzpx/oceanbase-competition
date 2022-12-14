/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef OCEANBASE_TRANSACTION_OB_TRANS_AUDIT_RECORD_MGR_
#define OCEANBASE_TRANSACTION_OB_TRANS_AUDIT_RECORD_MGR_

#include "ob_trans_define.h"
#include "lib/queue/ob_fixed_queue.h"
#include "lib/atomic/ob_atomic.h"
#include "common/ob_simple_iterator.h"

namespace oceanbase
{
namespace transaction
{

struct ObTransAuditCommonInfo
{
  int64_t tenant_id_;
  common::ObAddr server_addr_;
  ObTransID trans_id_;
  // common::ObPartitionKey partition_key_;

  void reset()
  {
    tenant_id_ = 0;
    server_addr_.reset();
    trans_id_.reset();
    // partition_key_.reset();
  }
};

struct ObTransAuditInfo
{
  uint64_t session_id_;
  uint64_t proxy_session_id_;
  int32_t trans_type_;
  uint64_t ctx_create_time_;
  uint64_t expired_time_;
  ObStartTransParam trans_param_;
  int64_t trans_ctx_type_;
  int64_t total_sql_no_;
  int32_t ctx_refer_;
  int64_t ctx_addr_;
  int status_;
  bool for_replay_;
  ObElrTransInfoArray prev_trans_arr_;
  ObElrTransInfoArray next_trans_arr_;

  void reset()
  {
    session_id_ = 0;
    proxy_session_id_ = 0;
    trans_type_ = TransType::UNKNOWN;
    ctx_create_time_ = 0;
    expired_time_ = 0;
    trans_param_.reset();
    trans_ctx_type_ = ObTransCtxType::UNKNOWN;
    total_sql_no_ = 0;
    ctx_refer_ = 0;
    ctx_addr_ = 0;
    status_ = common::OB_SUCCESS;
    prev_trans_arr_.reset();
    next_trans_arr_.reset();
  }
};

struct ObTransAuditStmtInfo
{
  int64_t sql_no_;
  sql::ObPhyPlanType phy_plan_type_;             // 执计划的类型
  common::ObTraceIdAdaptor trace_id_;
  int64_t ctx_addr_;                 // 执行该语句的上下文地址
  int64_t proxy_receive_us_;         // proxy收到请求的时刻
  int64_t server_receive_us_;        // server收到请求的时刻
  int64_t trans_receive_us_;         // 事务层start_stmt/start_participant的时间点（us）
  int64_t trans_execute_us_;         // 事务层start_stmt/end_stmt直接的耗时
  int64_t lock_for_read_retry_count_; // lock_for_read的次数

  void reset()
  {
    sql_no_ = 0;
    phy_plan_type_ = sql::ObPhyPlanType::OB_PHY_PLAN_UNINITIALIZED;
    trace_id_.reset();
    ctx_addr_ = 0;
    proxy_receive_us_ = 0;
    server_receive_us_ = 0;
    trans_receive_us_ = 0;
    trans_execute_us_ = 0;
    lock_for_read_retry_count_ = -1;
  }

  TO_STRING_KV(K_(sql_no), K_(phy_plan_type), K_(trace_id), K_(proxy_receive_us), K_(server_receive_us),
               K_(trans_receive_us), K_(trans_execute_us), K_(lock_for_read_retry_count));
};

typedef common::ObSimpleIterator<ObTransAuditStmtInfo,
                                 common::ObModIds::OB_TRANS_VIRTUAL_TABLE_TRANS_STAT, 16> ObTransAuditStmtInfoIterator;

class ObTransAuditRecord
{
public:
  ObTransAuditRecord() : lock_(ObLatchIds::TRANS_AUDIT_RECORD_LOCK), is_valid_(false), ctx_(NULL) {}
  ~ObTransAuditRecord() {}
  int init(ObTransCtx *ctx);
  void reset();
public:
  // 获取事务audit data，如果ctx指针有效直接从ctx中获取，否则从buffer获取
  int get_trans_audit_data(ObTransAuditCommonInfo &common_info, ObTransAuditInfo &trans_info,
                           char *trace_log_buffer, int64_t buf_len);

  // 获取事务sql audit data，直接从buffer中获取
  int get_trans_sql_audit_data(ObTransAuditCommonInfo &common_info,
                               ObTransAuditStmtInfoIterator &stmt_info_iter);

  // 用于 PartTransCtx 拷贝依赖关系列表
  int set_trans_dep_arr(const ObElrTransInfoArray &prev_trans_arr,
                        const ObElrTransInfoArray &next_trans_arr);

  // 事务上下文在结束释放时，通过这个接口将数据填到audit buffer中，并将buffer中自己的指针置空
  //int set_trans_audit_data(int64_t tenant_id, const common::ObAddr &addr, const ObTransID &trans_id,
  //    const common::ObPartitionKey &pkey, uint64_t session_id, uint64_t proxy_session_id,
  //    int32_t trans_type, int32_t ctx_refer, uint64_t ctx_create_time, uint64_t expired_time,
  //    const ObStartTransParam &trans_param, int64_t trans_ctx_type, int status, bool for_replay);

  // 语句执行过程中将数据回填到audit buffer中
  int set_start_stmt_info(int64_t sql_no, sql::ObPhyPlanType phy_plan_type,
      const common::ObTraceIdAdaptor &trace_id, int64_t proxy_receive_us,
      int64_t server_receive_us, int64_t trans_receive_us);
  int set_end_stmt_info(int64_t sql_no, int64_t trans_execute_us, int64_t lock_for_read_retry_count);

  bool is_valid();
  ObTransTraceLog *get_trace_log() { return &trace_log_; }

  static const int64_t STMT_INFO_COUNT = 32;
private:
  ObTransAuditCommonInfo common_info_;

  ObTransAuditInfo trans_info_;
  ObTransTraceLog trace_log_;

  ObTransAuditStmtInfo stmt_info_[STMT_INFO_COUNT];

private:
  common::SpinRWLock lock_;
  bool is_valid_;  // 用于在迭代时判断是否含有有效数据，在事务set_ctx_addr后有效
  ObTransCtx *ctx_;
};

/*
 * 一开始申请一块大的内存空间，将所有空闲空间指针放入free_addrs_管理
 * 事务开始时从free_addrs_申请空闲内存并reset()
 * 事务结束后放入地址free_addrs_但不清除内容
 */
class ObTransAuditRecordMgr
{
public:
  ObTransAuditRecordMgr() : is_inited_(false), record_count_(0), records_(NULL) {}
  ~ObTransAuditRecordMgr() { destroy(); }

public:
  int init(const int32_t mem_size, const uint64_t tenant_id);
  void destroy();

  static int mtl_init(ObTransAuditRecordMgr *&rec_mgr);
  static void mtl_destroy(ObTransAuditRecordMgr *&rec_mgr);

public:
  int get_empty_record(ObTransAuditRecord *&record);
  int get_record(const int32_t index, ObTransAuditRecord *&record);
  int revert_record(ObTransAuditRecord *record);

  int32_t get_record_count() { return record_count_; }

private:
  // 事务记录使用的最大内存
  static const int64_t MAX_TRANS_RECORD_MEMORY_SIZE = 100 * 1024 * 1024; // 100 MB
  static const int64_t MINI_MODE_MAX_TRANS_RECORD_MEMORY_SIZE = 32 * 1024 * 1024; // 32 MB
private:
  bool is_inited_;
  int32_t record_count_;
  common::ObFixedQueue<ObTransAuditRecord> free_addrs_;
  ObTransAuditRecord *records_;
private:
  DISALLOW_COPY_AND_ASSIGN(ObTransAuditRecordMgr);
};

class ObTransAuditRecordIterator
{
public:
  ObTransAuditRecordIterator()
      : last_ret_(common::OB_SUCCESS),
        record_index_(0),
        record_mgr_(nullptr) { }
  ~ObTransAuditRecordIterator() { }
  int init(ObTransAuditRecordMgr *mgr);
  bool is_vaild();
  int get_next(ObTransAuditRecord *&record);
private:
  int last_ret_;
  int32_t record_index_;
  ObTransAuditRecordMgr *record_mgr_;
};

class ObTransAuditDataIterator
{
public:
  ObTransAuditDataIterator() { }
  ~ObTransAuditDataIterator() { }
  int init(ObTransAuditRecordMgr *mgr) { return rec_iter_.init(mgr); }
  bool is_valid() { return rec_iter_.is_vaild(); }
public:
  int get_next(ObTransAuditCommonInfo &common_info,
               ObTransAuditInfo &trans_info,
               char *trace_log_buffer, int64_t buf_len);
private:
  ObTransAuditRecordIterator rec_iter_;
};

class ObTransSQLAuditDataIterator
{
public:
  ObTransSQLAuditDataIterator() { }
  ~ObTransSQLAuditDataIterator() { }
  int init(ObTransAuditRecordMgr *mgr);
  bool is_valid() { return stmt_info_iter_.is_ready() && rec_iter_.is_vaild(); }
public:
  int get_next(ObTransAuditCommonInfo &common_info,
               ObTransAuditStmtInfo &stmt_info);
private:
  ObTransAuditStmtInfoIterator stmt_info_iter_;
  ObTransAuditCommonInfo common_info_;
  ObTransAuditRecordIterator rec_iter_;
};

}  // namespace transaction
}  // namespace oceanbase

#endif  // OCEANBASE_TRANSACTION_OB_TRANS_AUDIT_
