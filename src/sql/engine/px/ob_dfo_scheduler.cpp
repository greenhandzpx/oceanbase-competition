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

#define USING_LOG_PREFIX SQL_ENG
#include "lib/container/ob_se_array.h"
#include "share/ob_rpc_share.h"
#include "share/schema/ob_part_mgr_util.h"
#include "sql/dtl/ob_dtl_channel_group.h"
#include "sql/engine/px/ob_dfo_scheduler.h"
#include "sql/engine/px/ob_px_scheduler.h"
#include "sql/engine/px/ob_px_util.h"
#include "sql/engine/px/ob_px_dtl_msg.h"
#include "sql/engine/px/ob_px_rpc_processor.h"
#include "sql/engine/px/ob_px_sqc_async_proxy.h"
#include "share/ob_server_blacklist.h"

using namespace oceanbase::common;
using namespace oceanbase::share;
using namespace oceanbase::share::schema;
using namespace oceanbase::sql;
using namespace oceanbase::sql::dtl;

ObDfoSchedulerBasic::ObDfoSchedulerBasic(ObPxCoordInfo &coord_info,
                      ObPxRootDfoAction &root_dfo_action,
                      ObIPxCoordEventListener &listener)
  : coord_info_(coord_info),
    root_dfo_action_(root_dfo_action),
    listener_(listener)
{
}

int ObDfoSchedulerBasic::dispatch_root_dfo_channel_info(ObExecContext &ctx, ObDfo &child, ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  ObPxTaskChSets parent_ch_sets;
  int64_t child_dfo_id = child.get_dfo_id();
  if (parent.is_root_dfo()) {
    ObDtlChTotalInfo *ch_info = nullptr;
    if (OB_FAIL(child.get_dfo_ch_info(0, ch_info))) {
      LOG_WARN("failed to get task receive chs", K(ret));
    } else if (!parent.check_root_valid()) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("NULL ptr", K(parent), K(ret));
    } else if (OB_FAIL(root_dfo_action_.receive_channel_root_dfo(ctx, parent, *ch_info))) {
      LOG_WARN("failed to receive channel for root dfo", K(ret));
    }
  }
  return ret;
}

int ObDfoSchedulerBasic::init_all_dfo_channel(ObExecContext &ctx) const
{
  int ret = OB_SUCCESS;
  /*do nothings*/
  UNUSED(ctx);
  return ret;
}

int ObDfoSchedulerBasic::on_sqc_threads_inited(ObExecContext &ctx, ObDfo &dfo) const
{
  int ret = OB_SUCCESS;
  UNUSED(ctx);
  if (OB_FAIL(dfo.prepare_channel_info())) {
    LOG_WARN("failed to prepare channel info", K(ret));
  }
  if (OB_SUCC(ret)) {
    if (dfo.have_px_bloom_filter() && OB_FAIL(dfo.alloc_bloom_filter_ch())) {
      LOG_WARN("fail to init px bloom filter channel", K(ret));
    }
  }
  LOG_TRACE("on_sqc_threads_inited: dfo data xchg ch allocated", K(ret));
  return ret;
}

// ??????m * n?????????shuffle
int ObDfoSchedulerBasic::build_data_mn_xchg_ch(ObExecContext &ctx, ObDfo &child, ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  if (SM_NONE != parent.get_slave_mapping_type()
      || ObPQDistributeMethod::Type::PARTITION == child.get_dist_method()
      || ObPQDistributeMethod::Type::PARTITION_RANDOM == child.get_dist_method()
      || ObPQDistributeMethod::Type::PARTITION_HASH == child.get_dist_method()
      || ObPQDistributeMethod::Type::PARTITION_RANGE == child.get_dist_method()) {
    // ???????????????channel map?????????channel map??????????????????
    // 1. slave-mapping??????????????????slave mapping?????????????????????
    // 2. affinity+pw??????(PARTITION)???pkey???????????????partition????????????
    // 3. PARTITION_RANDOM???pkey???????????????sqc????????????
    uint64_t tenant_id = OB_INVALID_ID;
    if (OB_FAIL(get_tenant_id(ctx, tenant_id))) {
    } else if (OB_FAIL(ObSlaveMapUtil::build_mn_ch_map(
        ctx, child, parent, tenant_id))) {
      LOG_WARN("fail to build slave mapping group", K(ret));
    }
  } else {
    // ????????????????????????channel??????
    int64_t child_dfo_idx = -1;
    ObPxChTotalInfos *transmit_mn_ch_info = &child.get_dfo_ch_total_infos();
    uint64_t tenant_id = -1;
    if (OB_FAIL(ObDfo::check_dfo_pair(parent, child, child_dfo_idx))) {
      LOG_WARN("failed to check dfo pair", K(ret));
    } else if (OB_FAIL(get_tenant_id(ctx, tenant_id))) {
    } else if (OB_FAIL(ObSlaveMapUtil::build_mn_channel(
        transmit_mn_ch_info, child, parent, tenant_id))) {
      LOG_WARN("failed to build mn channel", K(ret));
    }
  }
  return ret;
}

int ObDfoSchedulerBasic::build_data_xchg_ch(ObExecContext &ctx, ObDfo &child, ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  ret = build_data_mn_xchg_ch(ctx, child, parent);
  return ret;
}

int ObDfoSchedulerBasic::dispatch_receive_channel_info_via_sqc(ObExecContext &ctx,
                                                                       ObDfo &child,
                                                                       ObDfo &parent,
                                                                       bool is_parallel_scheduler) const
{
  int ret = OB_SUCCESS;
  UNUSED(ctx);
  ObPxTaskChSets parent_ch_sets;
  int64_t child_dfo_id = child.get_dfo_id();
  if (parent.is_root_dfo()) {
    if (OB_FAIL(dispatch_root_dfo_channel_info(ctx, child, parent))) {
      LOG_WARN("fail dispatch root dfo receive channel info", K(ret), K(parent), K(child));
    }
  } else {
    // ??? receive channels sets ?????? sqc ?????????????????????????????? SQC
    ObArray<ObPxSqcMeta *> sqcs;
    if (OB_FAIL(parent.get_sqcs(sqcs))) {
      LOG_WARN("fail get sqcs", K(parent), K(ret));
    } else {
      ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
        int64_t sqc_id = sqcs.at(idx)->get_sqc_id();
        ObPxReceiveDataChannelMsg &receive_data_channel_msg = sqcs.at(idx)->get_receive_channel_msg();
        if (OB_INVALID_INDEX == sqc_id) {
          ret = OB_ERR_UNEXPECTED;
          LOG_WARN("Unexpected param", K(sqc_id), K(ret));
        } else {
          ObDtlChTotalInfo *ch_info = nullptr;
          if (OB_FAIL(child.get_dfo_ch_info(idx, ch_info))) {
            LOG_WARN("failed to get task receive chs", K(ret));
          } else if (OB_FAIL(receive_data_channel_msg.set_payload(child_dfo_id, *ch_info))) {
            LOG_WARN("fail init msg", K(ret));
          } else if (!receive_data_channel_msg.is_valid()) {
            LOG_WARN("receive data channel msg is not valid", K(ret));
          } else if (!is_parallel_scheduler &&
              OB_FAIL(sqcs.at(idx)->add_serial_recieve_channel(receive_data_channel_msg))) {
            LOG_WARN("fail to add recieve channel", K(ret), K(receive_data_channel_msg));
          } else {
            LOG_TRACE("ObPxCoord::MsgProc::dispatch_receive_channel_info_via_sqc done.",
                      K(idx), K(cnt), K(sqc_id), K(child_dfo_id), K(parent_ch_sets));
          }
        }
      }
    }
  }
  return ret;
}

int ObDfoSchedulerBasic::set_temp_table_ctx_for_sqc(ObExecContext &ctx,
                                                    ObDfo &child) const
{
  int ret = OB_SUCCESS;
  ObArray<ObPxSqcMeta *> sqcs;
  if (OB_FAIL(child.get_sqcs(sqcs))) {
    LOG_WARN("failed to get sqcs from child.", K(ret));
  } else {
    for (int64_t i = 0; OB_SUCC(ret) && i < sqcs.count(); i++) {
      ObPxSqcMeta *sqc = sqcs.at(i);
      if (OB_ISNULL(sqc)) {
        ret = OB_ERR_UNEXPECTED;
        LOG_WARN("unexpect null sqc", K(ret));
      } else if (OB_FAIL(sqc->get_temp_table_ctx().assign(ctx.get_temp_table_ctx()))) {
        LOG_WARN("failed to assign temp table ctx", K(ret));
      }
    }
  }
  return ret;
}

