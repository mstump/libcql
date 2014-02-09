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

#include <sstream>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>
#include "libcql/cql_vector_stream.hpp"
#include "libcql/cql_serialization.hpp"
#include "libcql/internal/cql_defines.hpp"

#include "libcql/internal/cql_message_register_impl.hpp"

cql::cql_opcode_enum
cql::cql_message_register_impl_t::opcode() const
{
    return CQL_OPCODE_REGISTER;
}

void
cql::cql_message_register_impl_t::events(const std::list<std::string>& c)
{
    _events = c;
}

const std::list<std::string>&
cql::cql_message_register_impl_t::events() const
{
    return _events;
}

std::string
cql::cql_message_register_impl_t::str() const
{
    return std::string("[") + boost::algorithm::join(_events, ", ") + "]";
}

bool
cql::cql_message_register_impl_t::consume(cql::cql_error_t*)
{
    cql::vector_stream_t buffer(_buffer);
    std::istream stream(&buffer);

    cql::decode_string_list(stream, _events);
    return true;
}

bool
cql::cql_message_register_impl_t::prepare(cql::cql_error_t*)
{
    size_t size = sizeof(cql_short_t);
    BOOST_FOREACH(const std::string& event, _events) {
        size += event.size() + sizeof(cql_short_t);
    }
    _buffer.resize(size);

    cql::vector_stream_t buffer(_buffer);
    std::ostream stream(&buffer);

    cql::encode_string_list(stream, _events);
    return true;
}
