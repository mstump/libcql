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

#ifndef CQL_HEADER_IMPL_H_
#define CQL_HEADER_IMPL_H_

#include "libcql/cql.hpp"
#include "libcql/internal/cql_message.hpp"

namespace cql {

    class cql_error_t;

    class cql_header_impl_t {

    public:

        cql_header_impl_t();

        cql_header_impl_t(cql::cql_byte_t version,
                          cql::cql_byte_t flags,
                          cql::cql_stream_id_t stream,
                          cql::cql_byte_t opcode,
                          cql_int_t length);

        std::string
        str() const;

        bool
        consume(cql::cql_error_t* err);

        bool
        prepare(cql::cql_error_t* err);

        cql_message_buffer_t
        buffer();

        cql_int_t
        size() const;

        cql::cql_byte_t
        version() const;

        cql::cql_byte_t
        flags() const;

        cql::cql_stream_id_t
        stream() const;

        cql::cql_byte_t
        opcode() const;

        cql_int_t
        length() const;

        void
        version(cql::cql_byte_t v);

        void
        flags(cql::cql_byte_t v);

        void
        stream(cql::cql_stream_id_t v);

        void
        opcode(cql::cql_byte_t v);

        void
        length(cql_int_t v);

    private:
        cql_message_buffer_t _buffer;
        cql::cql_byte_t      _version;
        cql::cql_byte_t      _flags;
        cql::cql_stream_id_t _stream;
        cql::cql_byte_t      _opcode;
        cql::cql_int_t       _length;
    };

} // namespace cql

#endif // CQL_HEADER_IMPL_H_