int ObDfoSchedulerBasic::get_tenant_id(ObExecContext &ctx, uint64_t &tenant_id) const
{
  int ret = OB_SUCCESS;
  ObSQLSessionInfo *session = NULL;
  if (OB_ISNULL(session = ctx.get_my_session())) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("session is NULL", K(ret));
  } else {
    tenant_id = session->get_effective_tenant_id();
  }
  return ret;
}

int ObDfoSchedulerBasic::dispatch_transmit_channel_info_via_sqc(ObExecContext &ctx,
                                                                        ObDfo &child,
                                                                        ObDfo &parent) const
{
  UNUSED(ctx);
  UNUSED(parent);
  int ret = OB_SUCCESS;
  ObPxTaskChSets child_ch_sets;
  ObPxPartChMapArray &map = child.get_part_ch_map();
  if (child.is_root_dfo()) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("a child dfo should not be root dfo", K(child), K(ret));
  } else {
    ObArray<ObPxSqcMeta *> sqcs;
    if (OB_FAIL(child.get_sqcs(sqcs))) {
      LOG_WARN("fail get qc-sqc channel for QC", K(ret));
    } else {
      ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
        int64_t sqc_id = sqcs.at(idx)->get_sqc_id();
        ObPxTransmitDataChannelMsg &transmit_data_channel_msg = sqcs.at(idx)->get_transmit_channel_msg();
        ObDtlChTotalInfo *ch_info = nullptr;
        if (OB_FAIL(child.get_dfo_ch_info(sqc_id, ch_info))) {
          LOG_WARN("fail get child tasks", K(ret));
        } else if (OB_FAIL(transmit_data_channel_msg.set_payload(*ch_info, map))) {
          LOG_WARN("fail init msg", K(ret));
        }

        LOG_TRACE("ObPxCoord::MsgProc::dispatch_transmit_channel_info_via_sqc done."
                  "sent transmit_data_channel_msg to child task",
                  K(transmit_data_channel_msg), K(child), K(idx), K(cnt), K(ret));
      }
    }
  }
  return ret;
}

int ObDfoSchedulerBasic::dispatch_bf_channel_info(ObExecContext &ctx,
    ObDfo &child, ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  UNUSED(ctx);
  UNUSED(child);
  ObPhysicalPlanCtx *phy_plan_ctx = NULL;
  if (OB_ISNULL(phy_plan_ctx = GET_PHY_PLAN_CTX(ctx))) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("phy plan ctx NULL", K(ret));
  } else if (parent.is_root_dfo()) {
    if (ctx.get_bf_ctx().filter_ready_) {
      ObPxBloomFilterChInfo &ch_set_info = ctx.get_bf_ctx().ch_set_info_;
      ctx.get_bf_ctx().ch_set_.reset();
      if (OB_FAIL(ObDtlChannelUtil::get_receive_bf_dtl_channel_set(
          0, ch_set_info, ctx.get_bf_ctx().ch_set_))) {
        LOG_WARN("failed to get receive dtl channel set", K(ret));
      } else if (OB_FAIL(ObPxMsgProc::mark_rpc_filter(ctx))) {
        LOG_WARN("fail to send rpc bloom filter", K(ret));
      }
    } else {
      LOG_ERROR("unexpected status: filter ready must be true", K(ctx.get_bf_ctx().filter_ready_));
    }
  } else {
    // send to dfo with receive operator
    ObArray<ObPxSqcMeta *> sqcs;
    if (OB_FAIL(parent.get_sqcs(sqcs))) {
      LOG_WARN("fail get sqcs", K(parent), K(ret));
    } else {
      ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
        ObDtlChannel *ch = sqcs.at(idx)->get_qc_channel();
        int64_t sqc_id = sqcs.at(idx)->get_sqc_id();
        ObPxCreateBloomFilterChannelMsg bloom_filter_channel_msg;
        bloom_filter_channel_msg.sqc_count_ = sqcs.count();
        bloom_filter_channel_msg.sqc_id_ = sqc_id;
        if (OB_ISNULL(ch) || OB_INVALID_INDEX == sqc_id) {
          ret = OB_ERR_UNEXPECTED;
          LOG_WARN("Unexpected param", KP(ch), K(parent), K(sqc_id), K(ret));
        } else if (OB_FAIL(child.get_use_filter_chs(bloom_filter_channel_msg.ch_set_info_))) {
          LOG_WARN("fail get parent channel", K(ret));
        } else if (OB_FAIL(ch->send(bloom_filter_channel_msg,
            phy_plan_ctx->get_timeout_timestamp()))) {
          LOG_WARN("fail push data to channel", K(ret));
        } else if (OB_FAIL(ch->flush(true, false))) {
          LOG_WARN("fail flush dtl data", K(ret));
        } else {
          LOG_TRACE("dispatched bf ch",
                    K(idx), K(cnt), K(*ch), K(sqc_id));
        }
      }
      if (OB_SUCC(ret) && OB_FAIL(ObPxChannelUtil::sqcs_channles_asyn_wait(sqcs))) {
        LOG_WARN("failed to wait for sqcs", K(ret));
      }
    }
  }
  return ret;
}
int ObDfoSchedulerBasic::set_bloom_filter_ch_for_root_dfo(ObExecContext &ctx,
    ObDfo &dfo) const
{
  int ret = OB_SUCCESS;
  UNUSED(ctx);
  dfo.set_px_bloom_filter_mode(JoinFilterMode::CREATE);
  if (OB_FAIL(dfo.alloc_bloom_filter_ch())) {
    LOG_WARN("fail to alloc bloom filter channel", K(ret));
  }
  return ret;
}

int ObDfoSchedulerBasic::build_bloom_filter_ch(ObExecContext &ctx,
    ObDfo &child, ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  ObPxBloomFilterChInfo &use_filter_ch_info = child.get_use_filter_ch_info();
  uint64_t tenant_id = OB_INVALID_ID;
  if (OB_FAIL(get_tenant_id(ctx, tenant_id))) {
  } else if (OB_FAIL(ObSlaveMapUtil::build_bf_mn_channel(
      use_filter_ch_info, child, parent, tenant_id))) {
    LOG_WARN("failed to build mn channel", K(ret));
  }
  return ret;
}

// -------------?????????-----------
int ObSerialDfoScheduler::init_all_dfo_channel(ObExecContext &ctx) const
{
  int ret = OB_SUCCESS;
  ObIArray<ObDfo *> &dfos = coord_info_.dfo_mgr_.get_all_dfos_for_update();
  for (int i = 0; OB_SUCC(ret) && i < dfos.count(); ++i) {
    ObDfo *child = dfos.at(i);
    ObDfo *parent = child->parent();
    if (OB_ISNULL(child) || OB_ISNULL(parent)) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("dfo is null", K(ret));
    } else if (!child->has_child_dfo() && !child->is_thread_inited()) {
      if (child->has_temp_table_scan()) {
        if (OB_FAIL(ObPXServerAddrUtil::alloc_by_temp_child_distribution(ctx,
                                                                         *child))) {
          LOG_WARN("fail alloc addr by temp child distribution", K(child), K(ret));
        } else { /*do nothing.*/ }
      } else if (OB_FAIL(ObPXServerAddrUtil::alloc_by_data_distribution(
          coord_info_.pruning_table_location_,
          ctx, *child))) {
        LOG_WARN("fail to alloc data distribution", K(ret));
      } else { /*do nothing.*/ }
      if (OB_SUCC(ret)) {
        if (OB_FAIL(set_temp_table_ctx_for_sqc(ctx, *child))) {
          LOG_WARN("failed to set temp table ctx", K(ret));
        }
      }
    } else {
      /*do nothing*/
    }
    if (OB_SUCC(ret)) {
      if (parent->has_temp_table_scan() && !parent->is_thread_inited()) {
        if (OB_FAIL(ObPXServerAddrUtil::alloc_by_temp_child_distribution(ctx,
                                                                         *parent))) {
          LOG_WARN("fail alloc addr by data distribution", K(parent), K(ret));
        } else { /*do nohting.*/ }
      } else if (parent->is_root_dfo() && !parent->is_thread_inited() &&
          OB_FAIL(ObPXServerAddrUtil::alloc_by_local_distribution(ctx, *parent))) {
        LOG_WARN("fail to alloc local distribution", K(ret));
      } else if (!parent->is_root_dfo() &&
                 ObPQDistributeMethod::PARTITION_HASH == child->get_dist_method()) {
        if (OB_FAIL(ObPXServerAddrUtil::alloc_by_reference_child_distribution(
            coord_info_.pruning_table_location_,
            ctx,
            *child, *parent))) {
          LOG_WARN("fail alloc addr by data distribution", K(parent), K(child), K(ret));
        }
      } else if (!parent->is_root_dfo() && !parent->is_thread_inited() &&
          OB_FAIL(ObPXServerAddrUtil::alloc_by_data_distribution(
          coord_info_.pruning_table_location_, ctx, *parent))) {
        LOG_WARN("fail to alloc data distribution", K(ret));
      }
      if (OB_SUCC(ret) && !parent->is_scheduled()) {
        if (OB_FAIL(set_temp_table_ctx_for_sqc(ctx, *parent))) {
          LOG_WARN("failed to set temp table ctx", K(ret));
        }
      }
    }
    if (OB_SUCC(ret)) {
      if (OB_FAIL(init_dfo_channel(ctx, child, parent))) {
        LOG_WARN("fail to init dfo channel", K(ret));
      }
    }
  }

  return ret;
}

