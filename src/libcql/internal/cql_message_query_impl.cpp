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
#include "libcql/cql_vector_stream.hpp"
#include "libcql/cql_serialization.hpp"
#include "libcql/internal/cql_util.hpp"
#include "libcql/internal/cql_defines.hpp"

#include "libcql/internal/cql_message_query_impl.hpp"

cql::cql_message_query_impl_t::cql_message_query_impl_t(const std::string& query,
                                                        cql::cql_short_t consistency) :
    _consistency(consistency),
    _query(query)
{}

const std::string&
cql::cql_message_query_impl_t::query() const
{
    return _query;
}

cql::cql_short_t
cql::cql_message_query_impl_t::consistency() const
{
    return _consistency;
}

void
cql::cql_message_query_impl_t::query(const std::string& q)
{
    _query = q;
}

void
cql::cql_message_query_impl_t::consistency(cql::cql_short_t consistency)
{
    _consistency = consistency;
}

cql::cql_opcode_enum
cql::cql_message_query_impl_t::opcode() const
{
    return CQL_OPCODE_QUERY;
}

std::string
cql::cql_message_query_impl_t::str() const
{
    return _query + " " + cql::get_consistency_string(_consistency);
}

bool
cql::cql_message_query_impl_t::consume(cql::cql_error_t*)
{
    cql::vector_stream_t buffer(_buffer);
    std::istream stream(&buffer);
    cql::decode_long_string(stream, _query);
    stream.read(reinterpret_cast<char*>(&_consistency), sizeof(_consistency));
    _consistency = ntohs(_consistency);
    return true;
}

bool
cql::cql_message_query_impl_t::prepare(cql::cql_error_t*)
{
    _buffer.resize(_query.size() + sizeof(cql::cql_int_t) + sizeof(cql::cql_short_t));

    cql::vector_stream_t buffer(_buffer);
    std::ostream stream(&buffer);
    cql::encode_long_string(stream, _query);
    cql::encode_short(stream, _consistency);
    return true;
}
