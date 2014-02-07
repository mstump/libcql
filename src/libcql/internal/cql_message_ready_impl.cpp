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

#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_message_ready_impl.hpp"

cql::cql_opcode_enum
cql::cql_message_ready_impl_t::opcode() const
{
    return CQL_OPCODE_READY;
}

std::string
cql::cql_message_ready_impl_t::str() const
{
    return "READY";
}

bool
cql::cql_message_ready_impl_t::consume(cql::cql_error_t*)
{
    return true;
}

bool
cql::cql_message_ready_impl_t::prepare(cql::cql_error_t*)
{
    return true;
}
