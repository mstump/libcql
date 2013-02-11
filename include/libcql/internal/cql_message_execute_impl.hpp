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

#ifndef CQL_MESSAGE_EXECUTE_IMPL_H_
#define CQL_MESSAGE_EXECUTE_IMPL_H_

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "libcql/cql.hpp"
#include "libcql/internal/cql_message.hpp"

namespace cql {

    class cql_message_execute_impl_t :
        boost::noncopyable,
        public cql_message_t
    {

    public:
        typedef std::vector<cql::cql_byte_t> param_t;

        cql_message_execute_impl_t();

        cql_message_execute_impl_t(size_t size);

        cql_message_execute_impl_t(const std::vector<cql::cql_byte_t>& id,
                                   cql::cql_consistency_enum consistency);

        const std::vector<cql::cql_byte_t>&
        query_id() const;

        void
        query_id(const std::vector<cql::cql_byte_t>& id);

        cql::cql_consistency_enum
        consistency() const;

        void
        consistency(const cql::cql_consistency_enum consistency);

        void
        push_back(const param_t& val);

        void
        push_back(const std::string& val);

        void
        push_back(const cql::cql_short_t val);

        void
        push_back(const cql_int_t val);

        void
        push_back(const cql::cql_bigint_t val);

        void
        push_back(const float val);

        void
        push_back(const double val);

        void
        push_back(const bool val);

        void
        pop_back();

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
        typedef std::list<param_t> params_container_t;

        cql::cql_message_buffer_t    _buffer;
        std::vector<cql::cql_byte_t> _query_id;
        cql::cql_consistency_enum    _consistency;
        params_container_t           _params;
    };

} // namespace cql

#endif // CQL_MESSAGE_EXECUTE_IMPL_H_
