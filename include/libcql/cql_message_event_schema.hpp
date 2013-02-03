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

#ifndef CQL_MESSAGE_EVENT_SCHEMA_H_
#define CQL_MESSAGE_EVENT_SCHEMA_H_

#include <istream>
#include <ostream>
#include <string>
#include <boost/noncopyable.hpp>

#include "libcql/cql_message_event.hpp"

namespace cql {

    class cql_message_event_schema_t :
        boost::noncopyable,
        public cql_message_event_t
    {

    public:

        enum cql_event_schema_enum {CQL_EVENT_SCHEMA_UNKNOWN,
                                    CQL_EVENT_SCHEMA_CREATED,
                                    CQL_EVENT_SCHEMA_DROPPED,
                                    CQL_EVENT_SCHEMA_UPDATED};

        cql_message_event_schema_t();

        cql_message_event_schema_t(cql_event_schema_enum type,
                                   const std::string& keyspace,
                                   const std::string& column_family);

        cql::cql_byte_t
        opcode() const;

        cql_int_t
        size() const;

        std::istream&
        read(std::istream& input);

        std::ostream&
        write(std::ostream& output) const;

        std::string
        str() const;

        cql_event_enum
        event_type() const;

        cql_event_schema_enum
        change() const;

        const std::string&
        keyspace() const;

        const std::string&
        column_family() const;

    private:
        cql_event_schema_enum  _change;
        std::string            _keyspace;
        std::string            _column_family;
    };

} // namespace cql

#endif // CQL_MESSAGE_EVENT_SCHEMA_H_
