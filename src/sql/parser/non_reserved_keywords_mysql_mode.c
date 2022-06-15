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

#include <stdio.h>
#include "lib/alloc/alloc_assist.h"
#include "ob_non_reserved_keywords.h"
#include "sql/parser/sql_parser_mysql_mode_tab.h"

static t_node* mysql_none_reserved_keywords_root = NULL;
static t_node* mysql_sql_keywords_in_pl_root = NULL;

/* List of non-reserved keywords */
static const NonReservedKeyword Mysql_none_reserved_keywords[] = {
    {"access", ACCESS},
    {"account", ACCOUNT},
    {"action", ACTION},
    {"activate", ACTIVATE},
    {"active", ACTIVE},
    {"adddate", ADDDATE},
    {"after", AFTER},
    {"against", AGAINST},
    {"aggregate", AGGREGATE},
    {"algorithm", ALGORITHM},
    {"always", ALWAYS},
    {"analyse", ANALYSE},
    {"any", ANY},
    {"approx_count_distinct", APPROX_COUNT_DISTINCT},
    {"approx_count_distinct_synopsis", APPROX_COUNT_DISTINCT_SYNOPSIS},
    {"approx_count_distinct_synopsis_merge", APPROX_COUNT_DISTINCT_SYNOPSIS_MERGE},
    {"archivelog", ARCHIVELOG},
    {"ascii", ASCII},
    {"at", AT},
    {"authors", AUTHORS},
    {"auto", AUTO},
    {"auto_increment", AUTO_INCREMENT},
    {"autoextend_size", AUTOEXTEND_SIZE},
    {"availability", AVAILABILITY},
    {"avg", AVG},
    {"avg_row_length", AVG_ROW_LENGTH},
    {"backup", BACKUP},
    {"backupset", BACKUPSET},
    {"balance", BALANCE},
    {"base", BASE},
    {"baseline", BASELINE},
    {"baseline_id", BASELINE_ID},
    {"basic", BASIC},
    {"begin", BEGI},
    {"binlog", BINLOG},
    {"bit", BIT},
    {"block", BLOCK},
    {"block_size", BLOCK_SIZE},
    {"bool", BOOL},
    {"boolean", BOOLEAN},
    {"bootstrap", BOOTSTRAP},
    {"breadth", BREADTH},
    {"btree", BTREE},
    {"byte", BYTE},
    {"buckets", BUCKETS},
    {"cache", CACHE},
    {"cancel", CANCEL},
    {"cascaded", CASCADED},
    {"cast", CAST},
    {"catalog_name", CATALOG_NAME},
    {"chain", CHAIN},
    {"changed", CHANGED},
    {"charset", CHARSET},
    {"checksum", CHECKSUM},
    {"checkpoint", CHECKPOINT},
    {"chunk", CHUNK},
    {"cipher", CIPHER},
    {"class_origin", CLASS_ORIGIN},
    {"clean", CLEAN},
    {"clear", CLEAR},
    {"client", CLIENT},
    {"close", CLOSE},
    {"cluster", CLUSTER},
    {"cluster_id", CLUSTER_ID},
    {"coalesce", COALESCE},
    {"code", CODE},
    {"collation", COLLATION},
    {"column_format", COLUMN_FORMAT},
    {"column_name", COLUMN_NAME},
    {"columns", COLUMNS},
    {"comment", COMMENT},
    {"commit", COMMIT},
    {"committed", COMMITTED},
    {"compact", COMPACT},
    {"completion", COMPLETION},
    {"compressed", COMPRESSED},
    {"compression", COMPRESSION},
    {"concurrent", CONCURRENT},
    {"connection", CONNECTION},
    {"consistent", CONSISTENT},
    {"constraint_catalog", CONSTRAINT_CATALOG},
    {"constraint_name", CONSTRAINT_NAME},
    {"constraint_schema", CONSTRAINT_SCHEMA},
    {"contains", CONTAINS},
    {"context", CONTEXT},
    {"contributors", CONTRIBUTORS},
    {"copy", COPY},
    {"count", COUNT},
    {"cpu", CPU},
    {"create_timestamp", CREATE_TIMESTAMP},
    {"cube", CUBE},
    {"cume_dist", CUME_DIST},
    {"curdate", CURDATE},
    {"current", CURRENT},
    {"cursor_name", CURSOR_NAME},
    {"curtime", CURTIME},
    {"ctx_id", CTX_ID},
    {"cycle", CYCLE},
    {"dag", DAG},
    {"data", DATA},
    {"database_id", DATABASE_ID},
    {"datafile", DATAFILE},
    {"data_table_id", DATA_TABLE_ID},
    {"date", DATE},
    {"date_add", DATE_ADD},
    {"date_sub", DATE_SUB},
    {"datetime", DATETIME},
    {"day", DAY},
    {"deallocate", DEALLOCATE},
    {"decryption", DECRYPTION},
    {"default_auth", DEFAULT_AUTH},
    {"definer", DEFINER},
    {"delay", DELAY},
    {"delay_key_write", DELAY_KEY_WRITE},
    {"depth", DEPTH},
    {"des_key_file", DES_KEY_FILE},
    {"destination", DESTINATION},
    {"dense_rank", DENSE_RANK},
    {"diagnostics", DIAGNOSTICS},
    {"directory", DIRECTORY},
    {"disable", DISABLE},
    {"discard", DISCARD},
    {"disk", DISK},
    {"diskgroup", DISKGROUP},
    {"do", DO},
    {"dump", DUMP},
    {"dumpfile", DUMPFILE},
    {"duplicate", DUPLICATE},
    {"dynamic", DYNAMIC},
    {"default_tablegroup", DEFAULT_TABLEGROUP},
    {"effective", EFFECTIVE},
    {"empty", EMPTY},
    {"enable", ENABLE},
    {"encryption", ENCRYPTION},
    {"end", END},
    {"ends", ENDS},
    {"engine", ENGINE_},
    {"engines", ENGINES},
    {"enum", ENUM},
    {"entity", ENTITY},
    {"error", ERROR_P},
    {"error_code", ERROR_CODE},
    {"errors", ERRORS},
    {"escape", ESCAPE},
    {"event", EVENT},
    {"events", EVENTS},
    {"every", EVERY},
    {"except", EXCEPT},
    {"exchange", EXCHANGE},
    {"execute", EXECUTE},
    {"expansion", EXPANSION},
    {"expire", EXPIRE},
    {"expired", EXPIRED},
    {"expire_info", EXPIRE_INFO},
    {"export", EXPORT},
    {"extended", EXTENDED},
    {"extended_noaddr", EXTENDED_NOADDR},
    {"extent_size", EXTENT_SIZE},
    {"extract", EXTRACT},
    {"failover", FAILOVER},
    {"fast", FAST},
    {"faults", FAULTS},
    {"fields", FIELDS},
    {"file", FILEX},
    {"file_id", FILE_ID},
    {"final_count", FINAL_COUNT},
    {"first", FIRST},
    {"first_value", FIRST_VALUE},
    {"fixed", FIXED},
    {"flashback", FLASHBACK},
    {"flush", FLUSH},
    {"follower", FOLLOWER},
    {"format", FORMAT},
    {"found", FOUND},
    {"freeze", FREEZE},
    {"frequency", FREQUENCY},
    {"frozen", FROZEN},
    {"function", FUNCTION},
    {"full", FULL},
    {"following", FOLLOWING},
    {"general", GENERAL},
    {"geometry", GEOMETRY},
    {"geometrycollection", GEOMETRYCOLLECTION},
    {"get_format", GET_FORMAT},
    {"global", GLOBAL},
    {"global_name", GLOBAL_NAME},
    {"grants", GRANTS},
    {"grouping", GROUPING},
    {"group_concat", GROUP_CONCAT},
    {"gts", GTS},
    {"handler", HANDLER},
    {"hash", HASH},
    {"help", HELP},
    {"histogram", HISTOGRAM},
    {"host", HOST},
    {"hosts", HOSTS},
    {"hour", HOUR},
    {"id", ID},
    {"idc", IDC},
    {"identified", IDENTIFIED},
    {"ignore", IGNORE},
    {"ignore_server_ids", IGNORE_SERVER_IDS},
    {"import", IMPORT},
    {"incr", INCR},
    {"incremental", INCREMENTAL},
    {"indexes", INDEXES},
    {"index_table_id", INDEX_TABLE_ID},
    {"info", INFO},
    {"initial_size", INITIAL_SIZE},
    {"innodb", INNODB},
    {"insert_method", INSERT_METHOD},
    {"install", INSTALL},
    {"instance", INSTANCE},
    {"intersect", INTERSECT},
    {"invisible", INVISIBLE},
    {"invoker", INVOKER},
    {"io", IO},
    {"io_thread", IO_THREAD},
    {"ipc", IPC},
    {"isnull", ISNULL},
    {"isolate", ISOLATE},
    {"isolation", ISOLATION},
    {"issuer", ISSUER},
    {"job", JOB},
    {"json", JSON},
    {"json_arrayagg", JSON_ARRAYAGG},
    {"json_objectagg", JSON_OBJECTAGG},
    {"json_value", JSON_VALUE},
    {"key_block_size", KEY_BLOCK_SIZE},
    {"key_version", KEY_VERSION},
    {"kvcache", KVCACHE},
    {"ilogcache", ILOGCACHE},
    {"file_id", FILE_ID},
    {"lag", LAG},
    {"language", LANGUAGE},
    {"last", LAST},
    {"last_value", LAST_VALUE},
    {"lead", LEAD},
    {"leader", LEADER},
    {"leaves", LEAVES},
    {"leak", LEAK},
    {"leak_mod", LEAK_MOD},
    {"less", LESS},
    {"level", LEVEL},
    {"linestring", LINESTRING},
    {"list", LIST_},
    {"listagg", LISTAGG},
    {"ln", LN},
    {"local", LOCAL},
    {"locality", LOCALITY},
    {"location", LOCATION},
    {"locked", LOCKED},
    {"locks", LOCKS},
    {"logfile", LOGFILE},
    {"logonly_replica_num", LOGONLY_REPLICA_NUM},
    {"log", LOG},
    {"logs", LOGS},
    {"major", MAJOR},
    {"manual", MANUAL},
    {"master", MASTER},
    {"master_auto_position", MASTER_AUTO_POSITION},
    {"master_connect_retry", MASTER_CONNECT_RETRY},
    {"master_delay", MASTER_DELAY},
    {"master_heartbeat_period", MASTER_HEARTBEAT_PERIOD},
    {"master_host", MASTER_HOST},
    {"master_log_file", MASTER_LOG_FILE},
    {"master_log_pos", MASTER_LOG_POS},
    {"master_password", MASTER_PASSWORD},
    {"master_port", MASTER_PORT},
    {"master_retry_count", MASTER_RETRY_COUNT},
    {"master_server_id", MASTER_SERVER_ID},
    {"master_ssl", MASTER_SSL},
    {"master_ssl_ca", MASTER_SSL_CA},
    {"master_ssl_capath", MASTER_SSL_CAPATH},
    {"master_ssl_cert", MASTER_SSL_CERT},
    {"master_ssl_cipher", MASTER_SSL_CIPHER},
    {"master_ssl_crl", MASTER_SSL_CRL},
    {"master_ssl_crlpath", MASTER_SSL_CRLPATH},
    {"master_ssl_key", MASTER_SSL_KEY},
    {"master_user", MASTER_USER},
    {"max", MAX},
    {"maximize", MAXIMIZE},
    {"max_connections_per_hour", MAX_CONNECTIONS_PER_HOUR},
    {"max_cpu", MAX_CPU},
    {"max_disk_size", MAX_DISK_SIZE},
    {"max_iops", MAX_IOPS},
    {"max_memory", MAX_MEMORY},
    {"max_queries_per_hour", MAX_QUERIES_PER_HOUR},
    {"max_rows", MAX_ROWS},
    {"max_session_num", MAX_SESSION_NUM},
    {"max_size", MAX_SIZE},
    {"max_updates_per_hour", MAX_UPDATES_PER_HOUR},
    {"max_used_part_id", MAX_USED_PART_ID},
    {"max_user_connections", MAX_USER_CONNECTIONS},
    {"medium", MEDIUM},
    {"member", MEMBER},
    {"memory", MEMORY},
    {"memstore_percent", MEMSTORE_PERCENT},
    {"memtable", MEMTABLE},
    {"merge", MERGE},
    {"message_text", MESSAGE_TEXT},
    {"meta", META},
    {"microsecond", MICROSECOND},
    {"migrate", MIGRATE},
    {"migration", MIGRATION},
    {"min", MIN},
    {"min_cpu", MIN_CPU},
    {"min_iops", MIN_IOPS},
    {"min_memory", MIN_MEMORY},
    {"min_rows", MIN_ROWS},
    {"minor", MINOR},
    {"minute", MINUTE},
    {"minus", MINUS},
    {"mode", MODE},
    {"modify", MODIFY},
    {"month", MONTH},
    {"move", MOVE},
    {"multilinestring", MULTILINESTRING},
    {"multipoint", MULTIPOINT},
    {"multipolygon", MULTIPOLYGON},
    {"mutex", MUTEX},
    {"mysql_errno", MYSQL_ERRNO},
    {"name", NAME},
    {"names", NAMES},
    {"national", NATIONAL},
    {"nchar", NCHAR},
    {"ndb", NDB},
    {"ndbcluster", NDBCLUSTER},
    {"new", NEW},
    {"next", NEXT},
    {"no", NO},
    {"noarchivelog", NOARCHIVELOG},
    {"noaudit", NOAUDIT},
    {"no_parallel", NO_PARALLEL},
    {"no_rewrite", NO_REWRITE},
    {"no_wait", NO_WAIT},
    {"nodegroup", NODEGROUP},
    {"none", NONE},
    {"noparallel", NOPARALLEL},
    {"normal", NORMAL},
    {"now", NOW},
    {"nowait", NOWAIT},
    {"nulls", NULLS},
    {"number", NUMBER},
    {"nvarchar", NVARCHAR},
    {"occur", OCCUR},
    {"ntile", NTILE},
    {"nth_value", NTH_VALUE},
    {"of", OF},
    {"off", OFF},
    {"offset", OFFSET},
    {"old_key", OLD_KEY},
    {"over", OVER},
    {"old_password", OLD_PASSWORD},
    {"one", ONE},
    {"one_shot", ONE_SHOT},
    {"only", ONLY},
    {"open", OPEN},
    {"options", OPTIONS},
    {"orig_default", ORIG_DEFAULT},
    {"outline", OUTLINE},
    {"owner", OWNER},
    {"pack_keys", PACK_KEYS},
    {"page", PAGE},
    {"parameters", PARAMETERS},
    {"parallel", PARALLEL},
    {"parser", PARSER},
    {"partial", PARTIAL},
    {"partition_id", PARTITION_ID},
    {"partitioning", PARTITIONING},
    {"partitions", PARTITIONS},
    {"password", PASSWORD},
    {"pause", PAUSE},
    {"percent_rank", PERCENT_RANK},
    {"performance", PERFORMANCE},
    {"phase", PHASE},
    {"physical", PHYSICAL},
    {"plan", PLAN},
    {"planregress", PLANREGRESS},
    {"plugin", PLUGIN},
    {"plugin_dir", PLUGIN_DIR},
    {"plugins", PLUGINS},
    {"point", POINT},
    {"polygon", POLYGON},
    {"pool", POOL},
    {"port", PORT},
    {"position", POSITION},
    {"prepare", PREPARE},
    {"preserve", PRESERVE},
    {"prev", PREV},
    {"primary_cluster_id", PRIMARY_CLUSTER_ID},
    {"primary_rootservice_list", PRIMARY_ROOTSERVICE_LIST},
    {"primary_zone", PRIMARY_ZONE},
    {"privileges", PRIVILEGES},
    {"process", PROCESS},
    {"processlist", PROCESSLIST},
    {"profile", PROFILE},
    {"profiles", PROFILES},
    {"progressive_merge_num", PROGRESSIVE_MERGE_NUM},
    {"protection", PROTECTION},
    {"proxy", PROXY},
    {"public", PUBLIC},
    {"p_entity", P_ENTITY},
    {"p_chunk", P_CHUNK},
    {"preceding", PRECEDING},
    {"pctfree", PCTFREE},
    {"ps", PS},
    {"quarter", QUARTER},
    {"query", QUERY},
    {"quick", QUICK},
    {"rank", RANK},
    {"read_consistency", READ_CONSISTENCY},
    {"read_only", READ_ONLY},
    {"rebuild", REBUILD},
    {"recover", RECOVER},
    {"recovery", RECOVERY},
    {"recycle", RECYCLE},
    {"recyclebin", RECYCLEBIN},
    {"redo_buffer_size", REDO_BUFFER_SIZE},
    {"redo_transport_options", REDO_TRANSPORT_OPTIONS},
    {"redofile", REDOFILE},
    {"redundant", REDUNDANT},
    {"refresh", REFRESH},
    {"region", REGION},
    {"relay", RELAY},
    {"relay_log_file", RELAY_LOG_FILE},
    {"relay_log_pos", RELAY_LOG_POS},
    {"relay_thread", RELAY_THREAD},
    {"relaylog", RELAYLOG},
    {"reload", RELOAD},
    {"remove", REMOVE},
    {"reorganize", REORGANIZE},
    {"repair", REPAIR},
    {"repeatable", REPEATABLE},
    {"replica", REPLICA},
    {"replica_num", REPLICA_NUM},
    {"replica_type", REPLICA_TYPE},
    {"duplicate_scope", DUPLICATE_SCOPE},
    {"replication", REPLICATION},
    {"report", REPORT},
    {"reset", RESET},
    {"resource", RESOURCE},
    {"resource_pool_list", RESOURCE_POOL_LIST},
    {"respect", RESPECT},
    {"restart", RESTART},
    {"restore", RESTORE},
    {"resume", RESUME},
    {"returned_sqlstate", RETURNED_SQLSTATE},
    {"returning", RETURNING},
    {"returns", RETURNS},
    {"reverse", REVERSE},
    {"rewrite_merge_version", REWRITE_MERGE_VERSION},
    {"rollback", ROLLBACK},
    {"rolling", ROLLING},
    {"rollup", ROLLUP},
    {"root", ROOT},
    {"rootservice", ROOTSERVICE},
    {"rootservice_list", ROOTSERVICE_LIST},
    {"roottable", ROOTTABLE},
    {"rotate", ROTATE},
    {"routine", ROUTINE},
    {"row", ROW},
    {"row_count", ROW_COUNT},
    {"row_format", ROW_FORMAT},
    {"row_number", ROW_NUMBER},
    {"rows", ROWS},
    {"rtree", RTREE},
    {"run", RUN},
    {"sample", SAMPLE},
    {"savepoint", SAVEPOINT},
    {"schedule", SCHEDULE},
    {"schema_name", SCHEMA_NAME},
    {"scope", SCOPE},
    {"second", SECOND},
    {"security", SECURITY},
    {"seed", SEED},
    {"serial", SERIAL},
    {"serializable", SERIALIZABLE},
    {"server", SERVER},
    {"server_ip", SERVER_IP},
    {"server_port", SERVER_PORT},
    {"server_type", SERVER_TYPE},
    {"session", SESSION},
    {"session_user", SESSION_USER},
    {"statements", STATEMENTS},
    {"binding", BINDING},
    {"set_master_cluster", SET_MASTER_CLUSTER},
    {"set_slave_cluster", SET_SLAVE_CLUSTER},
    {"set_tp", SET_TP},
    {"share", SHARE},
    {"shutdown", SHUTDOWN},
    {"signed", SIGNED},
    {"simple", SIMPLE},
    {"slave", SLAVE},
    {"size", SIZE},
    {"slow", SLOW},
    {"slot_idx", SLOT_IDX},
    {"snapshot", SNAPSHOT},
    {"socket", SOCKET},
    {"some", SOME},
    {"soname", SONAME},
    {"sounds", SOUNDS},
    {"source", SOURCE},
    {"spfile", SPFILE},
    {"split", SPLIT},
    {"sql_after_gtids", SQL_AFTER_GTIDS},
    {"sql_after_mts_gaps", SQL_AFTER_MTS_GAPS},
    {"sql_before_gtids", SQL_BEFORE_GTIDS},
    {"sql_buffer_result", SQL_BUFFER_RESULT},
    {"sql_cache", SQL_CACHE},
    {"sql_id", SQL_ID},
    {"sql_no_cache", SQL_NO_CACHE},
    {"sql_thread", SQL_THREAD},
    {"sql_tsi_day", SQL_TSI_DAY},
    {"sql_tsi_hour", SQL_TSI_HOUR},
    {"sql_tsi_minute", SQL_TSI_MINUTE},
    {"sql_tsi_month", SQL_TSI_MONTH},
    {"sql_tsi_quarter", SQL_TSI_QUARTER},
    {"sql_tsi_second", SQL_TSI_SECOND},
    {"sql_tsi_week", SQL_TSI_WEEK},
    {"sql_tsi_year", SQL_TSI_YEAR},
    {"standby", STANDBY},
    {"start", START},
    {"starts", STARTS},
    {"stat", STAT},
    {"stats_auto_recalc", STATS_AUTO_RECALC},
    {"stats_persistent", STATS_PERSISTENT},
    {"stats_sample_pages", STATS_SAMPLE_PAGES},
    {"status", STATUS},
    {"std", STD},
    {"stddev", STDDEV},
    {"stddev_pop", STDDEV_POP},
    {"stddev_samp", STDDEV_SAMP},
    {"stop", STOP},
    {"storage", STORAGE},
    {"storage_format_version", STORAGE_FORMAT_VERSION},
    {"storage_format_work_version", STORAGE_FORMAT_WORK_VERSION},
    {"storing", STORING},
    {"string", STRING},
    {"strong", STRONG},
    {"subclass_origin", SUBCLASS_ORIGIN},
    {"subdate", SUBDATE},
    {"subject", SUBJECT},
    {"subpartition", SUBPARTITION},
    {"subpartitions", SUBPARTITIONS},
    {"substr", SUBSTR},
    {"substring", SUBSTRING},
    {"sum", SUM},
    {"super", SUPER},
    {"suspend", SUSPEND},
    {"successful", SUCCESSFUL},
    {"synchronization", SYNCHRONIZATION},
    {"sysdate", SYSDATE},
    {"system", SYSTEM},
    {"system_user", SYSTEM_USER},
    {"swaps", SWAPS},
    {"switch", SWITCH},
    {"switches", SWITCHES},
    {"switchover", SWITCHOVER},
    {"table_checksum", TABLE_CHECKSUM},
    {"table_mode", TABLE_MODE},
    {"table_id", TABLE_ID},
    {"table_name", TABLE_NAME},
    {"tablegroup", TABLEGROUP},
    {"tablegroups", TABLEGROUPS},
    {"tablegroup_id", TABLEGROUP_ID},
    {"tables", TABLES},
    {"tablespace", TABLESPACE},
    {"tablet", TABLET},
    {"tablet_max_size", TABLET_MAX_SIZE},
    {"task", TASK},
    {"template", TEMPLATE},
    {"temporary", TEMPORARY},
    {"temptable", TEMPTABLE},
    {"tenant", TENANT},
    {"tenant_id", TENANT_ID},
    {"text", TEXT},
    {"than", THAN},
    {"time", TIME},
    {"timestamp", TIMESTAMP},
    {"timestampadd", TIMESTAMPADD},
    {"timestampdiff", TIMESTAMPDIFF},
    {"tablet_size", TABLET_SIZE},
    {"tp_name", TP_NAME},
    {"tp_no", TP_NO},
    {"trace", TRACE},
    {"traditional", TRADITIONAL},
    {"transaction", TRANSACTION},
    {"trigger", TRIGGER},
    {"triggers", TRIGGERS},
    {"trim", TRIM},
    {"truncate", TRUNCATE},
    {"type", TYPE},
    {"types", TYPES},
    {"uncommitted", UNCOMMITTED},
    {"undefined", UNDEFINED},
    {"undo_buffer_size", UNDO_BUFFER_SIZE},
    {"undofile", UNDOFILE},
    {"unicode", UNICODE},
    {"uninstall", UNINSTALL},
    {"unit", UNIT},
    {"unit_num", UNIT_NUM},
    {"unknown", UNKNOWN},
    {"unlocked", UNLOCKED},
    {"until", UNTIL},
    {"unusual", UNUSUAL},
    {"upgrade", UPGRADE},
    {"use_bloom_filter", USE_BLOOM_FILTER},
    {"use_frm", USE_FRM},
    {"user", USER},
    {"user_resources", USER_RESOURCES},
    {"unbounded", UNBOUNDED},
    {"valid", VALID},
    {"validate", VALIDATE},
    {"value", VALUE},
    {"variance", VARIANCE},
    {"variables", VARIABLES},
    {"var_pop", VAR_POP},
    {"var_samp", VAR_SAMP},
    {"verbose", VERBOSE},
    {"verify", VERIFY},
    {"materialized", MATERIALIZED},
    {"validate", VALIDATE},
    {"view", VIEW},
    {"virtual_column_id", VIRTUAL_COLUMN_ID},
    {"visible", VISIBLE},
    {"wait", WAIT},
    {"warnings", WARNINGS},
    {"weak", WEAK},
    {"week", WEEK},
    {"weight_string", WEIGHT_STRING},
    {"whenever", WHENEVER},
    {"window", WINDOW},
    {"work", WORK},
    {"wrapper", WRAPPER},
    {"x509", X509},
    {"xa", XA},
    {"xml", XML},
    {"year", YEAR},
    {"zone", ZONE},
    {"zone_list", ZONE_LIST},
    {"time_zone_info", TIME_ZONE_INFO},
    {"zone_type", ZONE_TYPE},
    {"audit", AUDIT},
    {"PL", PL},
    {"remote_oss", REMOTE_OSS},
    {"throttle", THROTTLE},
    {"priority", PRIORITY},
    {"rt", RT},
    {"network", NETWORK},
    {"logical_reads", LOGICAL_READS},
    {"queue_time", QUEUE_TIME},
    {"obsolete", OBSOLETE},
    {"backuppiece", BACKUPPIECE},
    {"backup_backup_dest", BACKUP_BACKUP_DEST},
    {"backed", BACKED},
    {"preview", PREVIEW},
    {"up", UP},
    {"times", TIMES},
    {"backupround", BACKUPROUND},
    {"TTL", TTL}};