int ObSerialDfoScheduler::init_data_xchg_ch(ObExecContext &ctx, ObDfo *dfo) const
{
  int ret = OB_SUCCESS;
  UNUSED(ctx);
  ObArray<ObPxSqcMeta *> sqcs;
  if (OB_ISNULL(dfo)) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("dfo is null", K(ret));
  } else if (OB_FAIL(dfo->get_sqcs(sqcs))) {
    LOG_WARN("fail get qc-sqc channel for QC", K(ret));
  } else {
    ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
      ObPxSqcMeta *sqc = sqcs.at(idx);
      if (OB_ISNULL(sqc)) {
        ret = OB_ERR_UNEXPECTED;
        LOG_WARN("NULL unexpected sqc", K(ret));
      } else {
        sqc->set_task_count(1);
        sqc->set_thread_inited(true);
      }
    }
    if (OB_SUCC(ret)) {
      dfo->set_thread_inited(true);
      if (OB_FAIL(on_sqc_threads_inited(ctx, *dfo))) {
        LOG_WARN("failed to sqc thread init", K(ret));
      }
    }
  }
  return ret;
}
int ObSerialDfoScheduler::init_dfo_channel(ObExecContext &ctx, ObDfo *child, ObDfo *parent) const
{
  int ret = OB_SUCCESS;
  if (!child->is_thread_inited() && OB_FAIL(init_data_xchg_ch(ctx, child))) {
    LOG_WARN("fail to build data xchg ch", K(ret));
  } else if (!parent->is_thread_inited() && OB_FAIL(init_data_xchg_ch(ctx, parent))) {
    LOG_WARN("fail to build parent xchg ch", K(ret));
  } else if (OB_FAIL(build_data_xchg_ch(ctx, *child, *parent))) {
    LOG_WARN("fail to build data xchg ch", K(ret));
  } else if (OB_FAIL(dispatch_dtl_data_channel_info(ctx, *child, *parent))) {
    LOG_WARN("fail to dispatch dtl data channel", K(ret));
  }
  return ret;
}

int ObSerialDfoScheduler::dispatch_dtl_data_channel_info(ObExecContext &ctx, ObDfo &child, ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  if (OB_FAIL(dispatch_receive_channel_info_via_sqc(ctx, child,
      parent, /*is_parallel_scheduler*/false))) {
    LOG_WARN("fail to dispatch recieve channel", K(ret));
  } else if (OB_FAIL(dispatch_transmit_channel_info_via_sqc(ctx, child, parent))) {
    LOG_WARN("fail to dispatch transmit channel", K(ret));
  }
  return ret;
}

int ObSerialDfoScheduler::try_schedule_next_dfo(ObExecContext &ctx) const
{
  int ret = OB_SUCCESS;
  FLTSpanGuard(px_schedule);
  ObDfo *dfo = NULL;
  if (OB_FAIL(coord_info_.dfo_mgr_.get_ready_dfo(dfo))) {
    if (OB_ITER_END != ret) {
      LOG_WARN("fail get ready dfos", K(ret));
    } else {
      LOG_TRACE("No more dfos to schedule", K(ret));
    }
  } else if (OB_ISNULL(dfo)) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("dfo is null, unexpected schedule", K(ret));
  } else if (OB_FAIL(do_schedule_dfo(ctx, *dfo))) {
    LOG_WARN("fail to do schedule dfo", K(ret));
  }
  return ret;
}

int ObSerialDfoScheduler::dispatch_sqcs(ObExecContext &exec_ctx,
                                        ObDfo &dfo,
                                        ObArray<ObPxSqcMeta *> &sqcs) const
{
  int ret = OB_SUCCESS;
  const ObPhysicalPlan *phy_plan = NULL;
  ObPhysicalPlanCtx *phy_plan_ctx = NULL;
  ObSQLSessionInfo *session = NULL;
  ObCurTraceId::TraceId *cur_thread_id = NULL;
  if (OB_SUCC(ret)) {
    if (OB_UNLIKELY(NULL == (phy_plan = dfo.get_phy_plan()))) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("NULL plan ptr unexpected", K(ret));
    } else if (OB_ISNULL(phy_plan_ctx = GET_PHY_PLAN_CTX(exec_ctx))) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("phy plan ctx NULL", K(ret));
    } else if (OB_ISNULL(session = exec_ctx.get_my_session())) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("session is NULL", K(ret));
    } else if (OB_ISNULL(cur_thread_id = ObCurTraceId::get_trace_id())) {
      ret = OB_ALLOCATE_MEMORY_FAILED;
      LOG_WARN("no memory", K(ret));
    }
  }
  bool ignore_vtable_error = true;
  if (OB_SUCC(ret)) {
    ObDfo *child_dfo = nullptr;
    for (int i = 0; i < dfo.get_child_count() && OB_SUCC(ret); ++i) {
      if (OB_FAIL(dfo.get_child_dfo(i, child_dfo))) {
        LOG_WARN("fail to get child dfo", K(ret));
      } else if (!child_dfo->is_ignore_vtable_error()) {
        ignore_vtable_error = false;
        break;
      }
    }
  }
  int64_t cluster_id = GCONF.cluster_id;
  ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
    ObPxSqcMeta &sqc = *sqcs.at(idx);
    const ObAddr &addr = sqc.get_exec_addr();
    auto proxy = coord_info_.rpc_proxy_.to(addr);
    if (OB_UNLIKELY(share::ObServerBlacklist::get_instance().is_in_blacklist(
                        share::ObCascadMember(addr, cluster_id), true /* add_server */,
                        session->get_process_query_time()))) {
      if (!ignore_vtable_error) {
        ret = OB_RPC_CONNECT_ERROR;
        LOG_WARN("peer no in communication, maybe crashed", K(ret), K(sqc), K(cluster_id),
                K(session->get_process_query_time()));
      } else {
        LOG_WARN("ignore the black server list with virtual table", K(addr), K(ret));
      }
    }
    if (OB_FAIL(ret)) {
    } else {
      SMART_VAR(ObPxRpcInitSqcArgs, args) {
        int64_t timeout_us = phy_plan_ctx->get_timeout_timestamp() - ObTimeUtility::current_time();
        ObFastInitSqcCB sqc_cb(addr,
                              *cur_thread_id,
                              &session->get_retry_info_for_update(),
                              phy_plan_ctx->get_timeout_timestamp(),
                              coord_info_.interrupt_id_,
                              &sqc);
        args.set_serialize_param(exec_ctx, const_cast<ObOpSpec &>(*dfo.get_root_op_spec()), *phy_plan);
        if ((NULL != dfo.parent() && !dfo.parent()->is_root_dfo()) ||
          coord_info_.enable_px_batch_rescan()) {
          sqc.set_transmit_use_interm_result(true);
        }
        if (dfo.has_child_dfo()) {
          sqc.set_recieve_use_interm_result(true);
        }
        if (ignore_vtable_error && dfo.get_child_count() > 0) {
          sqc.set_ignore_vtable_error(true);
        }
        if (coord_info_.enable_px_batch_rescan()) {
          OZ(sqc.set_rescan_batch_params(coord_info_.batch_rescan_ctl_->params_));
        }
        if (timeout_us <= 0) {
          ret = OB_TIMEOUT;
          LOG_WARN("dispatch sqc timeout", K(ret));
        } else if (OB_FAIL(args.sqc_.assign(sqc))) {
          LOG_WARN("fail assign sqc", K(ret));
        } else if (FALSE_IT(sqc.set_need_report(true))) {
          // ????????? rpc ??????????????? true
          // ????????? https://work.aone.alibaba-inc.com/issue/26536120
        } else if (OB_FAIL(E(EventTable::EN_PX_SQC_INIT_FAILED) OB_SUCCESS)) {
          sqc.set_need_report(false);
          LOG_WARN("[SIM] server down. fail to init sqc", K(ret));
        } else if (OB_FAIL(proxy
                          .by(THIS_WORKER.get_rpc_tenant()?: session->get_effective_tenant_id())
                          .timeout(timeout_us)
                          .fast_init_sqc(args, &sqc_cb))) {
          LOG_WARN("fail to init sqc", K(ret), K(sqc));
          sqc.set_need_report(false);
          sqc.set_server_not_alive();
        }
      }
    }
  }
  return ret;
}

