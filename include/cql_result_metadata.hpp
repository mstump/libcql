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

#ifndef CQL_RESULT_METADATA_H_
#define CQL_RESULT_METADATA_H_

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/unordered_map.hpp>

#include "cql.hpp"

namespace cql {

    class cql_result_metadata_t :
        boost::noncopyable
    {

    public:
        typedef boost::tuple<std::string, std::string, std::string> column_name_t;

        cql_result_metadata_t();

        std::string
        str() const;

        std::istream&
        read(std::istream& input);

        cql_int_t
        flags() const;

        void
        flags(cql_int_t v);

        cql_int_t
        column_count() const;

        void
        column_count(cql_int_t v);

        bool
        has_global_keyspace() const;

        bool
        has_global_table() const;

        const std::string&
        global_keyspace() const;

        void
        global_keyspace(const std::string& keyspace);

        const std::string&
        global_table() const;

        void
        global_table(const std::string& table);

        bool
        exists(const std::string& column) const;

        bool
        exists(const std::string& keyspace,
               const std::string& table,
               const std::string& column) const;

        bool
        column_class(int i,
                     std::string& output) const;

        bool
        column_class(const std::string& column,
                     std::string& output) const;

        bool
        column_class(const std::string& keyspace,
                     const std::string& table,
                     const std::string& column,
                     std::string& output) const;

        bool
        column_type(int i,
                    cql::cql_short_t& output) const;

        bool
        column_type(const std::string& column,
                    cql::cql_short_t& output) const;

        bool
        column_type(const std::string& keyspace,
                    const std::string& table,
                    const std::string& column,
                    cql::cql_short_t& output) const;

        bool
        get_index(const std::string& column,
                  int& output) const;

        bool
        get_index(const std::string& keyspace,
                  const std::string& table,
                  const std::string& column,
                  int& output) const;


    private:

        struct option_t {
            cql::cql_short_t id;
            std::string value;
        };

        struct column_name_hash
            : std::unary_function<column_name_t, std::size_t>
        {
            std::size_t
            operator()(const column_name_t& n) const {
                std::size_t seed = 0;
                boost::hash_combine(seed, n.get<0>());
                boost::hash_combine(seed, n.get<1>());
                boost::hash_combine(seed, n.get<2>());
                return seed;
            }
        };

        struct column_name_equality
            : std::binary_function<column_name_t, column_name_t, bool>
        {
            std::size_t
            operator()(const column_name_t& a,
                       const column_name_t& b) const {

                return a.get<0>() == b.get<0>()
                    && a.get<1>() == b.get<1>()
                    && a.get<2>() == b.get<2>();
            }
        };

        typedef boost::unordered_map<column_name_t,
                                     int,
                                     column_name_hash,
                                     column_name_equality> column_name_idx_t;

        cql_int_t                   _flags;
        cql_int_t                   _column_count;
        std::string                 _global_keyspace_name;
        std::string                 _global_table_name;
        column_name_idx_t           _column_name_idx;
        boost::ptr_vector<option_t> _columns;
    };

} // namespace cql

#endif // CQL_RESULT_METADATA_H_
