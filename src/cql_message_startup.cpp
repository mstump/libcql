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
#include <boost/algorithm/string/join.hpp>
#include "cql_message_startup.hpp"
#include "internal/serialization.hpp"

cql::cql_message_startup_t::cql_message_startup_t() :
    _version(),
    _compression()
{}

void
cql::cql_message_startup_t::compression(const std::string& c)
{
    _compression = c;
}

const std::string&
cql::cql_message_startup_t::compression() const
{
    return _compression;
}

void
cql::cql_message_startup_t::version(const std::string& v)
{
    _version = v;
}

const std::string&
cql::cql_message_startup_t::version() const
{
    return _version;
}

cql_byte_t
cql::cql_message_startup_t::opcode() const
{
    return CQL_OPCODE_STARTUP;
}

cql_int_t
cql::cql_message_startup_t::size() const
{
    std::stringstream ss(std::stringstream::out);
    write(ss);
    return ss.str().size();
}

std::string
cql::cql_message_startup_t::str() const
{
    std::stringstream output;
    output << "{version: " << _version << ", compression: " << _compression << "}";
    return output.str();
}

std::istream&
cql::cql_message_startup_t::read(std::istream& input)
{
    std::map<std::string, std::string> startup;
    cql::internal::decode_string_map(input, startup);

    if (startup.find(CQL_VERSION) != startup.end())
        _version = startup[CQL_VERSION];

    if (startup.find(CQL_COMPRESSION) != startup.end())
        _compression = startup[CQL_COMPRESSION];

    return input;
}

std::ostream&
cql::cql_message_startup_t::write(std::ostream& output) const
{
    std::map<std::string, std::string> startup;

    if (!_version.empty())
        startup.insert(std::pair<std::string, std::string>(CQL_VERSION, _version));

    if (!_compression.empty())
        startup.insert(std::pair<std::string, std::string>(CQL_COMPRESSION, _compression));

    cql::internal::encode_string_map(output, startup);
    return output;
}
