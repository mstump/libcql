/*
  Copyright (c) 2013 Matthew Stump

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

#ifndef __CQL_DEFINES_H_INCLUDED__
#define __CQL_DEFINES_H_INCLUDED__

#define CQL_FRAME_MAX_SIZE 1024 * 1024 * 256

#define CQL_VERSION_1_REQUEST  0x01
#define CQL_VERSION_1_RESPONSE 0x81

#define CQL_FLAG_NOFLAG      0x00
#define CQL_FLAG_COMPRESSION 0x01
#define CQL_FLAG_TRACE       0x02

#define CQL_VERSION       "CQL_VERSION"
#define CQL_VERSION_IMPL  "3.0.0"
#define CQL_VERSION_3_0_0 "3.0.0"

#define CQL_COMPRESSION        "COMPRESSION"
#define CQL_COMPRESSION_SNAPPY "snappy"

#define CQL_OPCODE_ERROR        0x00
#define CQL_OPCODE_STARTUP      0x01
#define CQL_OPCODE_READY        0x02
#define CQL_OPCODE_AUTHENTICATE 0x03
#define CQL_OPCODE_CREDENTIALS  0x04
#define CQL_OPCODE_OPTIONS      0x05
#define CQL_OPCODE_SUPPORTED    0x06
#define CQL_OPCODE_QUERY        0x07
#define CQL_OPCODE_RESULT       0x08
#define CQL_OPCODE_PREPARE      0x09
#define CQL_OPCODE_EXECUTE      0x0A
#define CQL_OPCODE_REGISTER     0x0B
#define CQL_OPCODE_EVENT        0x0C

#define CQL_RESULT_VOID              0x0001
#define CQL_RESULT_ROWS              0x0002
#define CQL_RESULT_SET_KEYSPACE      0x0003
#define CQL_RESULT_PREPARED          0x0004
#define CQL_RESULT_SCHEMA_CHANGE     0x0005

#define CQL_RESULT_ROWS_FLAGS_GLOBAL_TABLES_SPEC 0x0001

#define CQL_EVENT_TOPOLOGY_CHANGE "TOPOLOGY_CHANGE"
#define CQL_EVENT_TOPOLOGY_CHANGE_NEW "NEW_NODE"
#define CQL_EVENT_TOPOLOGY_CHANGE_REMOVE "REMOVED_NODE"

#define CQL_EVENT_STATUS_CHANGE "STATUS_CHANGE"
#define CQL_EVENT_STATUS_CHANGE_UP "UP"
#define CQL_EVENT_STATUS_CHANGE_DOWN "DOWN"

#define CQL_EVENT_SCHEMA_CHANGE "SCHEMA_CHANGE"
#define CQL_EVENT_SCHEMA_CHANGE_CREATED "CREATED"
#define CQL_EVENT_SCHEMA_CHANGE_UPDATED "UPDATED"
#define CQL_EVENT_SCHEMA_CHANGE_DROPPED "DROPPED"

#endif // __CQL_DEFINES_H_INCLUDED__
