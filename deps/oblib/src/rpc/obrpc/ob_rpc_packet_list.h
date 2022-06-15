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

#define MAX_PCODE 0xFFFF

// for initialization
PCODE_DEF(OB_TEST_PCODE, 0x001)
PCODE_DEF(OB_TEST2_PCODE, 0x002)
PCODE_DEF(OB_TEST3_PCODE, 0x003)
PCODE_DEF(OB_TEST4_PCODE, 0x004)
PCODE_DEF(OB_TEST5_PCODE, 0x005)
PCODE_DEF(OB_TEST6_PCODE, 0x006)
PCODE_DEF(OB_TEST7_PCODE, 0x007)
PCODE_DEF(OB_TEST8_PCODE, 0x008)
PCODE_DEF(OB_TEST9_PCODE, 0x009)

// INVALID REQUEST PCODE, no such rpc
PCODE_DEF(OB_ERROR_PACKET, 0x010)

// for test rpc stream compress
PCODE_DEF(OB_RPC_STREAM_TEST_DECODE_COMPRESS_PCODE, 0x011)
PCODE_DEF(OB_RPC_STREAM_TEST_DECODE_RAW_PCODE, 0x012)
PCODE_DEF(OB_RPC_STREAM_TEST_ENCODE_SEGMENT_PCODE, 0x013)
PCODE_DEF(OB_RPC_STREAM_TEST_ENCODE_RAW_PCODE, 0x014)
PCODE_DEF(OB_RPC_STREAM_TEST_INIT_CTX_PCODE, 0x015)

// core, manager
PCODE_DEF(OB_BOOTSTRAP, 0x101)
PCODE_DEF(OB_RENEW_LEASE, 0x102)
PCODE_DEF(OB_IS_EMPTY_SERVER, 0x103)
PCODE_DEF(OB_SET_ROLE, 0x104)
PCODE_DEF(OB_GET_ROLE, 0x105)
PCODE_DEF(OB_SET_CONFIG, 0x106)
PCODE_DEF(OB_GET_CONFIG, 0x107)
PCODE_DEF(OB_EXECUTE_BOOTSTRAP, 0x108)
PCODE_DEF(OB_REFRESH_CONFIG, 0x109)
PCODE_DEF(OB_ROOT_MAJOR_FREEZE, 0x10A)
PCODE_DEF(OB_ADD_SERVER, 0x10B)
PCODE_DEF(OB_DELETE_SERVER, 0x10C)
PCODE_DEF(OB_CANCEL_DELETE_SERVER, 0x10D)
PCODE_DEF(OB_ADD_ZONE, 0x10E)
PCODE_DEF(OB_DELETE_ZONE, 0x10F)
PCODE_DEF(OB_START_ZONE, 0x110)
PCODE_DEF(OB_STOP_ZONE, 0x111)
PCODE_DEF(OB_ADD_TENANT_TMP, 0x112)

PCODE_DEF(OB_TENANT_MGR, 0x113)
PCODE_DEF(OB_MERGE_FINISH, 0x114)
PCODE_DEF(OB_MERGE_ERROR, 0x115)
PCODE_DEF(OB_START_SERVER, 0x116)
PCODE_DEF(OB_STOP_SERVER, 0x117)
PCODE_DEF(OB_BROADCAST_DS_ACTION, 0x118)
PCODE_DEF(OB_GET_FROZEN_VERSION, 0x119)
PCODE_DEF(OB_GET_FROZEN_STATUS, 0x11A)
PCODE_DEF(OB_FETCH_ALIVE_SERVER, 0x11B)
PCODE_DEF(OB_ROOT_MINOR_FREEZE, 0x11C)
PCODE_DEF(OB_ALTER_ZONE, 0x11D)
PCODE_DEF(OB_RESTORE_TENANT, 0x11E)
PCODE_DEF(OB_GET_CLUSTER_INFO, 0x11F)
PCODE_DEF(OB_CLUSTER_HB, 0x120)
PCODE_DEF(OB_NOTIFY_TENANT_SERVER_UNIT_RESOURCE, 0x121)
PCODE_DEF(OB_LOG_DDL_NOP_OPERATOR, 0x122)
PCODE_DEF(OB_GET_TENANT_SCHEMA_VERSIONS, 0x123)
PCODE_DEF(OB_BROADCAST_SCHEMA, 0x124)
PCODE_DEF(OB_CLUSTER_REGIST, 0x125)
PCODE_DEF(OB_UPDATE_CLUSTER_INFO, 0x126)
PCODE_DEF(OB_GET_SCHEMA_SNAPSHOT, 0x127)
PCODE_DEF(OB_CHECK_GTS_REPLICA_FOR_STOP_SERVER, 0x128)
PCODE_DEF(OB_CHECK_GTS_REPLICA_FOR_STOP_ZONE, 0x129)
PCODE_DEF(OB_FINISH_SCHEMA_SPLIT, 0x13A)
PCODE_DEF(OB_GET_TENANT_REFRESHED_SCHEMA_VERSION, 0x13B)
PCODE_DEF(OB_FINISH_REPLAY_SCHEMA, 0x13C)
PCODE_DEF(OB_RESTORE_PARTITIONS, 0x13D)
PCODE_DEF(OB_GET_MASTER_RS, 0x13E)
PCODE_DEF(OB_GET_TENANT_UNIT_DIST, 0x13F)  // compat only
PCODE_DEF(OB_CHECK_STANDBY_CAN_ACCESS, 0x140)
PCODE_DEF(OB_REFRESH_MEMORY_STAT, 0x141)
PCODE_DEF(OB_UPDATE_TABLE_SCHEMA_VERSION, 0x142)
PCODE_DEF(OB_CHECK_DEPLOYMENT_MODE, 0x143)
PCODE_DEF(OB_UPDATE_FREEZE_SCHEMA_VERSIONS, 0x144)
PCODE_DEF(OB_GET_CLUSTER_INFO_IN_STRICT_MODE, 0x145)
PCODE_DEF(OB_GET_CLUSTER_STATS, 0x146)
// remote interrupt call
PCODE_DEF(OB_REMOTE_INTERRUPT_CALL, 0x1EE)