/** https://dev.mysql.com/doc/refman/5.7/en/sql-syntax-prepared-statements.html
 ALTER TABLE
 ALTER USER
 ANALYZE TABLE
 CACHE INDEX
 CALL
 CHANGE MASTER
 CHECKSUM {TABLE | TABLES}
 COMMIT
 {CREATE | DROP} INDEX
 {CREATE | RENAME | DROP} DATABASE
 {CREATE | DROP} TABLE
 {CREATE | RENAME | DROP} USER
 {CREATE | DROP} VIEW
 DELETE
 DO
 FLUSH {TABLE | TABLES | TABLES WITH READ LOCK | HOSTS | PRIVILEGES
   | LOGS | STATUS | MASTER | SLAVE | DES_KEY_FILE | USER_RESOURCES}
 GRANT
 INSERT
 INSTALL PLUGIN
 KILL
 \LOAD INDEX INTO CACHE
 OPTIMIZE TABLE
 RENAME TABLE
 REPAIR TABLE
 REPLACE
 RESET {MASTER | SLAVE | QUERY CACHE}
 REVOKE
 SELECT
 SET
 SHOW {WARNINGS | ERRORS}
 SHOW BINLOG EVENTS
 SHOW CREATE {PROCEDURE | FUNCTION | EVENT | TABLE | VIEW}
 SHOW {MASTER | BINARY} LOGS
 SHOW {MASTER | SLAVE} STATUS
 SLAVE {START | STOP}
 TRUNCATE TABLE
 UNINSTALL PLUGIN
 UPDATE
 */
