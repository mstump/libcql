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

#include <iomanip>
#include <sstream>

#include "serialization.hpp"
#include "util.hpp"
#include "cql_message_prepare.hpp"


cql::cql_message_prepare_t::cql_message_prepare_t() :
    _query()
{}

cql::cql_message_prepare_t::cql_message_prepare_t(const std::string& query) :
    _query(query)
{}

const std::string&
cql::cql_message_prepare_t::query() const
{
    return _query;
}

void
cql::cql_message_prepare_t::query(const std::string& q)
{
    _query = q;
}

cql_byte_t
cql::cql_message_prepare_t::opcode() const
{
    return CQL_OPCODE_PREPARE;
}

cql_int_t
cql::cql_message_prepare_t::size() const
{
    std::stringstream ss(std::stringstream::out);
    write(ss);
    return ss.str().size();
}

std::string
cql::cql_message_prepare_t::str() const
{
    std::stringstream output;
    output << _query;
    output << " " << std::setfill('0') << std::string("0x") << std::setw(2);
    return output.str();
}

std::istream&
cql::cql_message_prepare_t::read(std::istream& input)
{
    cql::internal::decode_long_string(input, _query);
    return input;
}

std::ostream&
cql::cql_message_prepare_t::write(std::ostream& output) const
{
    cql::internal::encode_long_string(output, _query);
    return output;
}
