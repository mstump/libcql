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
#include "internal/cql_defines.hpp"
#include "internal/cql_serialization.hpp"

#include "internal/cql_message_supported.hpp"

cql::cql_message_supported_t::cql_message_supported_t() :
    _versions(),
    _compressions()
{}

void
cql::cql_message_supported_t::compressions(const std::list<std::string>& c)
{
    _compressions = c;
}

const std::list<std::string>&
cql::cql_message_supported_t::compressions() const
{
    return _compressions;
}

void
cql::cql_message_supported_t::versions(const std::list<std::string>& v)
{
    _versions = v;
}

const std::list<std::string>&
cql::cql_message_supported_t::version() const
{
    return _versions;
}

cql::cql_byte_t
cql::cql_message_supported_t::opcode() const
{
    return CQL_OPCODE_SUPPORTED;
}

cql::cql_int_t
cql::cql_message_supported_t::size() const
{
    std::stringstream ss(std::stringstream::out);
    write(ss);
    return ss.str().size();
}

std::string
cql::cql_message_supported_t::str() const
{
    std::stringstream output;
    output << "{versions: [" << boost::algorithm::join(_versions, ", ");
    output << "], compressions: [" << boost::algorithm::join(_compressions, ", ") << "]}";
    return output.str();
}

std::istream&
cql::cql_message_supported_t::read(std::istream& input)
{
    std::map<std::string, std::list<std::string> > supported;
    cql::internal::decode_string_multimap(input, supported);

    if (supported.find(CQL_VERSION) != supported.end())
        _versions = supported[CQL_VERSION];

    if (supported.find(CQL_COMPRESSION) != supported.end())
        _compressions = supported[CQL_COMPRESSION];

    return input;
}

std::ostream&
cql::cql_message_supported_t::write(std::ostream& output) const
{
    std::map<std::string, std::list<std::string> > supported;

    if (!_versions.empty())
        supported.insert(std::pair<std::string, std::list<std::string> >(CQL_VERSION, _versions));

    if (!_compressions.empty())
        supported.insert(std::pair<std::string, std::list<std::string> >(CQL_COMPRESSION, _compressions));

    cql::internal::encode_string_multimap(output, supported);
    return output;
}