PCODE_DEF(OB_GET_DIAGNOSE_ARGS, 0x1FF)

// DDL relating pcode
PCODE_DEF(OB_FETCH_SCHEMA, 0x201)
PCODE_DEF(OB_SWITCH_SCHEMA, 0x202)
PCODE_DEF(OB_CREATE_TABLE, 0x203)
PCODE_DEF(OB_DROP_TABLE, 0x204)
PCODE_DEF(OB_ALTER_TABLE, 0x205)
PCODE_DEF(OB_CREATE_TENANT, 0x206)
PCODE_DEF(OB_DROP_TENANT, 0x207)
PCODE_DEF(OB_CREATE_DATABASE, 0x208)
PCODE_DEF(OB_DROP_DATABASE, 0x209)
PCODE_DEF(OB_CREATE_PARTITION, 0x20A)
PCODE_DEF(OB_SWITCH_LEADER, 0x20B)
PCODE_DEF(OB_CREATE_TABLEGROUP, 0x20C)
PCODE_DEF(OB_DROP_TABLEGROUP, 0x20D)
PCODE_DEF(OB_CREATE_INDEX, 0x20E)
PCODE_DEF(OB_DROP_INDEX, 0x20F)
PCODE_DEF(OB_MODIFY_TENANT, 0x210)
PCODE_DEF(OB_LOCK_TENANT, 0x211)
PCODE_DEF(OB_CREATE_RESOURCE_UNIT, 0x212)
PCODE_DEF(OB_DROP_RESOURCE_UNIT, 0x213)
PCODE_DEF(OB_CREATE_RESOURCE_POOL, 0x214)
PCODE_DEF(OB_ALTER_RESOURCE_POOL, 0x215)
PCODE_DEF(OB_DROP_RESOURCE_POOL, 0x216)
PCODE_DEF(OB_CREATE_VIEW, 0x217)
PCODE_DEF(OB_ALTER_VIEW, 0x218)
PCODE_DEF(OB_DROP_VIEW, 0x219)
PCODE_DEF(OB_ALTER_TABLEGROUP, 0x21A)
PCODE_DEF(OB_UPDATE_INDEX_TABLE_STATUS, 0x21B)
PCODE_DEF(OB_RENAME_TABLE, 0x21C)
PCODE_DEF(OB_TRUNCATE_TABLE, 0x21D)
PCODE_DEF(OB_ALTER_DATABASE, 0x21E)
PCODE_DEF(OB_CREATE_TABLE_LIKE, 0x21F)
PCODE_DEF(OB_GET_LEADER_CANDIDATES, 0x220)
PCODE_DEF(OB_REQUEST_HEARTBEAT, 0x221)
PCODE_DEF(OB_BROADCAST_SYS_SCHEMA, 0x222)
PCODE_DEF(OB_ALTER_RESOURCE_UNIT, 0x223)
PCODE_DEF(OB_CREATE_PARTITION_BATCH, 0x224)
PCODE_DEF(OB_GET_PARTITION_COUNT, 0x226)
PCODE_DEF(OB_SWITCH_LEADER_LIST, 0x227)
PCODE_DEF(OB_FLASHBACK_TABLE_FROM_RECYCLEBIN, 0x228)
PCODE_DEF(OB_PURGE_TABLE, 0x229)
PCODE_DEF(OB_FLASHBACK_DATABASE, 0x22A)
PCODE_DEF(OB_PURGE_DATABASE, 0x22B)
PCODE_DEF(OB_PURGE_EXPIRE_RECYCLE_OBJECTS, 0x22C)
PCODE_DEF(OB_SWITCH_LEADER_LIST_ASYNC, 0x22D)
PCODE_DEF(OB_GET_LEADER_CANDIDATES_ASYNC, 0x22E)
PCODE_DEF(OB_COMMIT_ALTER_TENANT_LOCALITY, 0x22F)
PCODE_DEF(OB_COMMIT_ALTER_TABLE_LOCALITY, 0x230)
PCODE_DEF(OB_SET_GLOBAL_VARIABLE, 0x231)
PCODE_DEF(OB_UPDATE_SYS_VAR_VERSION, 0x232)
PCODE_DEF(OB_ALTER_TABLE_V2, 0x233)
PCODE_DEF(OB_ADD_SYSVAR, 0x234)
PCODE_DEF(OB_MODIFY_SYSVAR, 0x235)
PCODE_DEF(OB_FLASHBACK_INDEX, 0x236)
PCODE_DEF(OB_PURGE_INDEX, 0x237)
PCODE_DEF(OB_CHECK_UNIQUE_INDEX_REQUEST, 0x238)
PCODE_DEF(OB_CHECK_UNIQUE_INDEX_RESPONSE, 0x239)
PCODE_DEF(OB_CHECK_SINGLE_REPLICA_MAJOR_SSTABLE_EXIST, 0x23A)
PCODE_DEF(OB_CHECK_ALL_REPLICA_MAJOR_SSTABLE_EXIST, 0x23B)
PCODE_DEF(OB_COMMIT_ALTER_TABLEGROUP_LOCALITY, 0x23C)
PCODE_DEF(OB_QUERY_MAX_DECIDED_TRANS_VERSION, 0x23D)
PCODE_DEF(OB_QUERY_IS_VALID_MEMBER, 0x23E)
PCODE_DEF(OB_GET_LEADER_CANDIDATES_ASYNC_V2, 0x23F)
PCODE_DEF(OB_SPLIT_RESOURCE_POOL, 0x240)
PCODE_DEF(OB_CHECK_SINGLE_REPLICA_MAJOR_SSTABLE_EXIST_WITH_TIME, 0x241)
PCODE_DEF(OB_CHECK_ALL_REPLICA_MAJOR_SSTABLE_EXIST_WITH_TIME, 0x242)
PCODE_DEF(OB_CALC_COLUMN_CHECKSUM_REQUEST, 0x243)
PCODE_DEF(OB_CALC_COLUMN_CHECKSUM_RESPONSE, 0x244)
PCODE_DEF(OB_DO_SEQUENCE_DDL, 0x245)
PCODE_DEF(OB_ALTER_CLUSTER_ATTR_DDL, 0x246)
PCODE_DEF(OB_ALTER_CLUSTER, 0x247)
PCODE_DEF(OB_CREATE_TENANT_END, 0x248)
PCODE_DEF(OB_GEN_NEXT_SCHEMA_VERSION, 0x249)
PCODE_DEF(OB_REACH_PARTITION_LIMIT, 0x24A)
PCODE_DEF(OB_ALTER_CLUSTER_INFO, 0x24B)
PCODE_DEF(OB_CHECK_MERGE_FINISH, 0x24C)
PCODE_DEF(OB_CHECK_CLUSTER_VALID_TO_ADD, 0x24D)
PCODE_DEF(OB_GET_STANDBY_CLUSTER_STATISTIC, 0x24F)

