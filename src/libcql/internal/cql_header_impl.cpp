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
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include "libcql/cql_vector_stream.hpp"
#include "libcql/cql_serialization.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_util.hpp"

#include "libcql/internal/cql_header_impl.hpp"

#define CQL_HEADER_SIZE sizeof(_version) + sizeof(_flags) + sizeof(_stream) + sizeof(_opcode) + sizeof(_length)

cql::cql_header_impl_t::cql_header_impl_t() :
    _buffer(new std::vector<cql_byte_t>(CQL_HEADER_SIZE, 0)),
    _version(0),
    _flags(0),
    _stream(0),
    _opcode(0),
    _length(0)
{}

cql::cql_header_impl_t::cql_header_impl_t(cql::cql_byte_t version,
                                          cql::cql_byte_t flags,
                                          cql::cql_stream_id_t stream,
                                          cql::cql_byte_t opcode,
                                          cql::cql_int_t length) :
    _buffer(new std::vector<cql_byte_t>(CQL_HEADER_SIZE, 0)),
    _version(version),
    _flags(flags),
    _stream(stream),
    _opcode(opcode),
    _length(length)
{}

cql::cql_message_buffer_t
cql::cql_header_impl_t::buffer()
{
    return _buffer;
}

std::string
cql::cql_header_impl_t::str() const
{
    std::stringstream output;
    output << std::setfill('0') << "0x";

    BOOST_FOREACH(cql::cql_byte_t b, *_buffer) {
        output << std::setw(2) << cql::hex(b);
    }

    output << " {version: 0x" << std::setw(2) << cql::hex(_version);
    output << ", flags: 0x" << std::setw(2) << cql::hex(_flags);
    output << ", stream: 0x" << std::setw(2) << cql::hex(_stream);
    output << ", opcode: 0x" << std::setw(2) << cql::hex(_opcode);
    output << ", length: " << boost::lexical_cast<std::string>(_length) << "}";

    return output.str();
}

bool
cql::cql_header_impl_t::prepare(cql::cql_error_t* err)
{
    cql::vector_stream_t buffer(*_buffer);
    std::ostream stream(&buffer);

    stream.put(_version);
    stream.put(_flags);
    stream.put(_stream);
    stream.put(_opcode);
    cql::encode_int(stream, _length);
    return true;
}

bool
cql::cql_header_impl_t::consume(cql::cql_error_t* err)
{
    cql::vector_stream_t buffer(*_buffer);
    std::istream stream(&buffer);

    _version = stream.get();
    _flags = stream.get();
    _stream = stream.get();
    _opcode = stream.get();
    cql::decode_int(stream, _length);
    return true;
}

cql::cql_int_t
cql::cql_header_impl_t::size() const
{
    return CQL_HEADER_SIZE;
}

cql::cql_byte_t
cql::cql_header_impl_t::version() const
{
    return _version;
}

cql::cql_byte_t
cql::cql_header_impl_t::flags() const
{
    return _flags;
}

cql::cql_stream_id_t
cql::cql_header_impl_t::stream() const
{
    return _stream;
}

cql::cql_byte_t
cql::cql_header_impl_t::opcode() const
{
    return _opcode;
}

cql::cql_int_t
cql::cql_header_impl_t::length() const
{
    return _length;
}

void
cql::cql_header_impl_t::version(cql::cql_byte_t v)
{
    _version = v;
}

void
cql::cql_header_impl_t::flags(cql::cql_byte_t v)
{
    _flags = v;
}

void
cql::cql_header_impl_t::stream(cql::cql_stream_id_t v)
{
    _stream = v;
}

void
cql::cql_header_impl_t::opcode(cql::cql_byte_t v)
{
    _opcode = v;
}

void
cql::cql_header_impl_t::length(cql::cql_int_t v)
{
    _length = v;
}
