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

#ifndef CQL_MAP_H_
#define CQL_MAP_H_

#include "libcql/cql.hpp"

namespace cql {

    class cql_map_t
    {

    public:
        virtual
        ~cql_map_t(){};

        virtual bool
        get_key_bool(int i,
                     bool& output) const = 0;

        virtual bool
        get_key_int(int i,
                    cql_int_t& output) const = 0;

        virtual bool
        get_key_float(int i,
                      float& output) const = 0;

        virtual bool
        get_key_double(int i,
                       double& output) const = 0;

        virtual bool
        get_key_bigint(int i,
                       cql::cql_bigint_t& output) const = 0;

        virtual bool
        get_key_string(int i,
                       std::string& output) const = 0;

        virtual bool
        get_key_data(int i,
                     cql::cql_byte_t** output,
                     cql::cql_short_t& size) const = 0;

        virtual bool
        get_value_bool(int i,
                       bool& output) const = 0;

        virtual bool
        get_value_int(int i,
                      cql_int_t& output) const = 0;

        virtual bool
        get_value_float(int i,
                        float& output) const = 0;

        virtual bool
        get_value_double(int i,
                         double& output) const = 0;

        virtual bool
        get_value_bigint(int i,
                         cql::cql_bigint_t& output) const = 0;

        virtual bool
        get_value_string(int i,
                         std::string& output) const = 0;

        virtual bool
        get_value_data(int i,
                       cql::cql_byte_t** output,
                       cql::cql_short_t& size) const = 0;

        virtual std::string
        str() const = 0;

        virtual cql::cql_column_type_enum
        key_type() const = 0;

        virtual const std::string&
        key_custom_class() const = 0;

        virtual cql::cql_column_type_enum
        value_type() const = 0;

        virtual const std::string&
        value_custom_class() const = 0;

        virtual size_t
        size() const = 0;

    };


} // namespace cql

#endif // CQL_MAP_H_