static const NonReservedKeyword Mysql_sql_keywords_in_pl[] = {
    {"alter", ALTER},
    {"analyze", ANALYZE},
    {"binary", BINARY},
    {"binlog", BINLOG},
    {"cache", CACHE},
    {"call", CALL},
    {"change", CHANGE},
    {"checksum", CHECKSUM},
    {"commit", COMMIT},
    {"create", CREATE},
    {"database", DATABASE},
    {"delete", DELETE},
    {"des_key_file", DES_KEY_FILE},
    {"do", DO},
    {"drop", DROP},
    {"errors", ERRORS},
    {"event", EVENT},
    {"events", EVENTS},
    {"flush", FLUSH},
    {"function", FUNCTION},
    {"grant", GRANT},
    {"hosts", HOSTS},
    {"index", INDEX},
    {"insert", INSERT},
    {"install", INSTALL},
    {"into", INTO},
    {"kill", KILL},
    {"load", LOAD},
    {"lock", LOCK_},
    {"logs", LOGS},
    {"master", MASTER},
    {"optimize", OPTIMIZE},
    {"plugin", PLUGIN},
    {"privileges", PRIVILEGES},
    {"procedure", PROCEDURE},
    {"query", QUERY},
    {"read", READ},
    {"rename", RENAME},
    {"repair", REPAIR},
    {"replace", REPLACE},
    {"reset", RESET},
    {"revoke", REVOKE},
    {"select", SELECT},
    {"set", SET},
    {"show", SHOW},
    {"slave", SLAVE},
    {"start", START},
    {"status", STATUS},
    {"stop", STOP},
    {"table", TABLE},
    {"tables", TABLES},
    {"truncate", TRUNCATE},
    {"uninstall", UNINSTALL},
    {"update", UPDATE},
    {"user", USER},
    {"user_resources", USER_RESOURCES},
    {"view", VIEW},
    {"warnings", WARNINGS},
    {"with", WITH},
    {"prepare", PREPARE},
    {"execute", EXECUTE},
    {"deallocate", DEALLOCATE},
};