// Privilege relating pcode
PCODE_DEF(OB_CREATE_USER, 0x250)
PCODE_DEF(OB_DROP_USER, 0x251)
PCODE_DEF(OB_RENAME_USER, 0x252)
PCODE_DEF(OB_SET_PASSWD, 0x253)
PCODE_DEF(OB_GRANT, 0x254)
PCODE_DEF(OB_REVOKE_USER, 0x255)
PCODE_DEF(OB_LOCK_USER, 0x256)
PCODE_DEF(OB_REVOKE_DB, 0x257)
PCODE_DEF(OB_REVOKE_TABLE, 0x258)
PCODE_DEF(OB_OPTIMIZE_TABLE, 0x259)
PCODE_DEF(OB_BATCH_SET_MEMBER_LIST, 0x25A)
PCODE_DEF(OB_GET_SWITCHOVER_STATUS, 0x25B)

// profile
PCODE_DEF(OB_DO_PROFILE_DDL, 0x25D)
PCODE_DEF(OB_ALTER_USER_PROFILE, 0x25E)

PCODE_DEF(OB_STANDBY_GRANT, 0x261)
PCODE_DEF(OB_FORCE_DROP_SCHEMA, 0x262)
PCODE_DEF(OB_BATCH_WAIT_LEADER, 0x263)
PCODE_DEF(OB_UPGRADE_STANDBY_SCHEMA, 0x264)
PCODE_DEF(OB_MERGE_RESOURCE_POOL, 0x265)
PCODE_DEF(OB_BATCH_WRITE_CUTDATA_CLOG, 0x266)

PCODE_DEF(OB_REVOKE_SYSPRIV, 0x270)

// auto part ddl
PCODE_DEF(OB_EXECUTE_RANGE_PART_SPLIT, 0x271)
PCODE_DEF(OB_CLUSTER_ACTION_VERIFY, 0x273)
PCODE_DEF(OB_SUBMIT_BUILD_INDEX_TASK, 0x274)
PCODE_DEF(OB_FETCH_SSTABLE_SIZE, 0x275)

// system admin commnad
PCODE_DEF(OB_ADMIN_SWITCH_REPLICA_ROLE, 0x280)
PCODE_DEF(OB_ADMIN_DROP_REPLICA, 0x281)
PCODE_DEF(OB_ADMIN_MIGRATE_REPLICA, 0x282)
PCODE_DEF(OB_ADMIN_REPORT_REPLICA, 0x283)
PCODE_DEF(OB_ADMIN_RECYCLE_REPLICA, 0x284)
PCODE_DEF(OB_ADMIN_MERGE, 0x285)
PCODE_DEF(OB_ADMIN_CLEAR_ROOTTABLE, 0x286)
PCODE_DEF(OB_ADMIN_REFRESH_SCHEMA, 0x287)
PCODE_DEF(OB_ADMIN_SET_CONFIG, 0x288)
PCODE_DEF(OB_ADMIN_CLEAR_LOCATION_CACHE, 0x289)
PCODE_DEF(OB_ADMIN_RELOAD_UNIT, 0x28A)
PCODE_DEF(OB_ADMIN_RELOAD_SERVER, 0x28B)
PCODE_DEF(OB_ADMIN_RELOAD_ZONE, 0x28C)
PCODE_DEF(OB_ADMIN_CLEAR_MERGE_ERROR, 0x28D)
PCODE_DEF(OB_ADMIN_MIGRATE_UNIT, 0x28E)
PCODE_DEF(OB_ADMIN_UPGRADE_VIRTUAL_SCHEMA, 0x28F)
PCODE_DEF(OB_RUN_JOB, 0x290)
PCODE_DEF(OB_ADMIN_SWITCH_RS_ROLE, 0x291)
PCODE_DEF(OB_ADMIN_FLUSH_CACHE, 0x292)
PCODE_DEF(OB_ADMIN_UPGRADE_CMD, 0x293)
PCODE_DEF(OB_ADMIN_SET_MERGE_LIST, 0x294)
PCODE_DEF(OB_ADMIN_CHANGE_REPLICA, 0x295)
PCODE_DEF(OB_BATCH_START_ELECTION, 0X296)
PCODE_DEF(OB_BATCH_FLASHBACK, 0x297)
PCODE_DEF(OB_PARTITION_STOP_WRITE, 0x298)
PCODE_DEF(OB_PARTITION_CHECK_LOG, 0x299)
PCODE_DEF(OB_GET_WRS_INFO, 0x29B)
PCODE_DEF(OB_ADMIN_RELOAD_GTS, 0x29C)
PCODE_DEF(OB_BATCH_PREPARE_FLASHBACK, 0x29D)
PCODE_DEF(OB_ADMIN_REFRESH_MEMORY_STAT, 0x29E)
PCODE_DEF(OB_ADMIN_ROLLING_UPGRADE_CMD, 0x29F)
PCODE_DEF(OB_BATCH_SWITCH_RS_LEADER, 0x2A0)
PCODE_DEF(OB_RUN_UPGRADE_JOB, 0x2A1)

