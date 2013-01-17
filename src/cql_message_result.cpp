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
#include <iostream>
#include <sstream>
#include "serialization.hpp"
#include "util.hpp"

#include "cql_message_result.hpp"

std::string
result_type_string(cql_short_t t)
{
    switch(t) {

    case CQL_RESULT_ROWS:
        return "CQL_RESULT_ROWS";
        break;

    case CQL_RESULT_SET_KEYSPACE:
        return "CQL_RESULT_SET_KEYSPACE";
        break;

    case CQL_RESULT_PREPARED:
        return "CQL_RESULT_PREPARED";
        break;

    case CQL_RESULT_VOID:
        return "CQL_RESULT_VOID";
        break;

    default:
        return "UNKNOWN";
        break;
    }
}


cql::cql_message_result_t::cql_message_result_t() :
    _result_type(0),
    _row_count(0),
    _query_id(0)
{}

cql_int_t
cql::cql_message_result_t::result_type() const
{
    return _result_type;
}

uint8_t
cql::cql_message_result_t::opcode() const
{
    return CQL_OPCODE_RESULT;
}

cql_int_t
cql::cql_message_result_t::size() const
{
    std::stringstream ss(std::stringstream::out);
    write(ss);
    return ss.str().size();
}

std::string
cql::cql_message_result_t::str() const
{
    std::stringstream output;
    output << std::string("RESULT ") << result_type_string(_result_type);
    output << " ROW_COUNT " << _row_count;
    output << " QUERY_ID " << _query_id;
    output << " " << _metadata.str();
    return output.str();
}

std::istream&
cql::cql_message_result_t::read(std::istream& input)
{
    _keyspace_name.clear();
    _row_count = 0;

    cql::internal::decode_int(input, _result_type);

    switch(_result_type) {

    case CQL_RESULT_ROWS:
        _metadata.read(input);
        cql::internal::decode_int(input, _row_count);
        for (int i = 0; i < _row_count; ++i)
        {
            _rows.push_back(new cql_row_t(_metadata, input));
        }

        break;

    case CQL_RESULT_SET_KEYSPACE:
        cql::internal::decode_string(input, _keyspace_name);
        break;

    case CQL_RESULT_PREPARED:
        cql::internal::decode_short(input, _query_id);
        _metadata.read(input);
        break;

    case CQL_RESULT_VOID:
        break;
    }

    return input;
}

std::ostream&
cql::cql_message_result_t::write(std::ostream& output) const
{
    int32_t result_type = htonl(_result_type);
    output.write(reinterpret_cast<char*>(&result_type), sizeof(result_type));
    return output;
}

cql_int_t
cql::cql_message_result_t::row_count() const
{
    return _row_count;
}

cql_short_t
cql::cql_message_result_t::query_id() const
{
    return _query_id;
}

const cql::cql_row_t&
cql::cql_message_result_t::operator[](size_type n) const
{
    return _rows[n];
}

const cql::cql_row_t&
cql::cql_message_result_t::at(size_type n) const
{
    return _rows.at(n);
}

cql::cql_message_result_t::const_iterator
cql::cql_message_result_t::begin() const
{
    return _rows.begin();
}

cql::cql_message_result_t::const_iterator
cql::cql_message_result_t::end() const
{
    return _rows.end();
}
