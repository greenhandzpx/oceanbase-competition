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

#ifndef OCEANBASE_ROOTSERVER_OB_TTL_SCHEDULER_H_
#define OCEANBASE_ROOTSERVER_OB_TTL_SCHEDULER_H_

#include "share/table/ob_ttl_util.h"

namespace oceanbase
{
namespace rootserver
{
class ObRootService;

typedef common::ObArray<common::ObAddr> ServerList;
typedef common::hash::ObHashSet<common::ObAddr> ServerInfoSet;

/**
 * the task for clear ttl history task in __all_ttl_task_status_history
*/
class ObClearTTLStatusHistoryTask : public common::ObTimerTask {
public:
  explicit ObClearTTLStatusHistoryTask(ObRootService& rs);
  virtual ~ObClearTTLStatusHistoryTask() {}
  int init() { return OB_SUCCESS; }
  virtual void runTimerTask() override;
  void destroy() {}

private:
  ObRootService& root_service_;
};

typedef struct RsTenantTask {
  common::ObTTLStatus ttl_status_;
  ServerList send_servers_;
  ServerList eliminate_servers_;
  ServerList rsp_servers_;
  bool all_responsed_;

  RsTenantTask() 
    : ttl_status_(),
      send_servers_(),
      eliminate_servers_(),
      rsp_servers_(),
      all_responsed_(false) {}

TO_STRING_KV(K_(ttl_status),
             K_(send_servers),
             K_(eliminate_servers),
             K_(rsp_servers),
             K_(all_responsed));
} RsTenantTask;

typedef struct ObTTLTenantTask {
  ObArray<RsTenantTask> tasks_;
  bool need_refresh_;
  uint64_t tenant_id_;
  bool is_del_;

  ObTTLTenantTask(uint64_t tenant_id = OB_INVALID_ID)
    : tasks_(),
      need_refresh_(true),
      tenant_id_(tenant_id),
      is_del_(false) {}

  void reset() {
    tasks_.reuse();
    is_del_ = false;
    need_refresh_ = true;
  }

  TO_STRING_KV(K_(tasks),
               K_(need_refresh),
               K_(tenant_id),
               K_(is_del));
} ObTTLTenantTask;

/*
 * the scheduler for all ttl and max version deletion tasks executed in root service
 *
 * every ttl task has its record in an inner table __all_ttl_task_status
 * which will be used to recover or cleanup the task when the root server has switched
 */
class ObTTLScheduler : private common::ObTimerTask
{
public:
  static const int64_t SCHEDULE_PERIOD = 20 * 1000L * 1000L; // 20s 
  explicit ObTTLScheduler(ObRootService& rs) 
    : is_inited_(false),
      root_service_(rs),
      clear_ttl_history_task_(rs) {}

  virtual ~ObTTLScheduler() {}
  int init();
  int start();
  void wait();
  void stop();
  void destroy();

private:
  void runTimerTask() override;
private:
  typedef common::hash::ObHashMap<uint64_t, ObTTLTenantTask, common::hash::NoPthreadDefendMode> TenantTaskMap;
  bool is_inited_;
  ObRootService& root_service_;
  ObClearTTLStatusHistoryTask clear_ttl_history_task_;
};


class ObTTLTenantTaskMgr {
public:
  static ObTTLTenantTaskMgr& get_instance();
  int init();

  int add_ttl_task(uint64_t tenant_id, ObTTLTaskType task_type);
  int refresh_tenant(uint64_t tenant_id);

  int refresh_all();
  
  int alter_status_and_add_ttl_task(uint64_t tenant_id);

  int get_tenant_tasks(uint64_t tenant_id, ObTTLTenantTask& ten_tasks);
  
  int get_task(uint64_t tenant_id, uint64_t task_id, RsTenantTask& ten_task);

  int rsp_task_status(common::ObTTLTaskType rsp_task_type, ObTTLTaskStatus rs_status);