PCODE_DEF(OB_CREATE_OUTLINE, 0x350)
PCODE_DEF(OB_DROP_OUTLINE, 0x351)
PCODE_DEF(OB_ALTER_OUTLINE, 0x352)
PCODE_DEF(OB_RS_SET_TP, 0x353)
PCODE_DEF(OB_SET_TP, 0x354)
PCODE_DEF(OB_REFRESH_TIME_ZONE_INFO, 0x355)
PCODE_DEF(OB_REQUEST_TIME_ZONE_INFO, 0x356)
PCODE_DEF(OB_CREATE_SYNONYM, 0x357)
PCODE_DEF(OB_DROP_SYNONYM, 0x358)
PCODE_DEF(OB_CREATE_PLAN_BASELINE, 0x359)
PCODE_DEF(OB_DROP_PLAN_BASELINE, 0x360)
PCODE_DEF(OB_ALTER_PLAN_BASELINE, 0x361)
PCODE_DEF(OB_LOAD_BASELINE, 0x362)
PCODE_DEF(OB_ADMIN_LOAD_BASELINE, 0x363)
PCODE_DEF(OB_CREATE_ROUTINE, 0x364)
PCODE_DEF(OB_DROP_ROUTINE, 0x365)
PCODE_DEF(OB_CREATE_PACKAGE, 0x366)
PCODE_DEF(OB_ALTER_PACKAGE, 0x367)
PCODE_DEF(OB_DROP_PACKAGE, 0x368)
PCODE_DEF(OB_CREATE_USER_DEFINED_FUNCTION, 0x369)
PCODE_DEF(OB_DROP_USER_DEFINED_FUNCTION, 0x370)
PCODE_DEF(OB_ALTER_ROUTINE, 0x371)
PCODE_DEF(OB_CREATE_UDT, 0x372)
PCODE_DEF(OB_DROP_UDT, 0x373)
PCODE_DEF(OB_CREATE_TRIGGER, 0x374)
PCODE_DEF(OB_ALTER_TRIGGER, 0x375)
PCODE_DEF(OB_DROP_TRIGGER, 0x376)
PCODE_DEF(OB_CREATE_DBLINK, 0x377)
PCODE_DEF(OB_DROP_DBLINK, 0x378)
PCODE_DEF(OB_ALTER_DBLINK, 0x379)
PCODE_DEF(OB_FLASHBACK_TENANT, 0x37A)
PCODE_DEF(OB_PURGE_TENANT, 0x37B)
PCODE_DEF(OB_CREATE_RESTORE_POINT, 0x37D)
PCODE_DEF(OB_DROP_RESTORE_POINT, 0x37E)
PCODE_DEF(OB_GET_RECYCLE_SCHEMA_VERSIONS, 0x37F)

// ob server
// PCODE_DEF(OB_MIGRATE_OVER, 0x402)
PCODE_DEF(OB_CLEAR_REBUILD_ROOT_PARTITION, 0x403)
PCODE_DEF(OB_REPORT_ROOT_PARTITION, 0x404)
PCODE_DEF(OB_FETCH_ROOT_PARTITION, 0x405)
PCODE_DEF(OB_GET_ROOT_PARTITION, 0x406)
PCODE_DEF(OB_REMOVE_ROOT_PARTITION, 0x407)
PCODE_DEF(OB_GET_PARTITION_STAT, 0x408)
PCODE_DEF(OB_GET_MEMBER_LIST, 0x409)
PCODE_DEF(OB_QUERY_MAJOR_FREEZE_STATUS, 0x40A)
PCODE_DEF(OB_PREPARE_MAJOR_FREEZE, 0x40B)
PCODE_DEF(OB_COMMIT_MAJOR_FREEZE, 0x40C)
PCODE_DEF(OB_ABORT_MAJOR_FREEZE, 0x40D)
PCODE_DEF(OB_FETCH_LOCATION, 0x40E)
// PCODE_DEF(OB_CLONE_PARTITION, 0x40F)
PCODE_DEF(OB_REFRESH_SYNC_VALUE, 0x410)
PCODE_DEF(OB_REPORT_REPLICA, 0x411)
PCODE_DEF(OB_RECYCLE_REPLICA, 0x412)
PCODE_DEF(OB_CLEAR_LOCATION_CACHE, 0x413)
PCODE_DEF(OB_DROP_REPLICA, 0x414)
PCODE_DEF(OB_CLEAR_AUTOINC_CACHE, 0x415)
PCODE_DEF(OB_SYNC_AUTO_INCREMENT, 0x416)
PCODE_DEF(OB_SET_DS_ACTION, 0x417)
PCODE_DEF(OB_CHECK_PARTITION_TABLE, 0x418)
PCODE_DEF(OB_REBUILD_ROOT_PARTITION, 0x419)
PCODE_DEF(OB_SYNC_PARTITION_TABLE, 0x420)
PCODE_DEF(OB_SYNC_PT_FINISH, 0x421)
PCODE_DEF(OB_INCREMENTAL_COPY_DATA, 0x422)
PCODE_DEF(OB_CHECK_SWITCH_ALL_LEADER_OUT, 0x423)
PCODE_DEF(OB_REPORT_SINGLE_REPLICA, 0x424)
PCODE_DEF(OB_CHECK_SCHEMA_VERSION_ELAPSED, 0x425)
// partition online/offline
// PCODE_DEF(OB_ADD_TEMPORARY_REPLICA, 0x425)
PCODE_DEF(OB_ADD_REPLICA, 0x426)
PCODE_DEF(OB_REMOVE_REPLICA, 0x427)
PCODE_DEF(OB_MIGRATE_REPLICA, 0x428)
PCODE_DEF(OB_ADD_REPLICA_RES, 0x429)
PCODE_DEF(OB_CHECK_CTX_CREATE_TIMESTAMP_ELAPSED, 0x42A)
PCODE_DEF(OB_MIGRATE_REPLICA_RES, 0x430)
// PCODE_DEF(OB_ADD_TEMPORARY_REPLICA_RES, 0x431)
PCODE_DEF(OB_REMOVE_MEMBER, 0x432)
PCODE_DEF(OB_REBUILD_REPLICA, 0x433)
PCODE_DEF(OB_SYNC_FROZEN_STATUS, 0x434)
PCODE_DEF(OB_FETCH_BASE_DATA_META, 0x435)  // used for 1.4x upgrade to 2.1 when rebuild
PCODE_DEF(OB_FETCH_MACRO_BLOCK_OLD, 0x436)
PCODE_DEF(OB_CANCEL_SYS_TASK, 0x437)
PCODE_DEF(OB_ADMIN_REBUILD_REPLICA, 0x438)
PCODE_DEF(OB_REBUILD_REPLICA_RES, 0x439)
PCODE_DEF(OB_CHANGE_REPLICA, 0x440)
PCODE_DEF(OB_CHANGE_REPLICA_RES, 0x441)
PCODE_DEF(OB_REMOVE_NON_PAXOS_REPLICA, 0x442)
PCODE_DEF(OB_SET_DISK_VALID, 0x443)
PCODE_DEF(OB_ADMIN_FLUSH_BALANCE_INFO, 0x444)
// PCODE_DEF(OB_GET_ROOT_SERVICE_STATUS, 0x445)

