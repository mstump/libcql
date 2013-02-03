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
#include "libcql/internal/cql_serialization.hpp"

#include "libcql/cql_message_event_status.hpp"

cql::cql_message_event_status_t::cql_message_event_status_t() :
    _direction(CQL_EVENT_STATUS_UNKNOWN),
    _ip(),
    _port(0)
{}

cql::cql_message_event_status_t::cql_message_event_status_t(cql::cql_message_event_status_t::cql_event_status_enum direction,
                                                            const std::string& ip,
                                                            const cql::cql_int_t port) :

    _direction(direction),
    _ip(ip),
    _port(port)
{}

cql::cql_byte_t
cql::cql_message_event_status_t::opcode() const
{
    return CQL_OPCODE_EVENT;
}

cql::cql_int_t
cql::cql_message_event_status_t::size() const
{
    return 0;
}

std::ostream&
cql::cql_message_event_status_t::write(std::ostream& output) const
{
    return output;
}

std::istream&
cql::cql_message_event_status_t::read(std::istream& input)
{
    std::string direction;
    cql::internal::decode_string(input, direction);
    cql::internal::decode_inet(input, _ip, _port);

    if (direction == CQL_EVENT_STATUS_CHANGE_UP) {
        _direction = CQL_EVENT_STATUS_UP;
    }
    else if (direction == CQL_EVENT_STATUS_CHANGE_DOWN) {
        _direction = CQL_EVENT_STATUS_DOWN;
    }
    else {
        _direction = CQL_EVENT_STATUS_UNKNOWN;
    }

    return input;
}


std::string
cql::cql_message_event_status_t::str() const
{
    return "";
}

cql::cql_message_event_status_t::cql_event_enum
cql::cql_message_event_status_t::event_type() const
{
    return CQL_EVENT_TYPE_STATUS;
}

cql::cql_message_event_status_t::cql_event_status_enum
cql::cql_message_event_status_t::direction() const
{
    return _direction;
}

const std::string&
cql::cql_message_event_status_t::ip() const
{
    return _ip;
}

cql::cql_int_t
cql::cql_message_event_status_t::port() const
{
    return _port;
}
