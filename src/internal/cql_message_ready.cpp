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
  along with this program.      If not, see <http://www.gnu.org/licenses/>.
*/

#include "internal/cql_message_ready.hpp"

cql::cql_message_ready_t::cql_message_ready_t()
{}

cql_byte_t
cql::cql_message_ready_t::opcode() const
{
    return CQL_OPCODE_READY;
}

cql_int_t
cql::cql_message_ready_t::size() const
{
    return 0;
}

std::string
cql::cql_message_ready_t::str() const
{
    return "READY";
}

std::istream&
cql::cql_message_ready_t::read(std::istream& input)
{
    return input;
}

std::ostream&
cql::cql_message_ready_t::write(std::ostream& output) const
{
    return output;
}
