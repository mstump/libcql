/*
  Copyright (c) 2013 Matthew Stump

  This file is part of libcql.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include <iomanip>
#include <iostream>
#include <sstream>
#include <boost/foreach.hpp>
#include "cql_vector_stream.hpp"
#include "cql_defines.hpp"
#include "cql_list.hpp"
#include "cql_map.hpp"
#include "cql_set.hpp"
#include "cql_util.hpp"

#include "cql_message_result.hpp"

inline std::string
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
    _buffer(new std::vector<cql_byte_t>()),
    _query_id(0),
    _result_type(CQL_RESULT_VOID)
{}

cql::cql_message_result_t::cql_message_result_t(size_t size) :
    _buffer(new std::vector<cql_byte_t>(size)),
    _query_id(0),
    _result_type(CQL_RESULT_VOID)
{}

cql::cql_message_buffer_t
cql::cql_message_result_t::buffer()
{
    return _buffer;
}

const cql::cql_result_metadata_t&
cql::cql_message_result_t::get_metadata()
{
    return _metadata;
}

cql_int_t
cql::cql_message_result_t::result_type() const
{
    return _result_type;
}

cql_byte_t
cql::cql_message_result_t::opcode() const
{
    return CQL_OPCODE_RESULT;
}

cql_int_t
cql::cql_message_result_t::size() const
{
    return _buffer->size();
}

std::string
cql::cql_message_result_t::str() const
{
    std::stringstream output;
    output << std::string("RESULT ") << result_type_string(_result_type);

    if (! _query_id.empty()) {
        output << " QUERY_ID 0x";
        output << std::setfill('0');
        BOOST_FOREACH(cql_byte_t c, _query_id) {
            output << std::setw(2) << cql::hex(c);
        }
    }
    output << " ROW_COUNT " << row_count();
    output << " METADATA " << _metadata.str();
    return output.str();
}

bool
cql::cql_message_result_t::consume(
    cql::cql_error_t*)
{
    _keyspace_name.clear();

    cql_byte_t* pos = &((*_buffer)[0]);
    pos = cql::decode_int(pos, _result_type);

    if (_result_type == CQL_RESULT_ROWS) {
        cql_int_t row_count = 0;
        pos = _metadata.read(pos);
        pos = cql::decode_int(pos, row_count);
        cql_int_t column_count = _metadata.column_count();

        value_array_t::extent_gen extents;
        _values.resize(extents[row_count][column_count]);

        for (cql_int_t r = 0; r < row_count; ++r) {
            for (cql_int_t c = 0; c < column_count; ++c) {
                _values[r][c] = pos;
                cql_int_t len = 0;
                pos = cql::decode_int(pos, len);
                if (len > 0) {
                    pos += len;
                }
            }
        }
    }

    else if (_result_type == CQL_RESULT_SET_KEYSPACE) {
        cql::decode_string(pos, _keyspace_name);
    }

    else if (_result_type == CQL_RESULT_PREPARED) {
        cql::decode_short_bytes(pos, _query_id);
    }

    return true;
}

bool
cql::cql_message_result_t::prepare(
    cql::cql_error_t*)
{
    return true;
}

const std::vector<cql_byte_t>&
cql::cql_message_result_t::query_id() const
{
    return _query_id;
}

bool
cql::cql_message_result_t::get_bool(
    size_t row,
    size_t column,
    bool& output) const
{
    if (is_valid(row, column, CQL_COLUMN_TYPE_BOOLEAN)) {
        output = *(_values[row][column] + sizeof(cql_int_t)) != 0x00;
        return true;
    }
    return false;
}

bool
cql::cql_message_result_t::get_int(
    size_t row,
    size_t column,
    cql_int_t& output) const
{
    if (is_valid(row, column, CQL_COLUMN_TYPE_INT)) {
        cql::decode_int(_values[row][column] + sizeof(cql_int_t), output);
        return true;
    }
    return false;
}

bool
cql::cql_message_result_t::get_float(
    size_t row,
    size_t column,
    float& output) const
{
    if (is_valid(row, column, CQL_COLUMN_TYPE_FLOAT)) {
        cql::decode_float(_values[row][column] + sizeof(cql_int_t), output);
        return true;
    }
    return false;
}

bool
cql::cql_message_result_t::get_double(
    size_t row,
    size_t column,
    double& output) const
{
    if (is_valid(row, column, CQL_COLUMN_TYPE_DOUBLE)) {
        cql_byte_t* p = _values[row][column] + sizeof(cql_int_t);
        cql::decode_double(p, output);
        return true;
    }
    return false;
}

bool
cql::cql_message_result_t::get_bigint(
    size_t row,
    size_t column,
    cql_bigint_t& output) const
{
    if (is_valid(row, column, CQL_COLUMN_TYPE_BIGINT)) {
        cql::decode_bigint(_values[row][column] + sizeof(cql_int_t), output);
        return true;
    }
    return false;
}

bool
cql::cql_message_result_t::get_string(
    size_t row,
    size_t column,
    std::string& output) const
{
    cql_byte_t* data = 0;
    cql_int_t size = 0;
    if (get_data(row, column, &data, size)) {
        output.assign(data, data + size);
        return true;
    }
    return false;
}

bool
cql::cql_message_result_t::get_data(
    size_t row,
    size_t column,
    cql_byte_t** output,
    cql_int_t& size) const
{
    bool empty = false;
    if (is_null(row, column, empty)) {
        if (!empty) {
            cql_byte_t* pos = _values[row][column];
            *output = cql::decode_int(pos, size);
            return true;
        }
    }
    return false;
}

bool
cql::cql_message_result_t::get_list(
    size_t row,
    size_t column,
    cql::cql_list_t** output) const
{
    if (is_valid(row, column, CQL_COLUMN_TYPE_LIST)) {
        cql_int_t member_type;
        std::string member_class;
        _metadata.collection_primary_type(column, member_type);
        _metadata.collection_primary_class(column, member_class);
        *output = new cql::cql_list_t(_values[row][column] + sizeof(cql_int_t), member_type, member_class);
        return true;
    }
    return false;
}

bool
cql::cql_message_result_t::get_set(
    size_t row,
    size_t column,
    cql::cql_set_t** output) const
{
    if (is_valid(row, column, CQL_COLUMN_TYPE_SET)) {
        cql_int_t member_type;
        std::string member_class;
        _metadata.collection_primary_type(column, member_type);
        _metadata.collection_primary_class(column, member_class);
        *output = new cql::cql_set_t(_values[row][column] + sizeof(cql_int_t), member_type, member_class);
        return true;
    }
    return false;
}

bool
cql::cql_message_result_t::get_map(
    size_t row,
    size_t column,
    cql::cql_map_t** output) const
{
    if (is_valid(row, column, CQL_COLUMN_TYPE_MAP)) {
        cql_int_t key_type;
        cql_int_t value_type;
        std::string key_class;
        std::string value_class;
        _metadata.collection_primary_type(column, key_type);
        _metadata.collection_primary_class(column, key_class);
        _metadata.collection_secondary_type(column, value_type);
        _metadata.collection_secondary_class(column, value_class);
        *output = new cql::cql_map_t(_values[row][column] + sizeof(cql_int_t), key_type, value_type, key_class, value_class);
        return true;
    }
    return false;
}
