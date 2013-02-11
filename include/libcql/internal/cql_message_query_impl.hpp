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

#ifndef CQL_MESSAGE_QUERY_IMPL_H_
#define CQL_MESSAGE_QUERY_IMPL_H_

#include "libcql/cql.hpp"
#include "libcql/internal/cql_message.hpp"

namespace cql {

    class cql_message_query_impl_t :
        public cql_message_t
    {

    public:

        cql_message_query_impl_t();

        cql_message_query_impl_t(size_t size);

        cql_message_query_impl_t(const std::string& query,
                                 cql::cql_short_t consistency);

        const std::string&
        query() const;

        cql::cql_short_t
        consistency() const;

        void
        query(const std::string& q);

        void
        consistency(cql::cql_short_t consistency);

        cql::cql_opcode_enum
        opcode() const;

        cql_int_t
        size() const;

        std::string
        str() const;

        bool
        consume(cql::cql_error_t* err);

        bool
        prepare(cql::cql_error_t* err);

        cql_message_buffer_t
        buffer();

    private:
        cql::cql_message_buffer_t _buffer;
        cql::cql_short_t          _consistency;
        std::string               _query;
    };

} // namespace cql

#endif // CQL_MESSAGE_QUERY_IMPL_H_