int ObSerialDfoScheduler::do_schedule_dfo(ObExecContext &ctx, ObDfo &dfo) const
{
  int ret = OB_SUCCESS;
  ObArray<ObPxSqcMeta *> sqcs;
  if (OB_FAIL(dfo.get_sqcs(sqcs))) {
    LOG_WARN("fail get qc-sqc channel for QC", K(ret));
  } else {
    ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
      ObPxSqcMeta *sqc = sqcs.at(idx);
      if (OB_ISNULL(sqc)) {
        ret = OB_ERR_UNEXPECTED;
        LOG_WARN("NULL unexpected sqc", K(ret));
      }
    }
  }
  LOG_TRACE("Dfo's sqcs count", K(dfo), "sqc_count", sqcs.count());

  ObSQLSessionInfo *session = NULL;
  if (OB_SUCC(ret)) {
    if (OB_UNLIKELY(NULL == (session = ctx.get_my_session()))) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("NULL ptr session", K(ret));
    }
  }

  // 0. ?????? QC-SQC ????????????
  ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
    ObPxSqcMeta &sqc = *sqcs.at(idx);
    ObDtlChannelInfo &qc_ci = sqc.get_qc_channel_info();
    ObDtlChannelInfo &sqc_ci = sqc.get_sqc_channel_info();
    const ObAddr &sqc_exec_addr = sqc.get_exec_addr();
    const ObAddr &qc_exec_addr = sqc.get_qc_addr();
    if (OB_FAIL(ObDtlChannelGroup::make_channel(session->get_effective_tenant_id(),
                                                sqc_exec_addr, /* producer exec addr */
                                                qc_exec_addr, /* consumer exec addr */
                                                sqc_ci /* producer */,
                                                qc_ci /* consumer */))) {
      LOG_WARN("fail make channel for QC-SQC", K(ret));
    } else {
      LOG_TRACE("Make a new channel for qc & sqc",
                K(idx), K(cnt), K(sqc_ci), K(qc_ci), K(sqc_exec_addr), K(qc_exec_addr));
    }
  }

  int64_t thread_id = GETTID();
  // 1. ?????? QC-SQC ??????
  ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
    ObPxSqcMeta &sqc = *sqcs.at(idx);
    ObDtlChannelInfo &ci = sqc.get_qc_channel_info();
    ObDtlChannel *ch = NULL;
    // ObDtlChannelGroup::make_channel ????????????????????? ci ?????????
    // ?????? link_channel ????????????????????????????????? channel
    if (OB_FAIL(ObDtlChannelGroup::link_channel(ci, ch))) {
      LOG_WARN("fail link channel", K(ci), K(ret));
    } else if (OB_ISNULL(ch)) {
      LOG_WARN("fail add qc channel", K(ret));
    } else {
      ch->set_qc_owner();
      ch->set_thread_id(thread_id);
      (void)coord_info_.msg_loop_.register_channel(*ch);
      sqc.set_qc_channel(ch);
      LOG_TRACE("link qc-sqc channel and registered to qc msg loop. ready to receive sqc ctrl msg",
                K(idx), K(cnt), K(*ch), K(dfo), K(sqc));
    }
  }


  if (OB_SUCC(ret)) {
    if (OB_FAIL(dispatch_sqcs(ctx, dfo, sqcs))) {
      LOG_WARN("fail to dispatch sqc", K(ret));
    }
  }

  dfo.set_scheduled();
  return ret;
}


// -------------?????????-----------

// ?????? DFO ??? SQC ??????
int ObParallelDfoScheduler::do_schedule_dfo(ObExecContext &exec_ctx, ObDfo &dfo) const
{
  int ret = OB_SUCCESS;

  ObArray<ObPxSqcMeta *> sqcs;
  if (OB_FAIL(dfo.get_sqcs(sqcs))) {
    LOG_WARN("fail get qc-sqc channel for QC", K(ret));
  } else {
    ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
      ObPxSqcMeta *sqc = sqcs.at(idx);
      if (OB_ISNULL(sqc)) {
        ret = OB_ERR_UNEXPECTED;
        LOG_WARN("NULL unexpected sqc", K(ret));
      }
    }
  }

  LOG_TRACE("Dfo's sqcs count", K(dfo), "sqc_count", sqcs.count());

  ObSQLSessionInfo *session = NULL;
  if (OB_SUCC(ret)) {
    if (OB_UNLIKELY(NULL == (session = exec_ctx.get_my_session()))) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("NULL ptr session", K(ret));
    }
  }
  // 0. ?????? QC-SQC ????????????
  ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
    ObPxSqcMeta &sqc = *sqcs.at(idx);
    ObDtlChannelInfo &qc_ci = sqc.get_qc_channel_info();
    ObDtlChannelInfo &sqc_ci = sqc.get_sqc_channel_info();
    const ObAddr &sqc_exec_addr = sqc.get_exec_addr();
    const ObAddr &qc_exec_addr = sqc.get_qc_addr();
    if (OB_FAIL(ObDtlChannelGroup::make_channel(session->get_effective_tenant_id(),
                                                sqc_exec_addr, /* producer exec addr */
                                                qc_exec_addr, /* consumer exec addr */
                                                sqc_ci /* producer */,
                                                qc_ci /* consumer */))) {
      LOG_WARN("fail make channel for QC-SQC", K(ret));
    } else {
      LOG_TRACE("Make a new channel for qc & sqc",
                K(idx), K(cnt), K(sqc_ci), K(qc_ci), K(sqc_exec_addr), K(qc_exec_addr));
    }
  }

  int64_t thread_id = GETTID();
  // 1. ?????? QC-SQC ??????
  ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
    ObPxSqcMeta &sqc = *sqcs.at(idx);
    ObDtlChannelInfo &ci = sqc.get_qc_channel_info();
    ObDtlChannel *ch = NULL;
    // ObDtlChannelGroup::make_channel ????????????????????? ci ?????????
    // ?????? link_channel ????????????????????????????????? channel
    if (OB_FAIL(ObDtlChannelGroup::link_channel(ci, ch))) {
      LOG_WARN("fail link channel", K(ci), K(ret));
    } else if (OB_ISNULL(ch)) {
      LOG_WARN("fail add qc channel", K(ret));
    } else {
      ch->set_qc_owner();
      ch->set_thread_id(thread_id);
      (void)coord_info_.msg_loop_.register_channel(*ch);
      sqc.set_qc_channel(ch);
      LOG_TRACE("link qc-sqc channel and registered to qc msg loop. ready to receive sqc ctrl msg",
                K(idx), K(cnt), K(*ch), K(dfo), K(sqc));
    }
  }

  if (OB_SUCC(ret)) {
    //if (dfo.is_prealloc_transmit_channel() || dfo.is_prealloc_receive_channel()) {
      // ??????????????????????????? DFO ???????????????
      //  - ????????? ???????????????????????????????????????1????????????????????? DFO
      //           ???????????????????????? task ?????? worker ?????????
      //           ????????? SQC ??????????????????????????????????????????
      //ret = fast_dispatch_sqc(exec_ctx, dfo, sqcs);
    //} else {
      // ????????????????????????????????????????????????
      //  - ????????? ??????????????????
      //  - ????????? ????????????????????????????????? sqc????????????????????????????????? dfo ??????
      //  - ????????? init sqc ??????????????????????????? sqc ????????????????????? task ?????????
      //           ??????????????????QC ??????????????????????????????????????????????????????????????????
      //           ???????????????
      //            1. sqc1 ?????????sqc2 ??????
      //            2. dfo abort, clean sqc state
      //            3. sqc1 ????????????????????? task (old news)
      //            4. sqc1, sqc2 ??????????????????
      //            5. sqc1 ????????????
      //            6. sqc2 ????????????????????? task (latest news)
      //            7. qc ?????? dfo ???????????????????????? (???????????????)
      //            8. sqc1 ?????????????????? task (too late msg)
      //
      ret = dispatch_sqc(exec_ctx, dfo, sqcs);
    //}
  }
  return ret;
}

