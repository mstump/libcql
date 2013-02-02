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

#ifndef CQL_ROW_H_
#define CQL_ROW_H_

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "cql.hpp"
#include "cql_result_metadata.hpp"

namespace cql {

    class cql_row_t :
        boost::noncopyable
    {

    public:
        typedef  std::vector<cql::cql_byte_t>             column_t;
        typedef  boost::ptr_vector<column_t>         columns_container_t;
        typedef  column_t*                           value_type;
        typedef  column_t&                           reference;
        typedef  const column_t&                     const_reference;
        typedef  columns_container_t::iterator       iterator;
        typedef  columns_container_t::const_iterator const_iterator;
        typedef  columns_container_t::size_type      size_type;

        cql_row_t(const cql::cql_result_metadata_t& metadata,
                  std::istream& input);

        cql_int_t
        size() const;

        std::string
        str() const;

        const column_t&
        operator[](size_type n) const;

        const column_t&
        at(size_type n) const;

        const_iterator
        begin() const;

        const_iterator
        end() const;

        bool
        is_null(int i,
                bool& output) const;

        bool
        is_null(const std::string& column,
                bool& output) const;

        bool
        is_null(const std::string& keyspace,
                const std::string& table,
                const std::string& column,
                bool& output) const;

        bool
        get_bool(int i,
                 bool& output) const;

        bool
        get_bool(const std::string& column,
                 bool& output) const;

        bool
        get_bool(const std::string& keyspace,
                 const std::string& table,
                 const std::string& column,
                 bool& output) const;

        bool
        get_int(int i,
                cql_int_t& output) const;

        bool
        get_int(const std::string& column,
                cql_int_t& output) const;

        bool
        get_int(const std::string& keyspace,
                const std::string& table,
                const std::string& column,
                cql_int_t& output) const;

        bool
        get_float(int i,
                  float& output) const;

        bool
        get_float(const std::string& column,
                  float& output) const;

        bool
        get_float(const std::string& keyspace,
                  const std::string& table,
                  const std::string& column,
                  float& output) const;

        bool
        get_double(int i,
                   double& output) const;

        bool
        get_double(const std::string& column,
                   double& output) const;

        bool
        get_double(const std::string& keyspace,
                   const std::string& table,
                   const std::string& column,
                   double& output) const;

        bool
        get_bigint(int i,
                   cql::cql_bigint_t& output) const;

        bool
        get_bigint(const std::string& column,
                   cql::cql_bigint_t& output) const;

        bool
        get_bigint(const std::string& keyspace,
                   const std::string& table,
                   const std::string& column,
                   cql::cql_bigint_t& output) const;

        bool
        get_string(int i,
                   std::string& output) const;

        bool
        get_string(const std::string& column,
                   std::string& output) const;

        bool
        get_string(const std::string& keyspace,
                   const std::string& table,
                   const std::string& column,
                   std::string& output) const;

    private:
        const cql::cql_result_metadata_t& _metadata;
        columns_container_t               _row;
    };

} // namespace cql

#endif // CQL_ROW_H_
