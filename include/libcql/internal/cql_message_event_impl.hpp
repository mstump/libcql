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

#ifndef CQL_MESSAGE_EVENT_IMPL_H_
#define CQL_MESSAGE_EVENT_IMPL_H_

#include <string>

#include "libcql/cql_event.hpp"
#include "libcql/internal/cql_message.hpp"

namespace cql {

    class cql_message_event_impl_t :
        public cql_event_t,
        public cql_message_t
    {
    public:

        cql_message_event_impl_t();

        cql_message_event_impl_t(size_t size);

        cql::cql_opcode_enum
        opcode() const;

        cql_int_t
        size() const;

        std::string
        str() const;

        virtual cql_event_enum
        event_type() const;

        virtual cql_event_topology_enum
        topology_change() const;

        virtual cql_event_status_enum
        status_change() const;

        virtual cql_event_schema_enum
        schema_change() const;

        virtual const std::string&
        keyspace() const;

        virtual const std::string&
        column_family() const;

        virtual const std::string&
        ip() const;

        virtual cql_int_t
        port() const;

        bool
        consume(cql::cql_error_t* err);

        bool
        prepare(cql::cql_error_t* err);

        cql_message_buffer_t
        buffer();

    private:
        cql::cql_message_buffer_t _buffer;

        cql_event_enum            _event_type;
        cql_event_topology_enum   _topology_change;
        cql_event_schema_enum     _schema_change;
        cql_event_status_enum     _status_change;

        std::string               _ip;
        cql_int_t                 _port;
        std::string               _keyspace;
        std::string               _column_family;
    };

} // namespace cql

#endif // CQL_MESSAGE_EVENT_IMPL_H_
