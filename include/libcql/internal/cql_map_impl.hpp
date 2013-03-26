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

#ifndef CQL_MAP_IMPL_H_
#define CQL_MAP_IMPL_H_

#include <vector>
#include "libcql/cql.hpp"
#include "libcql/cql_map.hpp"


namespace cql {

    class cql_map_impl_t :
        public cql_map_t
    {

    public:

        cql_map_impl_t(cql::cql_byte_t*          start,
                       cql::cql_column_type_enum key_type,
                       cql::cql_column_type_enum value_type,
                       std::string&              key_custom_class,
                       std::string&              value_custom_class);

        bool
        get_key_bool(int i,
                     bool& output) const;

        bool
        get_key_int(int i,
                    cql_int_t& output) const;

        bool
        get_key_float(int i,
                      float& output) const;

        bool
        get_key_double(int i,
                       double& output) const;

        bool
        get_key_bigint(int i,
                       cql::cql_bigint_t& output) const;

        bool
        get_key_string(int i,
                       std::string& output) const;

        bool
        get_key_data(int i,
                     cql::cql_byte_t** output,
                     cql::cql_short_t& size) const;

        bool
        get_value_bool(int i,
                       bool& output) const;

        bool
        get_value_int(int i,
                      cql_int_t& output) const;

        bool
        get_value_float(int i,
                        float& output) const;

        bool
        get_value_double(int i,
                         double& output) const;

        bool
        get_value_bigint(int i,
                         cql::cql_bigint_t& output) const;

        bool
        get_value_string(int i,
                         std::string& output) const;

        bool
        get_value_data(int i,
                       cql::cql_byte_t** output,
                       cql::cql_short_t& size) const;

        std::string
        str() const;

        cql::cql_column_type_enum
        key_type() const;

        const std::string&
        key_custom_class() const;

        cql::cql_column_type_enum
        value_type() const;

        const std::string&
        value_custom_class() const;

        size_t
        size() const;

    private:
        cql::cql_byte_t*              _start;
        std::vector<cql::cql_byte_t*> _keys;
        std::vector<cql::cql_byte_t*> _values;
        cql::cql_column_type_enum     _key_type;
        cql::cql_column_type_enum     _value_type;
        std::string                   _key_custom_class;
        std::string                   _value_custom_class;
    };


} // namespace cql

#endif // CQL_MAP_IMPL_H_