// partition batch online/offline
PCODE_DEF(OB_ADD_REPLICA_BATCH, 0x446)
PCODE_DEF(OB_REMOVE_NON_PAXOS_REPLICA_BATCH, 0x447)
PCODE_DEF(OB_REMOVE_MEMBER_BATCH, 0x448)
PCODE_DEF(OB_MIGRATE_REPLICA_BATCH, 0x449)
PCODE_DEF(OB_CHANGE_REPLICA_BATCH, 0x44A)
PCODE_DEF(OB_REBUILD_REPLICA_BATCH, 0x44B)
PCODE_DEF(OB_ADD_REPLICA_BATCH_RES, 0x44C)
PCODE_DEF(OB_REBUILD_REPLICA_BATCH_RES, 0x44D)
PCODE_DEF(OB_MIGRATE_REPLICA_BATCH_RES, 0x44E)
PCODE_DEF(OB_CHANGE_REPLICA_BATCH_RES, 0x44F)
PCODE_DEF(OB_CHECK_MIGRATE_TASK_EXIST, 0x450)
PCODE_DEF(OB_BATCH_REMOVE_MEMBER, 0x451)
PCODE_DEF(OB_BATCH_ADD_MEMBER, 0x452)
PCODE_DEF(OB_BATCH_MEMBER_CHANGE_DONE, 0x453)
PCODE_DEF(OB_RESTORE_REPLICA, 0x454)
PCODE_DEF(OB_RESTORE_REPLICA_RES, 0x455)
PCODE_DEF(OB_MODIFY_QUORUM_BATCH, 0x456)
PCODE_DEF(OB_CHECK_DANGLING_REPLICA_EXIST, 0x457)
PCODE_DEF(OB_CHECK_DANGLING_REPLICA_FINISH, 0x458)
// 2.0 partition migration related
PCODE_DEF(OB_FETCH_PARTITION_INFO, 0x459)
PCODE_DEF(OB_FETCH_CONSISTENT_CUT, 0x45A)
PCODE_DEF(OB_FETCH_TABLE_INFO, 0x45B)
PCODE_DEF(OB_FETCH_LOGIC_BASE_META, 0x45C)
PCODE_DEF(OB_FETCH_PHYSICAL_BASE_META, 0x45D)
PCODE_DEF(OB_FETCH_LOGIC_ROW, 0x45E)
PCODE_DEF(OB_COPY_LOCAL_INDEX_SSTABLE_BATCH, 0x45F)
PCODE_DEF(OB_COPY_GLOBAL_INDEX_SSTABLE_BATCH, 0x460)
PCODE_DEF(OB_FETCH_MIGRATE_STORAGE_INFO, 0x461)
PCODE_DEF(OB_FETCH_RANGE_INFO, 0x462)
PCODE_DEF(OB_ROOT_SPLIT_PARTITION, 0x463)
PCODE_DEF(OB_SPLIT_PARTITION, 0x464)
PCODE_DEF(OB_FETCH_SNAPSHOT_ROW, 0x465)
PCODE_DEF(OB_FETCH_MEMSTORE_INFO, 0x466)
PCODE_DEF(OB_COPY_GLOBAL_INDEX_RES, 0x467)
PCODE_DEF(OB_COPY_LOCAL_INDEX_RES, 0x468)
// TODO: should remove this rpc after global and local index builder integrated
PCODE_DEF(OB_TMP_OBSERVER_COPY_LOCAL_INDEX_SSTABLE, 0x469)
PCODE_DEF(OB_FETCH_LOGIC_DATA_CHECKSUM, 0x46A)
PCODE_DEF(OB_COPY_SSTABLE_BATCH, 0x46B)
PCODE_DEF(OB_COPY_SSTABLE_BATCH_RES, 0x46C)
PCODE_DEF(OB_CHECK_SYS_TASK_EXIST, 0x46D)
PCODE_DEF(OB_FETCH_LOGIC_DATA_CHECKSUM_SLICE, 0x46E)
PCODE_DEF(OB_FETCH_LOGIC_ROW_SLICE, 0x46F)
PCODE_DEF(OB_BATCH_SPLIT_PARTITION, 0x470)
PCODE_DEF(OB_FETCH_MACRO_BLOCK, 0x471)
PCODE_DEF(OB_FETCH_PARTITION_GROUP_INFO, 0x472)
PCODE_DEF(OB_FETCH_PG_PARTITION_INFO, 0x473)
PCODE_DEF(OB_ADD_DISK, 0x474)
PCODE_DEF(OB_DROP_DISK, 0x475)
PCODE_DEF(OB_CHECK_MEMBER_PG_MAJOR_SSTABLE_ENOUGH, 0x476)
PCODE_DEF(OB_FETCH_PUBLISH_VERSION, 0x477)
PCODE_DEF(OB_UPDATE_BASELINE_SCHEMA_VERSION, 0x478)
PCODE_DEF(OB_GET_REMOTE_TENANT_GROUP_STRING, 0x479)
PCODE_DEF(OB_SYNC_PG_PT_FINISH, 0x47A)
PCODE_DEF(OB_SYNC_PG_PARTITION_TABLE, 0x47B)
PCODE_DEF(OB_CHECK_FROZEN_VERSION, 0x47C)
PCODE_DEF(OB_CHECK_PHYSICAL_FLASHBACK_SUCC, 0x47D)
PCODE_DEF(OB_BACKUP_REPLICA_BATCH, 0x47E)
PCODE_DEF(OB_BACKUP_REPLICA_BATCH_RES, 0x47F)
PCODE_DEF(OB_CHECK_BACKUP_TASK_EXIST, 0x480)
PCODE_DEF(OB_BROADCAST_RS_LIST, 0x484)
PCODE_DEF(OB_GET_MIN_SSTABLE_SCHEMA_VERSION, 0x486)
PCODE_DEF(OB_GET_RECOVERY_POINT_META_INFO, 0x487)
PCODE_DEF(OB_STANDBY_CUTDATA_BATCH_TASK, 0x490)