int ObParallelDfoScheduler::dispatch_dtl_data_channel_info(ObExecContext &ctx, ObDfo &child, ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  /* ?????????????????????????????? receive channel???????????? transmit channel???
   * ?????????????????????????????? transmit ?????????????????? receive ???????????????????????????
   * ?????????????????? transmit ?????????????????????????????????????????????DTL ?????????????????????????????????
   */

  if (OB_SUCC(ret)) {
    if (parent.is_prealloc_receive_channel() && !parent.is_scheduled()) {
      // ?????? parent ????????????????????? receive ???????????????????????????????????????
      // ???????????? sqc ?????????????????? receive ?????? channel ??????????????? dtl
      if (OB_FAIL(dispatch_receive_channel_info_via_sqc(ctx, child, parent))) {
        LOG_WARN("fail dispatch receive channel info", K(child), K(parent), K(ret));
      }
    } else {
      if (OB_FAIL(dispatch_receive_channel_info(ctx, child, parent))) {
        LOG_WARN("fail dispatch receive channel info", K(child), K(parent), K(ret));
      }
    }
  }

  if (OB_SUCC(ret)) {
    if (child.is_prealloc_transmit_channel() && !child.is_scheduled()) {
      if (OB_FAIL(dispatch_transmit_channel_info_via_sqc(ctx, child, parent))) {
        LOG_WARN("fail dispatch transmit channel info", K(child), K(ret));
      }
    } else {
      if (OB_FAIL(dispatch_transmit_channel_info(ctx, child, parent))) {
        LOG_WARN("fail dispatch transmit channel info", K(child), K(ret));
      }
    }
  }

  return ret;
}


int ObParallelDfoScheduler::dispatch_transmit_channel_info(ObExecContext &ctx, ObDfo &child, ObDfo &parent) const
{
  UNUSED(ctx);
  UNUSED(parent);
  int ret = OB_SUCCESS;
  ObPxTaskChSets child_ch_sets;
  ObPxPartChMapArray &map = child.get_part_ch_map();
  ObPhysicalPlanCtx *phy_plan_ctx = NULL;
  // TODO: abort here to test transmit wait for channel info when inner_open.
  if (OB_ISNULL(phy_plan_ctx = GET_PHY_PLAN_CTX(ctx))) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("phy plan ctx NULL", K(ret));
  } else if (child.is_root_dfo()) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("a child dfo should not be root dfo", K(child), K(ret));
  } else {
    ObArray<ObPxSqcMeta *> sqcs;
    if (OB_FAIL(child.get_sqcs(sqcs))) {
      LOG_WARN("fail get qc-sqc channel for QC", K(ret));
    } else {
      ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
        ObDtlChannel *ch = sqcs.at(idx)->get_qc_channel();
        int64_t sqc_id = sqcs.at(idx)->get_sqc_id();
        ObPxTransmitDataChannelMsg transmit_data_channel_msg;
        if (OB_ISNULL(ch)) {
          ret = OB_ERR_UNEXPECTED;
          LOG_WARN("qc channel should not be null", K(ret));
        } else {
          ObDtlChTotalInfo *ch_info = nullptr;
          if (OB_FAIL(child.get_dfo_ch_info(idx, ch_info))) {
            LOG_WARN("fail get child tasks", K(ret));
          } else if (OB_FAIL(transmit_data_channel_msg.set_payload(*ch_info, map))) {
            LOG_WARN("fail init msg", K(ret));
          }
        }
        if (OB_FAIL(ret)) {
        } else if (OB_FAIL(ch->send(transmit_data_channel_msg,
              phy_plan_ctx->get_timeout_timestamp()))) { // ????????????????????? push ??????????????????????????????
          LOG_WARN("fail push data to channel", K(ret));
        } else if (OB_FAIL(ch->flush(true, false))) {
          LOG_WARN("fail flush dtl data", K(ret));
        }
        LOG_TRACE("ObPxCoord::MsgProc::dispatch_transmit_channel_info done."
                  "sent transmit_data_channel_msg to child task",
                  K(transmit_data_channel_msg), K(child), K(idx), K(cnt), K(ret));
      }
      if (OB_SUCC(ret) && OB_FAIL(ObPxChannelUtil::sqcs_channles_asyn_wait(sqcs))) {
        LOG_WARN("failed to wait for sqcs", K(ret));
      }
    }
  }
  return ret;
}

int ObParallelDfoScheduler::dispatch_receive_channel_info(ObExecContext &ctx,
                                                                  ObDfo &child,
                                                                  ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  UNUSED(ctx);
  ObPxTaskChSets parent_ch_sets;
  int64_t child_dfo_id = child.get_dfo_id();
  ObPhysicalPlanCtx *phy_plan_ctx = NULL;
  if (OB_ISNULL(phy_plan_ctx = GET_PHY_PLAN_CTX(ctx))) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("phy plan ctx NULL", K(ret));
  } else if (parent.is_root_dfo()) {
    if (OB_FAIL(dispatch_root_dfo_channel_info(ctx, child, parent))) {
      LOG_WARN("fail dispatch root dfo receive channel info", K(ret), K(parent), K(child));
    }
  } else {
    // ??? receive channels sets ?????? sqc ?????????????????????????????? SQC
    ObArray<ObPxSqcMeta *> sqcs;
    if (OB_FAIL(parent.get_sqcs(sqcs))) {
      LOG_WARN("fail get sqcs", K(parent), K(ret));
    } else {
      ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
        ObDtlChannel *ch = sqcs.at(idx)->get_qc_channel();
        int64_t sqc_id = sqcs.at(idx)->get_sqc_id();
        ObPxReceiveDataChannelMsg receive_data_channel_msg;
        if (OB_ISNULL(ch) || OB_INVALID_INDEX == sqc_id) {
          ret = OB_ERR_UNEXPECTED;
          LOG_WARN("Unexpected param", KP(ch), K(parent), K(sqc_id), K(ret));
        } else {
          ObDtlChTotalInfo *ch_info = nullptr;
          if (OB_FAIL(child.get_dfo_ch_info(idx, ch_info))) {
            LOG_WARN("failed to get task receive chs", K(ret));
          } else if (OB_FAIL(receive_data_channel_msg.set_payload(child_dfo_id, *ch_info))) {
            LOG_WARN("fail init msg", K(ret));
          } else if (!receive_data_channel_msg.is_valid()) {
            LOG_WARN("receive data channel msg is not valid", K(ret), K(receive_data_channel_msg));
          }
        }
        if (OB_SUCC(ret)) {
          if (OB_FAIL(ch->send(receive_data_channel_msg,
              phy_plan_ctx->get_timeout_timestamp()))) { // ????????????????????? push ??????????????????????????????
            LOG_WARN("fail push data to channel", K(ret));
          } else if (OB_FAIL(ch->flush(true, false))) {
            LOG_WARN("fail flush dtl data", K(ret));
          } else {
            LOG_TRACE("dispatched receive ch",
                      K(idx), K(cnt), K(*ch), K(sqc_id), K(child_dfo_id), K(parent_ch_sets));
          }
        }
      }
      if (OB_SUCC(ret) && OB_FAIL(ObPxChannelUtil::sqcs_channles_asyn_wait(sqcs))) {
        LOG_WARN("failed to wait for sqcs", K(ret));
      }
    }
  }
  return ret;
}

// ????????????can prealloc ??????????????????????????????
int ObParallelDfoScheduler::check_if_can_prealloc_xchg_ch(ObDfo &child,
                                                                  ObDfo &parent,
                                                                  bool &bret) const
{
  int ret = OB_SUCCESS;
  bret = true;
  ObSEArray<const ObPxSqcMeta *, 16> sqcs;

  if (child.is_scheduled() || parent.is_scheduled()) {
    bret = false;
  } else if (parent.is_px_create_bloom_filter() && child.is_px_use_bloom_filter()) {
    bret = false;
  } else if (OB_FAIL(child.get_sqcs(sqcs))) {
    LOG_WARN("fail to get child sqcs", K(ret));
  } else {
    ARRAY_FOREACH_X(sqcs, idx, cnt, true == bret) {
      const ObPxSqcMeta &sqc = *sqcs.at(idx);
      if (1 < sqc.get_max_task_count() ||
          1 < sqc.get_min_task_count()) {
        bret = false;
      }
    }
  }
  if (bret && OB_SUCC(ret)) {
    sqcs.reuse();
    if (OB_FAIL(parent.get_sqcs(sqcs))) {
      LOG_WARN("fail to get parent sqcs", K(ret));
    } else {
      ARRAY_FOREACH_X(sqcs, idx, cnt, true == bret) {
        const ObPxSqcMeta &sqc = *sqcs.at(idx);
        if (1 < sqc.get_max_task_count() ||
            1 < sqc.get_min_task_count()) {
          bret = false;
        }
      }
    }
  }
  return ret;
}

