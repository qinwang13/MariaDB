# Copyright (c) 2006, 2011, Oracle and/or its affiliates.
# Copyright (c) 2009, 2018, MariaDB Corporation
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

ADD_DEFINITIONS(-DMYSQL_SERVER -DEMBEDDED_LIBRARY
 ${SSL_DEFINES})

INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}/include 
${CMAKE_SOURCE_DIR}/libmysql
${CMAKE_SOURCE_DIR}/libmysqld
${CMAKE_SOURCE_DIR}/sql
${CMAKE_BINARY_DIR}/sql 
${CMAKE_SOURCE_DIR}/regex
${ZLIB_INCLUDE_DIR}
${SSL_INCLUDE_DIRS}
${SSL_INTERNAL_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/sql/backup
)

SET(GEN_SOURCES
${CMAKE_BINARY_DIR}/sql/sql_yacc.h 
${CMAKE_BINARY_DIR}/sql/sql_yacc.cc
${CMAKE_BINARY_DIR}/sql/lex_hash.h 
)

SET_SOURCE_FILES_PROPERTIES(${GEN_SOURCES} PROPERTIES GENERATED TRUE)

SET(SQL_EMBEDDED_SOURCES emb_qcache.cc libmysqld.c lib_sql.cc 
           ../libmysql/libmysql.c ../libmysql/errmsg.c ../client/get_password.c
           ../sql-common/client.c ../sql-common/my_time.c 
           ../sql-common/my_user.c ../sql-common/pack.c
           ../sql-common/client_plugin.c ../sql-common/mysql_async.c
           ../sql/password.c ../sql/discover.cc ../sql/derror.cc 
           ../sql/field.cc ../sql/field_conv.cc
           ../sql/filesort.cc ../sql/gstream.cc ../sql/slave.cc
           ../sql/signal_handler.cc
           ../sql/handler.cc ../sql/hash_filo.cc ../sql/hostname.cc 
           ../sql/init.cc ../sql/item_buff.cc ../sql/item_cmpfunc.cc 
           ../sql/item.cc ../sql/item_create.cc ../sql/item_func.cc 
           ../sql/item_geofunc.cc ../sql/item_row.cc ../sql/item_strfunc.cc 
           ../sql/item_subselect.cc ../sql/item_sum.cc ../sql/item_timefunc.cc 
           ../sql/item_xmlfunc.cc ../sql/key.cc ../sql/lock.cc ../sql/log.cc 
           ../sql/log_event.cc ../sql/mf_iocache.cc ../sql/my_decimal.cc 
           ../sql/net_serv.cc ../sql/opt_range.cc ../sql/opt_sum.cc 
           ../sql/parse_file.cc ../sql/procedure.cc ../sql/protocol.cc 
           ../sql/records.cc ../sql/repl_failsafe.cc ../sql/rpl_filter.cc
           ../sql/rpl_record.cc ../sql/sha2.cc ../sql/des_key_file.cc
           ../sql/rpl_injector.cc ../sql/set_var.cc ../sql/spatial.cc 
           ../sql/sp_cache.cc ../sql/sp.cc ../sql/sp_head.cc 
           ../sql/sp_pcontext.cc ../sql/sp_rcontext.cc ../sql/sql_acl.cc 
           ../sql/sql_analyse.cc ../sql/sql_base.cc ../sql/sql_cache.cc 
           ../sql/sql_class.cc ../sql/sql_crypt.cc ../sql/sql_cursor.cc 
           ../sql/sql_db.cc ../sql/sql_delete.cc ../sql/sql_derived.cc 
           ../sql/sql_do.cc ../sql/sql_error.cc ../sql/sql_handler.cc 
           ../sql/sql_help.cc ../sql/sql_insert.cc ../sql/datadict.cc
           ../sql/sql_admin.cc ../sql/sql_truncate.cc ../sql/sql_reload.cc
           ../sql/sql_lex.cc ../sql/keycaches.cc
           ../sql/sql_list.cc ../sql/sql_load.cc ../sql/sql_locale.cc 
           ../sql/sql_binlog.cc ../sql/sql_manager.cc
           ../sql/sql_parse.cc ../sql/sql_partition.cc ../sql/sql_plugin.cc 
           ../sql/debug_sync.cc ../sql/opt_table_elimination.cc
           ../sql/sql_prepare.cc ../sql/sql_rename.cc ../sql/sql_repl.cc 
           ../sql/sql_select.cc ../sql/sql_servers.cc
           ../sql/sql_show.cc ../sql/sql_state.c ../sql/sql_string.cc
           ../sql/sql_tablespace.cc ../sql/sql_table.cc ../sql/sql_test.cc
           ../sql/sql_trigger.cc ../sql/sql_udf.cc ../sql/sql_union.cc
           ../sql/sql_update.cc ../sql/sql_view.cc ../sql/sql_profile.cc
           ../sql/gcalc_tools.cc ../sql/gcalc_slicescan.cc
           ../sql/strfunc.cc ../sql/table.cc ../sql/thr_malloc.cc
           ../sql/sql_time.cc ../sql/tztime.cc ../sql/uniques.cc ../sql/unireg.cc
           ../sql/partition_info.cc ../sql/sql_connect.cc 
           ../sql/scheduler.cc ../sql/sql_audit.cc
           ../sql/sql_alter.cc ../sql/sql_partition_admin.cc
           ../sql/event_parse_data.cc
           ../sql/sql_signal.cc ../sql/rpl_handler.cc
           ../sql/sys_vars.cc
           ${CMAKE_BINARY_DIR}/sql/sql_builtin.cc
           ../sql/mdl.cc ../sql/transaction.cc
           ../sql/sql_join_cache.cc
           ../sql/multi_range_read.cc
           ../sql/opt_index_cond_pushdown.cc
           ../sql/opt_subselect.cc
           ../sql/create_options.cc ../sql/rpl_utility.cc
           ../sql/rpl_reporting.cc
           ../sql/sql_expression_cache.cc
           ${GEN_SOURCES}
           ${MYSYS_LIBWRAP_SOURCE}
)