// sql, including executor

// sql
PCODE_DEF(OB_SQL_PCODE_START, 0x500)  // psudo pcode, as a guardian
PCODE_DEF(OB_PHY_PLAN_EXECUTE, 0x501)
PCODE_DEF(OB_REMOTE_EXECUTE, 0x502)
PCODE_DEF(OB_DIST_EXECUTE, 0x503)
PCODE_DEF(OB_TASK_COMPLETE, 0x504)
PCODE_DEF(OB_TASK_NOTIFY_FETCH, 0x505)
PCODE_DEF(OB_TASK_FETCH_RESULT, 0x506)  // Abandoned
PCODE_DEF(OB_TASK_KILL, 0x507)
PCODE_DEF(OB_CLOSE_RESULT, 0x508)
PCODE_DEF(OB_KILL_SESSION, 0x509)
PCODE_DEF(OB_AP_DIST_EXECUTE, 0x50A)
PCODE_DEF(OB_TASK_FETCH_INTERM_RESULT, 0x50B)
PCODE_DEF(OB_MINI_TASK_EXECUTE, 0x50C)
PCODE_DEF(OB_AP_MINI_DIST_EXECUTE, 0x50D)
PCODE_DEF(OB_PX_INIT_SQC, 0x50E)
PCODE_DEF(OB_PX_INIT_TASK, 0x50F)
PCODE_DEF(OB_PX_INTP_SQC, 0x510)
PCODE_DEF(OB_PX_INTP_QC, 0x511)
PCODE_DEF(OB_LOAD_DATA_EXECUTE, 0x512)
PCODE_DEF(OB_BKGD_DIST_EXECUTE, 0x513)
PCODE_DEF(OB_BKGD_TASK_COMPLETE, 0x514)
PCODE_DEF(OB_ESTIMATE_PARTITION_ROWS, 0x515)
PCODE_DEF(OB_ESTIMATE_PARTITION_ROWS_MOCK, 0x516)  // mock for 1473 OB_ESTIMATE_PARTITION_ROWS
PCODE_DEF(OB_FETCH_INTERM_RESULT_ITEM, 0x517)
PCODE_DEF(OB_PX_ASYNC_INIT_SQC, 0x518)  // ob px async init sqc
PCODE_DEF(OB_LOAD_DATA_SHUFFLE, 0x519)
PCODE_DEF(OB_LOAD_DATA_INSERT, 0x51A)
PCODE_DEF(OB_AP_PING_SQL_TASK, 0x51B)
PCODE_DEF(OB_REMOTE_SYNC_EXECUTE, 0x51C)   // remote sync execute with sql
PCODE_DEF(OB_REMOTE_ASYNC_EXECUTE, 0x51D)  // remote async execute with sql
PCODE_DEF(OB_REMOTE_POST_RESULT, 0x51E)    // remote execute response result async
PCODE_DEF(OB_PX_FAST_INIT_SQC, 0x51F)
PCODE_DEF(OB_CHECK_BUILD_INDEX_TASK_EXIST, 0x520)  // check build index task exist
PCODE_DEF(OB_SQL_PCODE_END, 0x54F)                 // as a guardian

// for test schema
PCODE_DEF(OB_GET_LATEST_SCHEMA_VERSION, 0x550)
PCODE_DEF(OB_GET_ALL_SCHEMA, 0x551)

PCODE_DEF(OB_SERVER_UPDATE_STAT_CACHE, 0x560)
PCODE_DEF(OB_RS_UPDATE_STAT_CACHE, 0x561)

//// DTL
//
// Channel operations
PCODE_DEF(OB_DTL_CREATE_CHANNEL, 0x5A0)
PCODE_DEF(OB_DTL_DESTROY_CHANNEL, 0x5A1)
// Task operations
PCODE_DEF(OB_DTL_START_TASK, 0x5B0)
PCODE_DEF(OB_DTL_START_TASKS, 0x5B1)
PCODE_DEF(OB_DTL_CANCEL_TASK, 0x5B2)
// Data transfer
PCODE_DEF(OB_DTL_SEND, 0x5C0)
PCODE_DEF(OB_DTL_FETCH, 0x5C1)
PCODE_DEF(OB_DTL_BC_SEND, 0x5C2)

// election
PCODE_DEF(OB_ELECTION, 0x601)
PCODE_DEF(OB_ELECTION_RESPONSE, 0x602)

