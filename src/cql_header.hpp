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

#ifndef CQL_HEADER_H_
#define CQL_HEADER_H_

#include <iomanip>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "libcql/cql.h"
#include "cql_defines.hpp"
#include "cql_serialization.hpp"
#include "cql_util.hpp"
#include "cql_vector_stream.hpp"

namespace cql {

    struct cql_error_t;

    class cql_header_t {

    public:

        cql_header_t() :
            _buffer(new std::vector<cql_byte_t>(CQL_HEADER_SIZE, 0)),
            _version(0),
            _flags(0),
            _stream(0),
            _opcode(0),
            _length(0)
        {}

        cql_header_t(
            cql_byte_t version,
            cql_byte_t flags,
            cql_stream_id_t stream,
            cql_byte_t opcode,
            cql_int_t length) :
            _buffer(new std::vector<cql_byte_t>(CQL_HEADER_SIZE, 0)),
            _version(version),
            _flags(flags),
            _stream(stream),
            _opcode(opcode),
            _length(length)
        {}

        inline std::string
        str() const
        {
            std::stringstream output;
            output << std::setfill('0') << "0x";

            BOOST_FOREACH(cql_byte_t b, *_buffer) {
                output << std::setw(2) << cql::hex(b);
            }

            output << " {version: 0x" << std::setw(2) << cql::hex(_version);
            output << ", flags: 0x" << std::setw(2) << cql::hex(_flags);
            output << ", stream: 0x" << std::setw(2) << cql::hex(_stream);
            output << ", opcode: 0x" << std::setw(2) << cql::hex(_opcode);
            output << ", length: " << boost::lexical_cast<std::string>(_length) << "}";

            return output.str();
        }

        inline bool
        consume(cql::cql_error_t*)
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

        inline bool
        prepare(cql::cql_error_t*)
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

        inline cql_message_buffer_t
        buffer()
        {
            return _buffer;
        }

        inline cql_int_t
        size() const
        {
            return CQL_HEADER_SIZE;
        }

        inline cql_byte_t
        version() const
        {
            return _version;
        }

        inline cql_byte_t
        flags() const
        {
            return _flags;
        }

        inline cql_stream_id_t
        stream() const
        {
            return _stream;
        }

        inline cql_byte_t
        opcode() const
        {
            return _opcode;
        }

        inline cql_int_t
        length() const
        {
            return _length;
        }

        inline void
        version(
            cql_byte_t v)
        {
            _version = v;
        }

        inline void
        flags(
            cql_byte_t v)
        {
            _flags = v;
        }

        inline void
        stream(
            cql_stream_id_t v)
        {
            _stream = v;
        }

        inline void
        opcode(
            cql_byte_t v)
        {
            _opcode = v;
        }

        inline void
        length(
            cql_int_t v)
        {
            _length = v;
        }

    private:
        cql_message_buffer_t _buffer;
        cql_byte_t           _version;
        cql_byte_t           _flags;
        cql_stream_id_t      _stream;
        cql_byte_t           _opcode;
        cql_int_t            _length;
    };

} // namespace cql

#endif // CQL_HEADER_H_