  int process_tenant_task_rsp(uint64_t tenant_id,
                              int64_t task_id,
                              int64_t task_type,
                              const ObAddr& server_addr);

  void reset_local_tenant_task();

  void proc_deleted_tenant();

  virtual int get_tenant_ids(ObIArray<uint64_t>& tenant_ids);

  int process_tenant_tasks(uint64_t tenant_id);

private:

  ObTTLTenantTaskMgr()
    : mutex_(),
      ten_task_arr_(),
      del_ten_arr_(),
      need_refresh_(true),
      is_inited_(false) {}

  int update_task_on_responsed(RsTenantTask& task);

  virtual bool is_enable_ttl(uint64_t tenant_id);

  virtual int delete_task(uint64_t tenant_id, uint64_t task_id);

  virtual int in_active_time(uint64_t tenant_id, bool& is_active_time);

  virtual int read_tenant_status(uint64_t tenant_id, 
                        common::ObTTLStatusArray& tenant_tasks);

  virtual int insert_tenant_task(ObTTLStatus& ttl_task);

  virtual int update_task_status(uint64_t tenant_id,
                                  uint64_t task_id,
                                  int64_t rs_new_status);


  bool tenant_exist(uint64_t tenant_id);
  virtual int update_tenant_tasks(uint64_t tenant_id, common::ObTTLStatusArray& tasks);

  virtual int get_alive_servers(uint64_t tenant_id,
                        ServerList& server_infos);
  /* variables */
  virtual int fetch_ttl_task_id(uint64_t tenant_id, int64_t &new_task_id);
  // RS-> observer ttl request
  virtual int dispatch_ttl_request(ServerList& addrs, 
                                   ServerList& eliminate_addrs, 
                                   uint64_t tenant_id,
                                   int ttl_cmd,
                                   int trigger_type,
                                   int64_t task_id);
  virtual int get_valid_servers(ServerList& all_list, ServerList& remove_list, ServerList& ret_list);

  int add_tenant(uint64_t tenant_id);
  void delete_tenant(uint64_t tenant_id);
  bool need_refresh_tenant(uint64_t tenant_id);
  bool need_task_retry(RsTenantTask& rs_task);
  // need lock
  int get_tenant_tasks_ptr(uint64_t tenant_id,
                           ObTTLTenantTask*& tasks_ptr);
  int get_task_ptr(uint64_t tenant_id,
                    uint64_t task_id, 
                    RsTenantTask*& ten_task);
  int user_cmd_upon_task(ObTTLTaskType task_type,
                         ObTTLTaskStatus curr_state,
                         ObTTLTaskStatus &next_state,
                         bool &add_new_task);

  bool is_all_responsed(RsTenantTask& rs_task);

  ObTTLTaskStatus next_status(int64_t curr);

  int add_ttl_task_internal(uint64_t tenant_id,
                            TRIGGER_TYPE trigger_type,
                            bool sync_server);
  
  ObTTLTaskType eval_task_cmd_type(ObTTLTaskStatus status);

  int send_server_task_req(RsTenantTask& task, ObTTLTaskType task_type);

  void refresh_deleted_tenants();

  bool is_addr_exist(ServerList& addr_arr, const ObAddr& addr);

private:
  lib::ObMutex mutex_; // lib::ObMutexGuard guard(mutex_);
  ObArray<ObTTLTenantTask> ten_task_arr_;
  ObArray<uint64_t> del_ten_arr_;
  bool need_refresh_;
  bool is_inited_;


  const int64_t OB_TTL_TASK_RETRY_INTERVAL = ObTTLScheduler::SCHEDULE_PERIOD * 15; // retry interval 300s
};

#define TTLMGR ObTTLTenantTaskMgr::get_instance()


} // end namespace rootserver
} // end namespace oceanbase

#endif /* OCEANBASE_ROOTSERVER_OB_DDL_SCHEDULER_H_ */