// transaction
PCODE_DEF(OB_TRANS, 0x701)
PCODE_DEF(OB_DUMP_MEMTABLE, 0x702)
PCODE_DEF(OB_TRANS_CHECK, 0x703)
PCODE_DEF(OB_TRANS_RESP, 0x706)
PCODE_DEF(OB_DUP_TABLE_LEASE_REQUEST, 0x707)
// transaction check for change leader by rpc
PCODE_DEF(OB_CHANGE_LEADER, 0x708)
PCODE_DEF(OB_GET_GTS_REQUEST, 0x710)
PCODE_DEF(OB_GET_GTS_RESPONSE, 0x711)
PCODE_DEF(OB_GET_GTS_ERR_RESPONSE, 0x712)
PCODE_DEF(OB_REDO_LOG_SYNC_REQUEST, 0x713)
PCODE_DEF(OB_REDO_LOG_SYNC_RESPONSE, 0x714)
PCODE_DEF(OB_DUP_TABLE_LEASE_RESPONSE, 0x715)
PCODE_DEF(OB_WRS_GET_CLUSTER_VERSION, 0x716)
PCODE_DEF(OB_WRS_CLUSTER_HEARTBEAT, 0x717)
PCODE_DEF(OB_HA_GTS_PING_REQUEST, 0x718)
PCODE_DEF(OB_HA_GTS_GET_REQUEST, 0x719)
PCODE_DEF(OB_HA_GTS_GET_RESPONSE, 0x71A)
PCODE_DEF(OB_HA_GTS_HEARTBEAT, 0x71B)
PCODE_DEF(OB_HA_GTS_UPDATE_META, 0x71C)
PCODE_DEF(OB_HA_GTS_CHANGE_MEMBER, 0x71D)

// sys admin
PCODE_DEF(OB_FORCE_SET_AS_SINGLE_REPLICA, 0x720)
PCODE_DEF(OB_FLUSH_CACHE, 0x721)
PCODE_DEF(OB_FORCE_PURGE_MEMTABLE, 0x722)
PCODE_DEF(OB_FORCE_PURGE_MEMTABLE_ASYNC, 0x723)
PCODE_DEF(OB_FORCE_RESET_PARENT, 0x724)
PCODE_DEF(OB_FORCE_SET_PARENT, 0x725)
PCODE_DEF(OB_FORCE_SET_REPLICA_NUM, 0x726)
PCODE_DEF(OB_STORAGE_HANDLE, 0x727)
PCODE_DEF(OB_STORAGE_SYNC_HANDLE, 0x728)
PCODE_DEF(OB_FORCE_SWITCH_ILOG_FILE, 0x729)
PCODE_DEF(OB_FORCE_SET_ALL_AS_SINGLE_REPLICA, 0x72A)
PCODE_DEF(OB_FORCE_REMOVE_REPLICA, 0x72B)
PCODE_DEF(OB_FORCE_CREATE_SYS_TABLE, 0x72C)
PCODE_DEF(OB_FORCE_DISABLE_BLACKLIST, 0x72D)
PCODE_DEF(OB_FORCE_ENABLE_BLACKLIST, 0x72E)
PCODE_DEF(OB_FORCE_CLEAR_BLACKLIST, 0x72F)
PCODE_DEF(OB_UPDATE_TENANT_MEMORY, 0x730)
PCODE_DEF(OB_FORCE_SET_SERVER_LIST, 0x731)
PCODE_DEF(OB_FORCE_ALTER_TABLE_SUBPARTITION, 0x732)
PCODE_DEF(OB_FORCE_SET_LOCALITY, 0x733)
PCODE_DEF(OB_RENEW_IN_ZONE_HB, 0x738)
PCODE_DEF(OB_OBS_DISCONNECT_CLUSTER, 0x73B)

// BatchRpc
PCODE_DEF(OB_BATCH, 0x750)
// Server black list
PCODE_DEF(OB_SERVER_BLACKLIST_REQ, 0x751)
PCODE_DEF(OB_SERVER_BLACKLIST_RESP, 0x752)

// xa trans
PCODE_DEF(OB_XA_PREPARE, 0x760)
PCODE_DEF(OB_XA_END_TRANS, 0x761)
PCODE_DEF(OB_XA_SYNC_STATUS, 0x762)
PCODE_DEF(OB_XA_SYNC_STATUS_RESPONSE, 0x763)
PCODE_DEF(OB_XA_MERGE_STATUS, 0x764)
PCODE_DEF(OB_XA_HB_REQ, 0x765)
PCODE_DEF(OB_XA_HB_RESP, 0x766)

// clog
PCODE_DEF(OB_CLOG, 0x801)

PCODE_DEF(OB_LOG_GET_MC_TS, 0x805)
PCODE_DEF(OB_LOG_GET_FOLLOWER_SYNC_INFO, 0x806)
PCODE_DEF(OB_LOG_GET_MC_CTX_ARRAY, 0x807)
PCODE_DEF(OB_LOG_GET_PRIORITY_ARRAY, 0x808)
PCODE_DEF(OB_LOG_GET_REMOTE_LOG, 0x809)
PCODE_DEF(OB_LOG_GET_NEXT_LOG_TS, 0x80A)
PCODE_DEF(OB_CHECK_ALL_PART_LOG_RES, 0x80B)
PCODE_DEF(OB_QUERY_MAX_FLUSHED_ILOG_ID, 0x80C)

/* New Interface. To Replace Dying Rpc. */
PCODE_DEF(OB_LOG_REQ_START_LOG_ID_BY_TS, 0x850)
PCODE_DEF(OB_LOG_REQ_START_POS_BY_LOG_ID, 0x851)
PCODE_DEF(OB_LOG_FETCH_LOG_EXTERNAL, 0x852)
PCODE_DEF(OB_LOG_REQUEST_HEARTBEAT_INFO, 0x853)
PCODE_DEF(OB_LOG_REQ_START_LOG_ID_BY_TS_WITH_BREAKPOINT, 0x854)
PCODE_DEF(OB_LOG_REQ_START_POS_BY_LOG_ID_WITH_BREAKPOINT, 0x855)
PCODE_DEF(OB_LOG_OPEN_STREAM, 0x856)
PCODE_DEF(OB_LOG_STREAM_FETCH_LOG, 0x857)
PCODE_DEF(OB_LOG_LEADER_HEARTBEAT, 0x858)
PCODE_DEF(OB_KILL_PART_TRANS_CTX, 0x85B)