ADD_CONVENIENCE_LIBRARY(sql_embedded ${SQL_EMBEDDED_SOURCES})
DTRACE_INSTRUMENT(sql_embedded)
ADD_DEPENDENCIES(sql_embedded GenError GenServerSource)

# On Windows, static embedded server library is called mysqlserver.lib
# On Unix, it is libmysqld.a
IF(WIN32)
  SET(MYSQLSERVER_OUTPUT_NAME mysqlserver)
  SET(COMPONENT_MYSQLSERVER "Embedded")
  SET(COMPONENT_LIBMYSQLD "Embedded")
ELSE()
  SET(MYSQLSERVER_OUTPUT_NAME mysqld)
  SET(COMPONENT_MYSQLSERVER "Development")
  SET(COMPONENT_LIBMYSQLD "Server")
ENDIF()


SET(LIBS 
  dbug strings regex mysys vio 
  ${ZLIB_LIBRARY} ${SSL_LIBRARIES} 
  ${LIBWRAP} ${LIBCRYPT} ${LIBDL}
  ${MYSQLD_STATIC_PLUGIN_LIBS}
  sql_embedded
)

# Some storage engine were compiled for embedded specifically
# (with corresponding target ${engine}_embedded)
SET(EMBEDDED_LIBS)
FOREACH(LIB ${LIBS})
  GET_TARGET_PROPERTY(EMBEDDED_LOCATION ${LIB}_embedded LOCATION)
  IF(EMBEDDED_LOCATION)
    LIST(APPEND EMBEDDED_LIBS ${LIB}_embedded)
  ELSE()
    LIST(APPEND EMBEDDED_LIBS ${LIB})
  ENDIF()
ENDFOREACH()

MERGE_LIBRARIES(mysqlserver STATIC ${EMBEDDED_LIBS} 
  OUTPUT_NAME ${MYSQLSERVER_OUTPUT_NAME} COMPONENT ${COMPONENT_MYSQLSERVER})

INSTALL(FILES embedded_priv.h DESTINATION ${INSTALL_INCLUDEDIR}/private COMPONENT  ${COMPONENT_MYSQLSERVER})

# Visual Studio users need debug  static library 
IF(MSVC)
 INSTALL_DEBUG_TARGET(mysqlserver DESTINATION ${INSTALL_LIBDIR}/debug)
ENDIF()

IF(UNIX)
 INSTALL_DEBUG_TARGET(mysqlserver DESTINATION ${INSTALL_LIBDIR} RENAME
  ${CMAKE_STATIC_LIBRARY_PREFIX}mysqld-debug)
ENDIF()

# List of exported functions in embedded (client api except client plugin or 
# async (*_start/*_cont functions)

SET(EMBEDDED_API)

FOREACH(f ${CLIENT_API_FUNCTIONS})
  IF(f MATCHES "plugin|_start$|_cont$")
    # Ignore functions, embedded does not export them
  ELSE()
    SET(EMBEDDED_API ${EMBEDDED_API} ${f})
  ENDIF()	
ENDFOREACH()

IF(NOT DISABLE_SHARED)
  MERGE_LIBRARIES(libmysqld SHARED mysqlserver EXPORTS ${EMBEDDED_API}
    COMPONENT  ${COMPONENT_LIBMYSQLD})
  IF(UNIX)
    # Name the shared library, handle versioning (provides same api as client
    # library hence the same version)
    SET_TARGET_PROPERTIES(libmysqld PROPERTIES 
      OUTPUT_NAME mysqld 
      SOVERSION "${SHARED_LIB_MAJOR_VERSION}")
    # Clean direct output flags, as 2 targets have the same base name
    # libmysqld
    SET_TARGET_PROPERTIES(libmysqld PROPERTIES CLEAN_DIRECT_OUTPUT 1)
    SET_TARGET_PROPERTIES(mysqlserver PROPERTIES CLEAN_DIRECT_OUTPUT 1)
    IF(LIBMYSQLD_SO_EXTRA_LIBS)
       TARGET_LINK_LIBRARIES(libmysqld ${LIBMYSQLD_SO_EXTRA_LIBS})
    ENDIF()
  ENDIF()
ENDIF()