const NonReservedKeyword* mysql_non_reserved_keyword_lookup(const char* word)
{
  return find_word(word, mysql_none_reserved_keywords_root, Mysql_none_reserved_keywords);
}

int mysql_sql_reserved_keyword_lookup(const char* word)
{
  int ret = -1;
  const NonReservedKeyword* res_word = find_word(word, mysql_sql_keywords_in_pl_root, Mysql_sql_keywords_in_pl);
  if (NULL != res_word) {
    ret = res_word->keyword_type;
  }
  return ret;
}

// return 0 if succ, return 1 if fail
int create_mysql_trie_tree()
{
  int ret = 0;
  if (0 != (ret = create_trie_tree(Mysql_none_reserved_keywords,
                LENGTH_OF(Mysql_none_reserved_keywords),
                &mysql_none_reserved_keywords_root))) {
    (void)printf("ERROR create trie tree failed! \n");
  } else if (0 !=
             (ret = create_trie_tree(
                  Mysql_sql_keywords_in_pl, LENGTH_OF(Mysql_sql_keywords_in_pl), &mysql_sql_keywords_in_pl_root))) {
    (void)printf("ERROR create trie tree failed! \n");
  } else { /*do nothing*/
  }
  return ret;
}

void __attribute__((constructor)) init_mysql_non_reserved_keywords_tree()
{
  int ret = 0;
  if (0 != (ret = create_mysql_trie_tree())) {
    (void)printf("ERROR build mysql_non_reserved_keywords tree failed=>%d", ret);
  }
}