// partition service
PCODE_DEF(OB_PTS_FETCH_INFO, 0x901)
PCODE_DEF(OB_PTS_MEMBER_CHANGE, 0x902)
PCODE_DEF(OB_GET_LEADER_MEMBER_LIST, 0x903)
PCODE_DEF(OB_MINOR_FREEZE, 0x904)  // reused by ObService
PCODE_DEF(OB_PTS_ADD_REPLICA, 0x905)
PCODE_DEF(OB_PTS_REMOVE_REPLICA, 0x906)
PCODE_DEF(OB_GET_MEMBER_LIST_AND_LEADER, 0x907)
PCODE_DEF(OB_WARM_UP_REQUEST, 0x908)
PCODE_DEF(OB_IN_MEMBER_CHANGE_DONE, 0x909)
PCODE_DEF(OB_PTS_REMOVE_PARTITION, 0x90A)
PCODE_DEF(OB_GET_ELECTION_LEADER, 0x90B)
PCODE_DEF(OB_SPLIT_DEST_PARTITION_REQUEST, 0x90C)
PCODE_DEF(OB_REPLICA_SPLIT_PROGRESS_REQUEST, 0x90D)
PCODE_DEF(OB_CHECK_MEMBER_MAJOR_SSTABLE_ENOUGH, 0x90E)
PCODE_DEF(OB_BATCH_REMOVE_PARTITION, 0x90F)
PCODE_DEF(OB_BATCH_GET_MEMBER_LIST_AND_LEADER, 0x910)
PCODE_DEF(OB_BATCH_GET_ROLE, 0x911)
PCODE_DEF(OB_BATCH_GET_PROTECTION_LEVEL, 0x912)
PCODE_DEF(OB_UPDATE_STANDBY_CLUSTER_INFO, 0x913)
PCODE_DEF(OB_CHECK_NEED_OFFLINE_REPLICA, 0x914)
PCODE_DEF(OB_GET_MEMBER_LIST_AND_LEADER_V2, 0x915)
PCODE_DEF(OB_CHECK_FLASHBACK_INFO_DUMP, 0x916)
PCODE_DEF(OB_BROADCAST_LOCATIONS, 0x917)

PCODE_DEF(OB_RPC_ASSEMBLE, 0x1000)

// Table API (by )
PCODE_DEF(OB_TABLE_API_LOGIN, 0x1101)
PCODE_DEF(OB_TABLE_API_EXECUTE, 0x1102)
PCODE_DEF(OB_TABLE_API_BATCH_EXECUTE, 0x1103)
PCODE_DEF(OB_TABLE_API_EXECUTE_QUERY, 0x1104)
PCODE_DEF(OB_TABLE_API_QUERY_AND_MUTATE, 0x1105)
PCODE_DEF(OB_TABLE_API_EXECUTE_QUERY_SYNC, 0x1106)
PCODE_DEF(OB_TABLE_TTL, 0x1107)
PCODE_DEF(OB_TTL_REQUEST, 0x1108)   
PCODE_DEF(OB_TTL_RESPONSE, 0x1109)   

// Event Job API
PCODE_DEF(OB_RUN_EVENT_JOB, 0x1201)
PCODE_DEF(OB_CREATE_EVENT_JOB, 0x1202)
PCODE_DEF(OB_ALTER_EVENT_JOB, 0x1203)
PCODE_DEF(OB_DROP_EVENT_JOB, 0x1204)

// backup and restore
PCODE_DEF(OB_GET_TENANT_LOG_ARCHIVE_STATUS, 0x1401)
PCODE_DEF(OB_ARCHIVE_LOG, 0x1402)
PCODE_DEF(OB_BACKUP_DATABASE, 0x1403)
PCODE_DEF(OB_BACKUP_MANAGE, 0x1404)
PCODE_DEF(OB_PHYSICAL_RESTORE_TENANT, 0x1405)
PCODE_DEF(OB_REBUILD_INDEX_IN_RESTORE, 0x1406)
PCODE_DEF(OB_PHYSICAL_RESTORE_REPLICA, 0x1407)
PCODE_DEF(OB_PHYSICAL_RESTORE_REPLICA_RES, 0x1408)
PCODE_DEF(OB_MODIFY_SCHEMA_IN_RESTORE, 0x1409)
PCODE_DEF(OB_CHECK_BACKUP_SCHEDULER_WORKING, 0x140A)
PCODE_DEF(OB_PHYSICAL_RESTORE_RES, 0x140B)
PCODE_DEF(OB_VALIDATE_BACKUP_BATCH, 0x140C)
PCODE_DEF(OB_VALIDATE_BACKUP_RES, 0x140D)
PCODE_DEF(OB_VALIDATE_BACKUP_BATCH_RES, 0x140E)
PCODE_DEF(OB_BACKUP_BACKUPSET, 0x140F)
PCODE_DEF(OB_BACKUP_BACKUPSET_BATCH, 0x1410)
PCODE_DEF(OB_BACKUP_BACKUPSET_BATCH_RES, 0x1411)
PCODE_DEF(OB_BACKUP_ARCHIVE_LOG, 0x1412)
PCODE_DEF(OB_BACKUP_ARCHIVE_LOG_BATCH, 0x1413)
PCODE_DEF(OB_BACKUP_ARCHIVE_LOG_BATCH_RES, 0x1414)
PCODE_DEF(OB_STANDBY_CUTDATA_BATCH_TASK_RES, 0x1415)
PCODE_DEF(OB_BACKUP_BACKUPPIECE, 0x1416)
PCODE_DEF(OB_GET_TENANT_LOG_ARCHIVE_STATUS_V2, 0x1417)
