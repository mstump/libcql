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
#include "libcql/cql_vector_stream.hpp"
#include "libcql/cql_serialization.hpp"
#include "libcql/internal/cql_defines.hpp"

#include "libcql/internal/cql_message_prepare_impl.hpp"

cql::cql_message_prepare_impl_t::cql_message_prepare_impl_t() :
    _buffer(new std::vector<cql_byte_t>())
{}

cql::cql_message_prepare_impl_t::cql_message_prepare_impl_t(size_t size) :
    _buffer(new std::vector<cql_byte_t>(size))
{}

cql::cql_message_prepare_impl_t::cql_message_prepare_impl_t(const std::string& query) :
    _buffer(new std::vector<cql_byte_t>()),
    _query(query)
{}

cql::cql_message_buffer_t
cql::cql_message_prepare_impl_t::buffer()
{
    return _buffer;
}

const std::string&
cql::cql_message_prepare_impl_t::query() const
{
    return _query;
}

void
cql::cql_message_prepare_impl_t::query(const std::string& q)
{
    _query = q;
}

cql::cql_opcode_enum
cql::cql_message_prepare_impl_t::opcode() const
{
    return CQL_OPCODE_PREPARE;
}

cql::cql_int_t
cql::cql_message_prepare_impl_t::size() const
{
    return _buffer->size();
}

std::string
cql::cql_message_prepare_impl_t::str() const
{
    return _query;
}

bool
cql::cql_message_prepare_impl_t::consume(cql::cql_error_t* err)
{
    cql::vector_stream_t buffer(*_buffer);
    std::istream stream(&buffer);
    cql::decode_long_string(stream, _query);
    return true;
}

bool
cql::cql_message_prepare_impl_t::prepare(cql::cql_error_t* err)
{
    _buffer->resize(_query.size() + sizeof(cql::cql_int_t));
    cql::vector_stream_t buffer(*_buffer);
    std::ostream stream(&buffer);
    cql::encode_long_string(stream, _query);
    return true;
}
