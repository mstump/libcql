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

#include "cql_defines.hpp"
#include "cql_message_options.hpp"

cql::cql_message_options_t::cql_message_options_t() :
    _buffer(new std::vector<cql_byte_t>(0))
{}

cql::cql_message_options_t::cql_message_options_t(
    size_t size) :
    _buffer(new std::vector<cql_byte_t>(size, 0))
{}

cql::cql_message_buffer_t
cql::cql_message_options_t::buffer()
{
    return _buffer;
}

cql_int_t
cql::cql_message_options_t::size() const
{
    return _buffer->size();
}

cql_byte_t
cql::cql_message_options_t::opcode() const
{
    return CQL_OPCODE_OPTIONS;
}

std::string
cql::cql_message_options_t::str() const
{
    return "OPTIONS";
}

bool
cql::cql_message_options_t::consume(
    cql::cql_error_t*)
{
    return true;
}

bool
cql::cql_message_options_t::prepare(
    cql::cql_error_t*)
{
    return true;
}
