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

#ifndef CQL_LIST_H_
#define CQL_LIST_H_

#include <list>
#include <vector>
#include <boost/noncopyable.hpp>

#include "libcql/cql.hpp"

namespace cql {

    class cql_list_t :
        boost::noncopyable
    {

    public:
        typedef std::vector<cql::cql_byte_t>& column_t;

        cql_list_t(column_t column);

        std::string
        str() const;

        cql::cql_short_t
        member_type() const;

        void
        get_list(std::list<bool>& output) const;

        void
        get_list(std::list<cql_int_t>& output) const;

        void
        get_list(std::list<float>& output) const;

        void
        get_list(std::list<double>& output) const;

        void
        get_list(std::list<cql::cql_bigint_t>& output) const;

        void
        get_list(std::list<std::string>& output) const;

    private:
        column_t _column;
        cql::cql_short_t _type;
    };

} // namespace cql

#endif // CQL_LIST_H_
