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

#include <map>
#include <sstream>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/algorithm/string/join.hpp>
#include "internal/cql_defines.hpp"
#include "internal/cql_serialization.hpp"

#include "internal/cql_message_credentials.hpp"

cql::cql_message_credentials_t::cql_message_credentials_t() :
    _credentials()
{}

void
cql::cql_message_credentials_t::credentials(const std::map<std::string, std::string>& c)
{
    _credentials = c;
}

const std::map<std::string, std::string>&
cql::cql_message_credentials_t::credentials() const
{
    return _credentials;
}

cql::cql_byte_t
cql::cql_message_credentials_t::opcode() const
{
    return CQL_OPCODE_CREDENTIALS;
}

cql::cql_int_t
cql::cql_message_credentials_t::size() const
{
    std::stringstream ss(std::stringstream::out);
    write(ss);
    return ss.str().size();
}

std::string
cql::cql_message_credentials_t::str() const
{
    std::list<std::string> keys;
    boost::copy(
        _credentials | boost::adaptors::map_keys,
        std::back_inserter(keys));

    std::stringstream output;
    output << "[" << boost::algorithm::join(keys, ", ") << "]";
    return output.str();
}

std::istream&
cql::cql_message_credentials_t::read(std::istream& input)
{
    cql::internal::decode_string_map(input, _credentials);
    return input;
}

std::ostream&
cql::cql_message_credentials_t::write(std::ostream& output) const
{
    cql::internal::encode_string_map(output, _credentials);
    return output;
}
