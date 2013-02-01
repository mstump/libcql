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

#include <sstream>
#include <boost/algorithm/string/join.hpp>
#include "internal/serialization.hpp"

#include "internal/cql_message_register.hpp"

cql::cql_message_register_t::cql_message_register_t()
{}

cql_byte_t
cql::cql_message_register_t::opcode() const
{
    return CQL_OPCODE_REGISTER;
}

cql_int_t
cql::cql_message_register_t::size() const
{
    std::stringstream ss(std::stringstream::out);
    write(ss);
    return ss.str().size();
}

void
cql::cql_message_register_t::events(const std::list<std::string>& c)
{
    _events = c;
}

const std::list<std::string>&
cql::cql_message_register_t::events() const
{
    return _events;
}

std::string
cql::cql_message_register_t::str() const
{
    return std::string("[") + boost::algorithm::join(_events, ", ") + "]";
}

std::istream&
cql::cql_message_register_t::read(std::istream& input)
{
    cql::internal::decode_string_list(input, _events);
    return input;
}

std::ostream&
cql::cql_message_register_t::write(std::ostream& output) const
{
    cql::internal::encode_string_list(output, _events);
    return output;
}
