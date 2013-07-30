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
#include <sstream>
#include <arpa/inet.h>
#include <boost/lexical_cast.hpp>

#include "cql_vector_stream.hpp"
#include "cql_serialization.hpp"
#include "cql_util.hpp"
#include "cql_defines.hpp"

#include "cql_message_query.hpp"

cql::cql_message_query_t::cql_message_query_t() :
    _buffer(new std::vector<cql_byte_t>()),
    _consistency(0),
    _query()
{}

cql::cql_message_query_t::cql_message_query_t(
    size_t size) :
    _buffer(new std::vector<cql_byte_t>(size)),
    _consistency(0),
    _query()
{}

cql::cql_message_query_t::cql_message_query_t(
    const std::string& query,
    cql_short_t        consistency) :
    _buffer(new std::vector<cql_byte_t>()),
    _consistency(consistency),
    _query(query)
{}

cql::cql_message_buffer_t
cql::cql_message_query_t::buffer()
{
    return _buffer;
}

const std::string&
cql::cql_message_query_t::query() const
{
    return _query;
}

cql_short_t
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
cql::cql_message_query_t::consistency(
    cql_short_t consistency)
{
    _consistency = consistency;
}

cql_byte_t
cql::cql_message_query_t::opcode() const
{
    return CQL_OPCODE_QUERY;
}

cql_int_t
cql::cql_message_query_t::size() const
{
    return _buffer->size();
}

std::string
cql::cql_message_query_t::str() const
{
    return _query + " " + boost::lexical_cast<std::string>(_consistency);
}

bool
cql::cql_message_query_t::consume(
    cql::cql_error_t*)
{
    cql::vector_stream_t buffer(*_buffer);
    std::istream stream(&buffer);
    cql::decode_long_string(stream, _query);
    stream.read(reinterpret_cast<char*>(&_consistency), sizeof(_consistency));
    _consistency = ntohs(_consistency);
    return true;
}

bool
cql::cql_message_query_t::prepare(
    cql::cql_error_t*)
{
    _buffer->resize(_query.size() + sizeof(cql_int_t) + sizeof(cql_short_t));

    cql::vector_stream_t buffer(*_buffer);
    std::ostream stream(&buffer);
    cql::encode_long_string(stream, _query);
    cql::encode_short(stream, _consistency);
    return true;
}
