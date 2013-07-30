/*
  Copyright (c) 2013 Matthew Stump

  This file is part of libcql.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef CQL_MESSAGE_EVENT_H_
#define CQL_MESSAGE_EVENT_H_

#include <string>
#include "cql_message.hpp"

namespace cql {

    class cql_message_event_t :
        public cql_message_t
    {
    public:

        cql_message_event_t();

        cql_message_event_t(
            size_t size);

        cql_byte_t
        opcode() const;

        cql_int_t
        size() const;

        std::string
        str() const;

        cql_int_t
        event_type() const;

        cql_int_t
        topology_change() const;

        cql_int_t
        status_change() const;

        cql_int_t
        schema_change() const;

        const std::string&
        keyspace() const;

        const std::string&
        column_family() const;

        const std::string&
        ip() const;

        cql_int_t
        port() const;

        bool
        consume(
            cql::cql_error_t* err);

        bool
        prepare(
            cql::cql_error_t* err);

        cql_message_buffer_t
        buffer();

    private:
        cql::cql_message_buffer_t _buffer;
        cql_int_t                 _event_type;
        cql_int_t                 _topology_change;
        cql_int_t                 _schema_change;
        cql_int_t                 _status_change;
        std::string               _ip;
        cql_int_t                 _port;
        std::string               _keyspace;
        std::string               _column_family;
    };

} // namespace cql

#endif // CQL_MESSAGE_EVENT_H_
