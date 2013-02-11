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
#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>
#include "libcql/cql_vector_stream.hpp"
#include "libcql/cql_serialization.hpp"
#include "libcql/internal/cql_defines.hpp"

#include "libcql/internal/cql_message_register_impl.hpp"

cql::cql_message_register_impl_t::cql_message_register_impl_t() :
    _buffer(new std::vector<cql_byte_t>())
{}

cql::cql_message_register_impl_t::cql_message_register_impl_t(size_t size) :
    _buffer(new std::vector<cql_byte_t>(size))
{}

cql::cql_message_buffer_t
cql::cql_message_register_impl_t::buffer()
{
    return _buffer;
}

cql::cql_opcode_enum
cql::cql_message_register_impl_t::opcode() const
{
    return CQL_OPCODE_REGISTER;
}

cql::cql_int_t
cql::cql_message_register_impl_t::size() const
{
    return _buffer->size();
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
cql::cql_message_register_impl_t::consume(cql::cql_error_t* err)
{
    cql::vector_stream_t buffer(*_buffer);
    std::istream stream(&buffer);

    cql::decode_string_list(stream, _events);
    return true;
}

bool
cql::cql_message_register_impl_t::prepare(cql::cql_error_t* err)
{
    size_t size = sizeof(cql_short_t);
    BOOST_FOREACH(const std::string& event, _events) {
        size += event.size() + sizeof(cql_short_t);
    }
    _buffer->resize(size);

    cql::vector_stream_t buffer(*_buffer);
    std::ostream stream(&buffer);

    cql::encode_string_list(stream, _events);
    return true;
}