int ObParallelDfoScheduler::do_fast_schedule(ObExecContext &exec_ctx,
                                                     ObDfo &child,
                                                     ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  // ????????????????????????????????????????????????????????????????????????????????????
  // ??????????????????????????????????????????????????????
  //  1. ?????? root dfo ??????root dfo ?????????????????????????????????
  //  2. ???????????? child dfo ??????????????????
  //  ????????????????????????????????? child ??? parent ????????????????????????
  //  ????????? parent-child ??? channel ???????????????
  //  3. ???????????? child ?????????????????? channel ??????????????? sqc ???
  if (OB_SUCC(ret) && !parent.is_scheduled()) {
    parent.set_prealloc_receive_channel(true);
    if (parent.has_parent() && parent.parent()->is_thread_inited()) {
      parent.set_prealloc_transmit_channel(true);
    }
    if (OB_FAIL(mock_on_sqc_init_msg(exec_ctx, parent))) {
      LOG_WARN("fail mock init parent dfo", K(parent), K(child), K(ret));
    }
  }
  if (OB_SUCC(ret) && !child.is_scheduled()) {
    child.set_prealloc_transmit_channel(true);
    if (OB_FAIL(mock_on_sqc_init_msg(exec_ctx, child))) {
      LOG_WARN("fail mock init child dfo", K(parent), K(child), K(ret));
    }
  }
  if (OB_SUCC(ret) && !parent.is_scheduled()) {
    if (OB_FAIL(schedule_dfo(exec_ctx, parent))) {
      LOG_WARN("fail schedule root dfo", K(parent), K(ret));
    }
  }
  if (OB_SUCC(ret) && !child.is_scheduled()) {
    if (OB_FAIL(schedule_dfo(exec_ctx, child))) {
      LOG_WARN("fail schedule child dfo", K(child), K(ret));
    }
  }
  if (OB_SUCC(ret)) {
    LOG_TRACE("fast schedule ok", K(parent), K(child));
  }
  return ret;
}

int ObParallelDfoScheduler::mock_on_sqc_init_msg(ObExecContext &ctx, ObDfo &dfo) const
{
  int ret = OB_SUCCESS;
  ObArray<ObPxSqcMeta *> sqcs;
  if (dfo.is_root_dfo()) {
    // root dfo ?????? mock ????????????
  } else if (OB_FAIL(dfo.get_sqcs(sqcs))) {
    LOG_WARN("fail get qc-sqc channel for QC", K(ret));
  } else {
    ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
      ObPxSqcMeta *sqc = sqcs.at(idx);
      if (OB_ISNULL(sqc)) {
        ret = OB_ERR_UNEXPECTED;
        LOG_WARN("NULL unexpected sqc", K(ret));
      } else if (1 != sqc->get_max_task_count()) {
        ret = OB_ERR_UNEXPECTED;
        LOG_WARN("only if all sqc task cnt is one", K(*sqc), K(ret));
      } else {
        ObPxInitSqcResultMsg pkt;
        pkt.dfo_id_ = sqc->get_dfo_id();
        pkt.sqc_id_ = sqc->get_sqc_id();
        pkt.rc_ = OB_SUCCESS;
        pkt.task_count_ = sqc->get_max_task_count();
        if (OB_FAIL(proc_.on_sqc_init_msg(ctx, pkt))) {
          LOG_WARN("fail mock sqc init msg", K(pkt), K(*sqc), K(ret));
        }
      }
    }
  }
  return ret;
}


int ObParallelDfoScheduler::schedule_dfo(ObExecContext &exec_ctx,
    ObDfo &dfo) const
{
  int ret = OB_SUCCESS;
  int retry_times = 0;
  ObPhysicalPlanCtx *phy_plan_ctx = NULL;
  /* ???????????????
   * 1. Timeout Msg: ??????????????????????????????????????????????????????????????????????????????
   * 2. No Msg: DFO ?????? DTL ???????????????????????????
   * 3. DFO Msg: ?????????????????? dispatch ??????
   *
   * ?????????????????????
   * 1. QC ??? DTL???????????????
   */
  NG_TRACE_EXT(dfo_start, OB_ID(dfo_id), dfo.get_dfo_id());
  if (dfo.is_root_dfo()) {
    if (OB_FAIL(on_root_dfo_scheduled(exec_ctx, dfo))) {
      LOG_WARN("fail setup root dfo", K(ret));
    }
  } else if (OB_ISNULL(phy_plan_ctx = GET_PHY_PLAN_CTX(exec_ctx))) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("phy plan ctx NULL", K(ret));
  } else if (OB_FAIL(do_schedule_dfo(exec_ctx, dfo))) {
    LOG_WARN("fail dispatch dfo", K(ret));
  }
  // ?????????????????????????????????????????????
  // ??? schedule ?????????????????? query ???????????????
  dfo.set_scheduled();
  LOG_TRACE("schedule dfo ok", K(dfo), K(retry_times), K(ret));
  return ret;
}


int ObParallelDfoScheduler::on_root_dfo_scheduled(ObExecContext &ctx, ObDfo &root_dfo) const
{
  int ret = OB_SUCCESS;
  ObPxSqcMeta *sqc = NULL;

  LOG_TRACE("on_root_dfo_scheduled", K(root_dfo));

  if (OB_FAIL(root_dfo.get_sqc(0, sqc))) {
    LOG_WARN("fail find sqc", K(root_dfo), K(ret));
  } else if (OB_ISNULL(sqc)) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("NULL ptr", K(root_dfo), KP(sqc), K(ret));
  } else {
    sqc->set_task_count(1);
    sqc->set_thread_inited(true);
    root_dfo.set_thread_inited(true);
    root_dfo.set_used_worker_count(0);
    ret = on_sqc_threads_inited(ctx, root_dfo);
  }

  if (OB_SUCC(ret)) {
    if (root_dfo.is_thread_inited()) {
      // ???????????? self-child ???
      if (OB_SUCC(ret)) {
        int64_t cnt = root_dfo.get_child_count();
        ObDfo *child= NULL;
        if (1 != cnt) {
          ret = OB_ERR_UNEXPECTED;
          LOG_WARN("root dfo should has only 1 child dfo", K(cnt), K(ret));
        } else if (OB_FAIL(root_dfo.get_child_dfo(0, child))) {
          LOG_WARN("fail get child dfo", K(cnt), K(ret));
        } else if (OB_ISNULL(child)) {
          ret = OB_ERR_UNEXPECTED;
          LOG_WARN("NULL unexpected", K(ret));
        } else if (child->is_thread_inited()) {
          // ?????? root-child ???????????? schedule ???????????????????????????
          // ??????????????? schedule ????????? dfo ?????????????????? on_dfo_pair_thread_inited ??????
          // ?????????root-A-B ???????????????A???B ??????????????????????????????????????? thread init msg
          // ??????????????? root???????????? root ????????? on_dfo_pair_thread_inited
          ret = proc_.on_dfo_pair_thread_inited(ctx, *child, root_dfo);
        }
      }
    }
  }
  return ret;
}

// ???????????? DFO ????????? server????????? SQC
int ObParallelDfoScheduler::dispatch_sqc(ObExecContext &exec_ctx,
                                         ObDfo &dfo,
                                         ObArray<ObPxSqcMeta *> &sqcs) const
{
  int ret = OB_SUCCESS;
  bool fast_sqc = dfo.is_fast_dfo();

  const ObPhysicalPlan *phy_plan = NULL;
  ObPhysicalPlanCtx *phy_plan_ctx = NULL;
  ObSQLSessionInfo *session = NULL;

  if (OB_SUCC(ret)) {
    if (OB_UNLIKELY(NULL == (phy_plan = dfo.get_phy_plan()))) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("NULL plan ptr unexpected", K(ret));
    } else if (OB_ISNULL(phy_plan_ctx = GET_PHY_PLAN_CTX(exec_ctx))) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("phy plan ctx NULL", K(ret));
    } else if (OB_ISNULL(session = exec_ctx.get_my_session())) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("session is NULL", K(ret));
    }
  }
  ObPxSqcAsyncProxy proxy(coord_info_.rpc_proxy_, dfo, exec_ctx, phy_plan_ctx, session, phy_plan, sqcs);
  if (OB_FAIL(ret)) {
  } else if (OB_FAIL(proxy.launch_all_rpc_request())) {
    LOG_WARN("fail to send all init async sqc", K(exec_ctx), K(ret));
  } else if (OB_FAIL(proxy.wait_all())) {
    // ret ???????????? is_data_not_readable_err???????????????????????????`deal_with_init_sqc_error`????????????
    if (is_data_not_readable_err(ret) || is_server_down_error(ret)) {
      ObPxSqcMeta &sqc = *sqcs.at(proxy.get_error_index());
      LOG_WARN("fail to wait all async init sqc", K(ret), K(sqc), K(exec_ctx));
      int temp_ret = deal_with_init_sqc_error(exec_ctx, sqc, ret);
      if (temp_ret != OB_SUCCESS) {
        LOG_WARN("fail to deal with init sqc error", K(exec_ctx), K(sqc), K(temp_ret));
      }
    } else {
      LOG_WARN("fail to wait all async init sqc", K(ret), K(exec_ctx));
    }
    // ????????????process???sqc, ?????????sqc report???, ??????????????????wait_running_dfo????????????????????????sqc??????
    const ObSqcAsyncCB *cb = NULL;
    const ObArray<ObSqcAsyncCB *> &callbacks = proxy.get_callbacks();
    for (int i = 0; i < callbacks.count(); ++i) {
      cb = callbacks.at(i);
      if (OB_NOT_NULL(cb) && cb->is_processed() &&
          OB_SUCCESS == cb->get_ret_code().rcode_ &&
          OB_SUCCESS == cb->get_result().rc_) {
        ObPxSqcMeta &sqc = *sqcs.at(i);
        sqc.set_need_report(true);
      }
    }
  } else {
    const ObArray<ObSqcAsyncCB *> &callbacks = proxy.get_callbacks();
    ARRAY_FOREACH(callbacks, idx) {
      const ObSqcAsyncCB *cb = callbacks.at(idx);
      const ObPxRpcInitSqcResponse &resp = (*cb).get_result();
      ObPxSqcMeta &sqc = *sqcs.at(idx);
      sqc.set_need_report(true);
      if (!fast_sqc) {
        ObPxInitSqcResultMsg pkt;
        pkt.dfo_id_ = sqc.get_dfo_id();
        pkt.sqc_id_ = sqc.get_sqc_id();
        pkt.rc_ = resp.rc_;
        pkt.task_count_ = resp.reserved_thread_count_;
        if (resp.reserved_thread_count_ < sqc.get_max_task_count()) {
          LOG_INFO("SQC do not have enough thread, Downgraded thread allocation",
                   K(resp), K(sqc));
        }
        if (OB_FAIL(pkt.tablets_info_.assign(resp.partitions_info_))) {
          LOG_WARN("Failed to assign partition info", K(ret));
        } else if (OB_FAIL(proc_.on_sqc_init_msg(exec_ctx, pkt))) {
          LOG_WARN("fail to do sqc init callback", K(resp), K(pkt), K(ret));
        }
      }
    }
  }
  return ret;
}

