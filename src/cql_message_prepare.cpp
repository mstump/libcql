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
#include "cql_vector_stream.hpp"
#include "cql_serialization.hpp"
#include "cql_defines.hpp"

#include "cql_message_prepare.hpp"

cql::cql_message_prepare_t::cql_message_prepare_t() :
    _buffer(new std::vector<cql_byte_t>())
{}

cql::cql_message_prepare_t::cql_message_prepare_t(
    size_t size) :
    _buffer(new std::vector<cql_byte_t>(size))
{}

cql::cql_message_prepare_t::cql_message_prepare_t(
    const std::string& query) :
    _buffer(new std::vector<cql_byte_t>()),
    _query(query)
{}

cql::cql_message_buffer_t
cql::cql_message_prepare_t::buffer()
{
    return _buffer;
}

const std::string&
cql::cql_message_prepare_t::query() const
{
    return _query;
}

void
cql::cql_message_prepare_t::query(
    const std::string& q)
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
    return _buffer->size();
}

std::string
cql::cql_message_prepare_t::str() const
{
    return _query;
}

bool
cql::cql_message_prepare_t::consume(
    cql::cql_error_t*)
{
    cql::vector_stream_t buffer(*_buffer);
    std::istream stream(&buffer);
    cql::decode_long_string(stream, _query);
    return true;
}

bool
cql::cql_message_prepare_t::prepare(
    cql::cql_error_t*)
{
    _buffer->resize(_query.size() + sizeof(cql_int_t));
    cql::vector_stream_t buffer(*_buffer);
    std::ostream stream(&buffer);
    cql::encode_long_string(stream, _query);
    return true;
}
