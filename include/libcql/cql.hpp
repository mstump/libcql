/*
  Copyright (c) 2012 Matthew Stump

  This file is part of libcql.

  libcql is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  libcql is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __CQL_H_INCLUDED__
#define __CQL_H_INCLUDED__

#include <stdint.h>
#include <string>


namespace cql {
    typedef uint8_t   cql_byte_t;
    typedef uint16_t  cql_short_t;
    typedef int32_t   cql_int_t;
    typedef int64_t   cql_bigint_t;
    typedef int8_t    cql_stream_id_t;

    enum cql_opcode_enum {
        CQL_OPCODE_ERROR        = 0x00,
        CQL_OPCODE_STARTUP      = 0x01,
        CQL_OPCODE_READY        = 0x02,
        CQL_OPCODE_AUTHENTICATE = 0x03,
        CQL_OPCODE_CREDENTIALS  = 0x04,
        CQL_OPCODE_OPTIONS      = 0x05,
        CQL_OPCODE_SUPPORTED    = 0x06,
        CQL_OPCODE_QUERY        = 0x07,
        CQL_OPCODE_RESULT       = 0x08,
        CQL_OPCODE_PREPARE      = 0x09,
        CQL_OPCODE_EXECUTE      = 0x0A,
        CQL_OPCODE_REGISTER     = 0x0B,
        CQL_OPCODE_EVENT        = 0x0C
    };

    enum cql_consistency_enum {
        CQL_CONSISTENCY_ANY          = 0x0000,
        CQL_CONSISTENCY_ONE          = 0x0001,
        CQL_CONSISTENCY_TWO          = 0x0002,
        CQL_CONSISTENCY_THREE        = 0x0003,
        CQL_CONSISTENCY_QUORUM       = 0x0004,
        CQL_CONSISTENCY_ALL          = 0x0005,
        CQL_CONSISTENCY_LOCAL_QUORUM = 0x0006,
        CQL_CONSISTENCY_EACH_QUORUM  = 0x0007
    };

    enum cql_column_type_enum {
        CQL_COLUMN_TYPE_UNKNOWN   = 0xFFFF,
        CQL_COLUMN_TYPE_CUSTOM    = 0x0000,
        CQL_COLUMN_TYPE_ASCII     = 0x0001,
        CQL_COLUMN_TYPE_BIGINT    = 0x0002,
        CQL_COLUMN_TYPE_BLOB      = 0x0003,
        CQL_COLUMN_TYPE_BOOLEAN   = 0x0004,
        CQL_COLUMN_TYPE_COUNTER   = 0x0005,
        CQL_COLUMN_TYPE_DECIMAL   = 0x0006,
        CQL_COLUMN_TYPE_DOUBLE    = 0x0007,
        CQL_COLUMN_TYPE_FLOAT     = 0x0008,
        CQL_COLUMN_TYPE_INT       = 0x0009,
        CQL_COLUMN_TYPE_TEXT      = 0x000A,
        CQL_COLUMN_TYPE_TIMESTAMP = 0x000B,
        CQL_COLUMN_TYPE_UUID      = 0x000C,
        CQL_COLUMN_TYPE_VARCHAR   = 0x000D,
        CQL_COLUMN_TYPE_VARINT    = 0x000E,
        CQL_COLUMN_TYPE_TIMEUUID  = 0x000F,
        CQL_COLUMN_TYPE_INET      = 0x0010,
        CQL_COLUMN_TYPE_LIST      = 0x0020,
        CQL_COLUMN_TYPE_MAP       = 0x0021,
        CQL_COLUMN_TYPE_SET       = 0x0022
    };

    enum cql_log_level_enum {
        CQL_LOG_CRITICAL = 0x00,
        CQL_LOG_ERROR    = 0x01,
        CQL_LOG_INFO     = 0x02,
        CQL_LOG_DEBUG    = 0x03
    };

    enum cql_result_type_enum {
        CQL_RESULT_VOID              = 0x0001,
        CQL_RESULT_ROWS              = 0x0002,
        CQL_RESULT_SET_KEYSPACE      = 0x0003,
        CQL_RESULT_PREPARED          = 0x0004,
        CQL_RESULT_SCHEMA_CHANGE     = 0x0005
    };

    enum cql_event_enum {
        CQL_EVENT_TYPE_UNKOWN   = 0x00,
        CQL_EVENT_TYPE_TOPOLOGY = 0x01,
        CQL_EVENT_TYPE_STATUS   = 0x02,
        CQL_EVENT_TYPE_SCHEMA   = 0x03
    };

    enum cql_event_schema_enum {
        CQL_EVENT_SCHEMA_UNKNOWN = 0x00,
        CQL_EVENT_SCHEMA_CREATED = 0x01,
        CQL_EVENT_SCHEMA_DROPPED = 0x02,
        CQL_EVENT_SCHEMA_UPDATED = 0x03
    };

    enum cql_event_status_enum {
        CQL_EVENT_STATUS_UNKNOWN = 0x00,
        CQL_EVENT_STATUS_UP      = 0x01,
        CQL_EVENT_STATUS_DOWN    = 0x02
    };

    enum cql_event_topology_enum {
        CQL_EVENT_TOPOLOGY_UNKNOWN     = 0x00,
        CQL_EVENT_TOPOLOGY_ADD_NODE    = 0x01,
        CQL_EVENT_TOPOLOGY_REMOVE_NODE = 0x02,
    };

} // namespace cql
#endif // __CQL_H_INCLUDED__