int ObParallelDfoScheduler::deal_with_init_sqc_error(ObExecContext &exec_ctx,
                                                 const ObPxSqcMeta &sqc,
                                                 int rc) const
{
  int ret = OB_SUCCESS;
  if (is_data_not_readable_err(rc) || is_server_down_error(ret)) {
    // ?????????????????????????????????????????????????????????????????????????????????
    // ??????????????????observer??????retry info???invalid servers??????
    const ObAddr &invalid_server = sqc.get_exec_addr();
    ObSQLSessionInfo *session = NULL;
    if (OB_ISNULL(session = GET_MY_SESSION(exec_ctx))) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("session is NULL", K(ret));
    } else {
      ObQueryRetryInfo &retry_info = session->get_retry_info_for_update();
      int add_ret = retry_info.add_invalid_server_distinctly(invalid_server, true);
      if (OB_UNLIKELY(OB_SUCCESS != add_ret)) {
        LOG_WARN("fail to add dist addr to invalid servers distinctly",
                 K(rc), "sqc", sqc, K(add_ret));
      }
    }
  }
  return ret;
}

// ???lightweight SQC ??????????????? server???????????????????????? px ???????????????
// ?????????????????????
int ObParallelDfoScheduler::fast_dispatch_sqc(ObExecContext &exec_ctx,
                                                      ObDfo &dfo,
                                                      ObArray<ObPxSqcMeta *> &sqcs) const
{
  int ret = OB_SUCCESS;
  int64_t timeout_us =  0;
  const ObPhysicalPlan *phy_plan = NULL;
  ObPhysicalPlanCtx *phy_plan_ctx = NULL;
  ObSQLSessionInfo *session = NULL;

  if (OB_UNLIKELY(NULL == (phy_plan = dfo.get_phy_plan()))) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("NULL plan ptr unexpected", K(ret));
  } else if (OB_ISNULL(phy_plan_ctx = GET_PHY_PLAN_CTX(exec_ctx))) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("phy plan ctx NULL", K(ret));
  } else if (OB_ISNULL(session = exec_ctx.get_my_session())) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("session is NULL", K(ret));
  }


  // ?????? sqc ????????????????????????????????????????????????
  //  1. ?????? sqc ??? rpc ??????
  //  2. ?????? sqc ??? rpc ???????????? sqc ????????????????????? worker ??????
  // ?????????????????????????????? dfo ?????????????????????????????????????????????
  int64_t cluster_id = GCONF.cluster_id;
  ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
    ObPxSqcMeta &sqc = *sqcs.at(idx);
    const ObAddr &addr = sqc.get_exec_addr();
    auto proxy = coord_info_.rpc_proxy_.to(addr);
    if (OB_UNLIKELY(share::ObServerBlacklist::get_instance().is_in_blacklist(
                      share::ObCascadMember(addr, cluster_id), true /* add_server */,
                      session->get_process_query_time()))) {
      if (!sqc.is_ignore_vtable_error()) {
        ret = OB_RPC_CONNECT_ERROR;
        LOG_WARN("peer no in communication, maybe crashed", K(ret), K(sqc), K(cluster_id),
                K(session->get_process_query_time()));
      } else {
        LOG_WARN("ignore the black server list with virtual table", K(ret));
      }
    }
    if (OB_FAIL(ret)) {
    } else {
      SMART_VAR(ObPxRpcInitSqcArgs, args) {
        ObPxRpcInitSqcResponse resp;
        timeout_us = phy_plan_ctx->get_timeout_timestamp() - ObTimeUtility::current_time();
        args.set_serialize_param(exec_ctx, const_cast<ObOpSpec &>(*dfo.get_root_op_spec()), *phy_plan);
        if (timeout_us <= 0) {
          ret = OB_TIMEOUT;
        } else if (OB_FAIL(args.sqc_.assign(sqc))) {
          LOG_WARN("fail assign sqc", K(ret));
        } else if (OB_FAIL(proxy
                          .by(THIS_WORKER.get_rpc_tenant()?: session->get_effective_tenant_id())
                          .timeout(timeout_us)
                          .init_sqc(args, resp))) {
          LOG_WARN("fail dispatch dfo rpc", K(sqc), K(ret));
        }
        LOG_TRACE("Sent lw dfo to addr", K(dfo), K(addr), K(args), K(resp));
      }
    }
  }
  return ret;
}


/* ????????? sqc ???????????????????????????????????????
 * ???????????????????????? dfo ????????? sqc????????? qc-sqc ?????????
 * ????????????????????????????????????????????? dfo
 */
int ObParallelDfoScheduler::do_cleanup_dfo(ObDfo &dfo) const
{
  int ret = OB_SUCCESS;
  int tmp_ret = ObInterruptUtil::broadcast_dfo(&dfo, OB_GOT_SIGNAL_ABORTING);
  if (OB_SUCCESS != tmp_ret) {
    LOG_WARN("fail broadcast interrupt to dfo", K(dfo), K(tmp_ret));
  } else {
    LOG_INFO("succ broadcast OB_GOT_SIGNAL_ABORTING to dfo sqcs", K(dfo));
  }
  tmp_ret = ObInterruptUtil::regenerate_interrupt_id(dfo);
  if (OB_SUCCESS != tmp_ret) {
    LOG_WARN("fail regenerate interrupt id for next round retry", K(dfo), K(tmp_ret));
  }
  // cleanup qc-sqc channel
  ObArray<ObPxSqcMeta *> sqcs;
  if (OB_FAIL(dfo.get_sqcs(sqcs))) {
    LOG_WARN("fail get dfo sqc", K(dfo), K(ret));
  } else {
    ARRAY_FOREACH_X(sqcs, idx, cnt, OB_SUCC(ret)) {
      ObPxSqcMeta &sqc = *sqcs.at(idx);
      sqc.set_need_report(false);
      ObDtlChannel *ch = NULL;
      if (NULL != (ch = sqc.get_qc_channel())) {
        (void)coord_info_.msg_loop_.unregister_channel(*ch);
        sqc.set_qc_channel(NULL);
        ObDtlChannelInfo &ci = sqc.get_qc_channel_info();
        if (OB_FAIL(ObDtlChannelGroup::unlink_channel(ci))) {
          LOG_WARN("fail unlink channel", K(ci), K(ret));
        }
      }
    }
  }
  return ret;
}

