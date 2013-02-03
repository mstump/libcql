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

#ifndef CQL_HEADER_H_
#define CQL_HEADER_H_

#include "libcql/cql.hpp"

namespace cql {
    namespace internal {

#pragma pack(push, 1)
        class cql_header_t {

        public:

            cql_header_t();

            cql_header_t(cql::cql_byte_t version,
                         cql::cql_byte_t flags,
                         cql::cql_stream_id_t stream,
                         cql::cql_byte_t opcode,
                         cql_int_t length);

            std::string
            str() const;

            std::ostream&
            write(std::ostream& output) const;

            std::istream&
            read(std::istream& input);

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
            cql::cql_byte_t      _version;
            cql::cql_byte_t      _flags;
            cql::cql_stream_id_t _stream;
            cql::cql_byte_t      _opcode;
            cql_int_t       _length;
        };
#pragma pack(pop)


    } // namespace internal
} // namespace cql

#endif // CQL_HEADER_H_
