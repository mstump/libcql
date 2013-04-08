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

#include "libcql/cql.hpp"

namespace cql {

    class cql_list_t
    {

    public:
        virtual
        ~cql_list_t(){};

        virtual std::string
        str() const = 0;

        virtual cql::cql_column_type_enum
        element_type() const = 0;

        virtual const std::string&
        custom_class() const = 0;

        virtual bool
        get_bool(size_t i,
                 bool& output) const = 0;

        virtual bool
        get_int(size_t i,
                cql_int_t& output) const = 0;

        virtual bool
        get_float(size_t i,
                  float& output) const = 0;

        virtual bool
        get_double(size_t i,
                   double& output) const = 0;

        virtual bool
        get_bigint(size_t i,
                   cql::cql_bigint_t& output) const = 0;

        virtual bool
        get_string(size_t i,
                   std::string& output) const = 0;

        virtual bool
        get_data(size_t i,
                 cql::cql_byte_t** output,
                 cql::cql_short_t& size) const = 0;

        virtual size_t
        size() const = 0;

    };

} // namespace cql

#endif // CQL_LIST_H_