int ObParallelDfoScheduler::try_schedule_next_dfo(ObExecContext &ctx) const
{
  int ret = OB_SUCCESS;
  FLTSpanGuard(px_schedule);
  ObSEArray<ObDfo *, 3> dfos;
  while (OB_SUCC(ret)) {
    // ???????????????????????? DFO???parent & child
    if (OB_FAIL(coord_info_.dfo_mgr_.get_ready_dfos(dfos))) {
      if (OB_ITER_END != ret) {
        LOG_WARN("fail get ready dfos", K(ret));
      } else {
        LOG_TRACE("No more dfos to schedule", K(ret));
      }
    } else if (0 == dfos.count()) {
      LOG_TRACE("No dfos to schedule for now. wait");
      break;
    } else if (2 != dfos.count()) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("get_ready_dfo should output a pair of dfo",
               "actual", dfos.count(), "expect", 2, K(ret));
    } else if (OB_ISNULL(dfos.at(0)) || OB_ISNULL(dfos.at(1))) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("NULL unexpetected", K(ret));
    } else {
      /*
       * ??? get_ready_dfo() ?????????0 ?????? child???1 ?????? parent???
       *
       *   parent  <-- 1
       *   /
       * child  <-- 0
       */
      ObDfo &child = *dfos.at(0);
      ObDfo &parent = *dfos.at(1);
      LOG_TRACE("to schedule", K(parent), K(child));
      if (OB_FAIL(schedule_pair(ctx, child, parent))) {
        LOG_WARN("fail schedule parent and child", K(ret));
      }
      FLT_SET_TAG(dfo_id, parent.get_dfo_id(),
                  qc_id, parent.get_qc_id(),
                  used_worker_cnt, parent.get_used_worker_count());
    }
  }
  return ret;
}

int ObParallelDfoScheduler::schedule_pair(ObExecContext &exec_ctx,
                                                  ObDfo &child,
                                                  ObDfo &parent) const
{
  int ret = OB_SUCCESS;
  //
  // for scan dfo:  dop + ranges -> dop + svr -> (svr1, th_cnt1), (svr2, th_cnt2), ...
  // for other dfo: dop + child svr -> (svr1, th_cnt1), (svr2, th_cnt2), ...
  //
  if (OB_SUCC(ret)) {
    // ????????????????????????????????????????????? child ???????????????????????? parent ????????????????????????
    if (!child.is_scheduled() && child.has_child_dfo()) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("a interm node with child running should not be in state of unscheduled",
               K(child), K(ret));
    } else if (!child.is_scheduled()) {
      if (child.has_temp_table_scan()) {
        if (OB_FAIL(ObPXServerAddrUtil::alloc_by_temp_child_distribution(exec_ctx,
                                                                         child))) {
          LOG_WARN("fail alloc addr by temp table distribution", K(child), K(ret));
        } else { /*do nohting.*/ }
      } else if (OB_FAIL(ObPXServerAddrUtil::alloc_by_data_distribution(
            coord_info_.pruning_table_location_,
            exec_ctx,
            child))) {
        LOG_WARN("fail alloc addr by data distribution", K(child), K(ret));
      }
      if (OB_SUCC(ret)) {
        if (OB_FAIL(set_temp_table_ctx_for_sqc(exec_ctx, child))) {
          LOG_WARN("failed to set temp table ctx", K(ret));
        }
      }
      LOG_TRACE("alloc_by_data_distribution", K(child));
    } else {
      // already in schedule, pass
    }
  }
  if (OB_SUCC(ret)) {
    if (!parent.is_scheduled()) {
      if (parent.has_temp_table_scan()) {
        if (OB_FAIL(ObPXServerAddrUtil::alloc_by_temp_child_distribution(exec_ctx,
                                                                         parent))) {
          LOG_WARN("fail alloc addr by data distribution", K(parent), K(ret));
        } else { /*do nohting.*/ }
      } else if (parent.is_root_dfo()) {
        // QC/local dfo????????????????????????????????????????????????????????????
        if (OB_FAIL(ObPXServerAddrUtil::alloc_by_local_distribution(exec_ctx,
                                                                    parent))) {
          LOG_WARN("alloc SQC on local failed", K(parent), K(ret));
        }
      } else {
        // DONE (xiaochu): ?????? parent dfo ??????????????? scan??????????????????????????????dfo ?????? scan
        // ?????????????????????????????? ??? child dfo ????????????????????? shuffle ??? parent ??????????????????
        // ?????????????????????????????????
        // (1) parent ??? child ??????????????? parent ???????????????????????????
        // (2) parent ?????????child ????????? parent shuffle???????????? parent ??????????????????????????????
        //     ??????pdml global index maintain?????????????????????
        // (3) parent???child ???????????????????????????????????????????????????????????????????????????????????????????????????
        // (4) parent???child????????????slave mapping??????????????????parent??????reference table??????????????????
        // sqc???
        //
        // ????????????????????????????????????????????????????????????????????????????????? TODO
        if (parent.has_scan_op() || parent.has_dml_op()) { // ?????? Partial Partition Wise Join
          // ???DFO?????????TSC??????pdml??????global index maintain op???
          // 1. ?????????TSC????????????sqcs???location????????????tsc??????location??????
          // 2. ??????pdml???dml+px????????????sqcs???locations????????????DML???????????????locations
          if (OB_FAIL(ObPXServerAddrUtil::alloc_by_data_distribution(
            coord_info_.pruning_table_location_, exec_ctx, parent))) {
            LOG_WARN("fail alloc addr by data distribution", K(parent), K(ret));
          }
          LOG_TRACE("alloc_by_data_distribution", K(parent));
        } else if (parent.is_single()) {
          // parent ??????????????? scalar group by?????????????????? is_local?????????
          // ??? alloc_by_data_distribution???????????????????????? QC ?????????????????????
          if (OB_FAIL(ObPXServerAddrUtil::alloc_by_data_distribution(
            coord_info_.pruning_table_location_, exec_ctx, parent))) {
            LOG_WARN("fail alloc addr by data distribution", K(parent), K(ret));
          }
          LOG_TRACE("alloc_by_local_distribution", K(parent));
        } else if (ObPQDistributeMethod::PARTITION_HASH == child.get_dist_method()) {
          if (OB_FAIL(ObPXServerAddrUtil::alloc_by_reference_child_distribution(
                  coord_info_.pruning_table_location_,
                  exec_ctx,
                  child, parent))) {
            LOG_WARN("fail alloc addr by data distribution", K(parent), K(child), K(ret));
          }
        } else if (OB_FAIL(ObPXServerAddrUtil::alloc_by_random_distribution(exec_ctx, child, parent))) {
          LOG_WARN("fail alloc addr by data distribution", K(parent), K(child), K(ret));
        }
        LOG_TRACE("alloc_by_child_distribution", K(child), K(parent));
      }
      if (OB_SUCC(ret)) {
        if (OB_FAIL(set_temp_table_ctx_for_sqc(exec_ctx, parent))) {
          LOG_WARN("failed to set temp table ctx", K(ret));
        }
      }
    } else {
      // already in schedule, pass
    }
  }



  // ???????????????QC ????????? child dfo ?????????????????????????????????????????????????????????
  bool can_prealloc = false;
  if (OB_SUCC(ret)) {
    if (OB_FAIL(check_if_can_prealloc_xchg_ch(child, parent, can_prealloc))) {
      LOG_WARN("fail check can prealloc xchange, ingore", K(ret));
    } else if (can_prealloc) {
      if (OB_FAIL(do_fast_schedule(exec_ctx, child, parent))) {
        LOG_WARN("fail do fast schedule", K(parent), K(child), K(ret));
      }
    }
  }

  // ???????????????????????????????????????????????????
  // ??????????????? do_fast_schedule ??????????????? parent / child
  // ???????????? is_schedule ????????????????????? true???????????? schedule_dfo
  // ???????????????????????????
  //
  if (child.is_px_use_bloom_filter() && parent.is_px_create_bloom_filter()) {
    // if with bloom filter, it is better to schedule parent first
    // because child dfo depends on parent dfo's bloom filter creation
    if (OB_SUCC(ret)) {
      if (!parent.is_scheduled()) {
        if (OB_FAIL(schedule_dfo(exec_ctx, parent))) { // ?????? DFO ????????? server
          LOG_WARN("fail schedule dfo", K(parent), K(ret));
        }
      }
    }
    if (OB_SUCC(ret)) {
      if (!child.is_scheduled()) {
        if (OB_FAIL(schedule_dfo(exec_ctx, child))) { // ?????? DFO ????????? server
          LOG_WARN("fail schedule dfo", K(child), K(ret));
        }
      }
    }
  } else {
    // otherwise I'd like to schedule child first
    // because child can do some useful (e.g. scan) work while parent is scheduling
    if (OB_SUCC(ret)) {
      if (!child.is_scheduled()) {
        if (OB_FAIL(schedule_dfo(exec_ctx, child))) { // ?????? DFO ????????? server
          LOG_WARN("fail schedule dfo", K(child), K(ret));
        }
      }
    }
    if (OB_SUCC(ret)) {
      if (!parent.is_scheduled()) {
        if (OB_FAIL(schedule_dfo(exec_ctx, parent))) { // ?????? DFO ????????? server
          LOG_WARN("fail schedule dfo", K(parent), K(ret));
        }
      }
    }
  }
  return ret;
}
