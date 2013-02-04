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

#include "libcql/internal/cql_defines.hpp"
#include "libcql/cql_serialization.hpp"

#include "libcql/cql_message_event_schema.hpp"

cql::cql_message_event_schema_t::cql_message_event_schema_t() :
    _change(CQL_EVENT_SCHEMA_UNKNOWN),
    _keyspace(),
    _column_family()
{}

cql::cql_message_event_schema_t::cql_message_event_schema_t(cql_event_schema_enum change,
                                                            const std::string& keyspace,
                                                            const std::string& column_family) :

    _change(change),
    _keyspace(keyspace),
    _column_family(column_family)
{}

cql::cql_byte_t
cql::cql_message_event_schema_t::opcode() const
{
    return CQL_OPCODE_EVENT;
}

cql::cql_int_t
cql::cql_message_event_schema_t::size() const
{
    return 0;
}

std::ostream&
cql::cql_message_event_schema_t::write(std::ostream& output) const
{
    return output;
}

std::istream&
cql::cql_message_event_schema_t::read(std::istream& input)
{
    std::string change;
    cql::decode_string(input, change);
    cql::decode_string(input, _keyspace);
    cql::decode_string(input, _column_family);

    if (change == CQL_EVENT_SCHEMA_CHANGE_CREATED) {
        _change = CQL_EVENT_SCHEMA_CREATED;
    }
    else if (change == CQL_EVENT_SCHEMA_CHANGE_DROPPED) {
        _change = CQL_EVENT_SCHEMA_DROPPED;
    }
    else if (change == CQL_EVENT_SCHEMA_CHANGE_UPDATED) {
        _change = CQL_EVENT_SCHEMA_UPDATED;
    }
    else {
        _change = CQL_EVENT_SCHEMA_UNKNOWN;
    }

    return input;
}


std::string
cql::cql_message_event_schema_t::str() const
{
    return "";
}

cql::cql_message_event_schema_t::cql_event_enum
cql::cql_message_event_schema_t::event_type() const
{
    return CQL_EVENT_TYPE_SCHEMA;
}

cql::cql_message_event_schema_t::cql_event_schema_enum
cql::cql_message_event_schema_t::change() const
{
    return _change;
}

const std::string&
cql::cql_message_event_schema_t::keyspace() const
{
    return _keyspace;
}

const std::string&
cql::cql_message_event_schema_t::column_family() const
{
    return _column_family;
}
