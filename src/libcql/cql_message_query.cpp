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
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_serialization.hpp"
#include "libcql/internal/cql_util.hpp"

#include "libcql/cql_message_query.hpp"

cql::cql_message_query_t::cql_message_query_t() :
    _consistency(0),
    _query()
{}

cql::cql_message_query_t::cql_message_query_t(const std::string& query,
                                              cql::cql_short_t consistency) :
    _consistency(consistency),
    _query(query)
{}

const std::string&
cql::cql_message_query_t::query() const
{
    return _query;
}

cql::cql_short_t
cql::cql_message_query_t::consistency() const
{
    return _consistency;
}

void
cql::cql_message_query_t::query(const std::string& q)
{
    _query = q;
}

void
cql::cql_message_query_t::consistency(cql::cql_short_t consistency)
{
    _consistency = consistency;
}

cql::cql_byte_t
cql::cql_message_query_t::opcode() const
{
    return CQL_OPCODE_QUERY;
}

cql::cql_int_t
cql::cql_message_query_t::size() const
{
    std::stringstream ss(std::stringstream::out);
    write(ss);
    return ss.str().size();
}

std::string
cql::cql_message_query_t::str() const
{
    return _query + " " + cql::internal::get_consistency_string(_consistency);
}

std::istream&
cql::cql_message_query_t::read(std::istream& input)
{
    cql::internal::decode_long_string(input, _query);
    input.read(reinterpret_cast<char*>(&_consistency), sizeof(_consistency));
    _consistency = ntohs(_consistency);
    return input;
}

std::ostream&
cql::cql_message_query_t::write(std::ostream& output) const
{
    cql::internal::encode_long_string(output, _query);
    cql::internal::encode_short(output, _consistency);
    return output;
}
