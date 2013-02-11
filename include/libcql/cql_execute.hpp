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

#ifndef CQL_EXECUTE_H_
#define CQL_EXECUTE_H_

#include <vector>
#include <boost/noncopyable.hpp>
#include "libcql/cql.hpp"

namespace cql {

    class cql_message_execute_impl_t;

    class cql_execute_t :
        boost::noncopyable
    {

    public:

        cql_execute_t();

        ~cql_execute_t();

        cql_execute_t(const std::vector<cql::cql_byte_t>& id,
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
        push_back(const std::vector<cql::cql_byte_t>& val);

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

        cql_message_execute_impl_t*
        impl() const;

    private:
        cql_message_execute_impl_t* _impl;
    };

} // namespace cql

#endif // CQL_EXECUTE_H_
