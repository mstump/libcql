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

#define CQL_CONSISTENCY_ANY          0x0000
#define CQL_CONSISTENCY_ONE          0x0001
#define CQL_CONSISTENCY_TWO          0x0002
#define CQL_CONSISTENCY_THREE        0x0003
#define CQL_CONSISTENCY_QUORUM       0x0004
#define CQL_CONSISTENCY_ALL          0x0005
#define CQL_CONSISTENCY_LOCAL_QUORUM 0x0006
#define CQL_CONSISTENCY_EACH_QUORUM  0x0007

#define CQL_COLUMN_TYPE_CUSTOM    0x0000
#define CQL_COLUMN_TYPE_ASCII     0x0001
#define CQL_COLUMN_TYPE_BIGINT    0x0002
#define CQL_COLUMN_TYPE_BLOB      0x0003
#define CQL_COLUMN_TYPE_BOOLEAN   0x0004
#define CQL_COLUMN_TYPE_COUNTER   0x0005
#define CQL_COLUMN_TYPE_DECIMAL   0x0006
#define CQL_COLUMN_TYPE_DOUBLE    0x0007
#define CQL_COLUMN_TYPE_FLOAT     0x0008
#define CQL_COLUMN_TYPE_INT       0x0009
#define CQL_COLUMN_TYPE_TEXT      0x000A
#define CQL_COLUMN_TYPE_TIMESTAMP 0x000B
#define CQL_COLUMN_TYPE_UUID      0x000C
#define CQL_COLUMN_TYPE_VARCHAR   0x000D
#define CQL_COLUMN_TYPE_VARINT    0x000E
#define CQL_COLUMN_TYPE_TIMEUUID  0x000F
#define CQL_COLUMN_TYPE_INET      0x0010
#define CQL_COLUMN_TYPE_LIST      0x0020
#define CQL_COLUMN_TYPE_MAP       0x0021
#define CQL_COLUMN_TYPE_SET       0x0022

#define CQL_LOG_CRITICAL 0x00
#define CQL_LOG_ERROR    0x01
#define CQL_LOG_INFO     0x02
#define CQL_LOG_DEBUG    0x03

namespace cql {

    typedef uint8_t   cql_byte_t;
    typedef uint16_t  cql_short_t;
    typedef int32_t   cql_int_t;
    typedef int64_t   cql_bigint_t;
    typedef int8_t    cql_stream_id_t;

} // namespace cql
#endif // __CQL_H_INCLUDED__
