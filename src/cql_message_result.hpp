/*
  Copyright (c) 2013 Matthew Stump

  This file is part of libcql.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef CQL_MESSAGE_RESULT_H_
#define CQL_MESSAGE_RESULT_H_

#include <vector>
#include <boost/multi_array.hpp>
#include <boost/asio/buffer.hpp>

#include "libcql/cql.h"
#include "cql_message.hpp"
#include "cql_result_metadata.hpp"
#include "cql_serialization.hpp"

namespace cql {

    class cql_list_t;
    class cql_map_t;
    class cql_set_t;

    class cql_message_result_t :
        boost::noncopyable,
        public cql_message_t
    {

    public:
        cql_message_result_t(
            size_t size);

        cql_message_result_t();

        cql_int_t
        result_type() const;

        cql_byte_t
        opcode() const;

        cql_int_t
        size() const;

        std::string
        str() const;

        inline size_t
        column_count() const
        {
            return _values.shape()[1];
        }

        inline size_t
        row_count() const
        {
            return _values.shape()[0];
        }

        const std::vector<cql_byte_t>&
        query_id() const;

        bool
        consume(
            cql::cql_error_t* err);

        bool
        prepare(
            cql::cql_error_t* err);

        cql_message_buffer_t
        buffer();

        const cql_result_metadata_t&
        get_metadata();

        inline bool
        exists(
            const std::string& column) const
        {
            return _metadata.exists(column);
        }

        inline bool
        get_index(
            const std::string& column,
            size_t&            output) const
        {
            return _metadata.get_index(column, reinterpret_cast<int&>(output));
        }

        inline bool
        column_class(
            size_t       column,
            std::string& output) const
        {
            return _metadata.column_class(column, output);
        }

        inline bool
        column_type(
            size_t     column,
            cql_int_t& output) const
        {
            return _metadata.column_type(column, output);
        }

        inline bool
        is_null(
            size_t row,
            size_t column,
            bool&  output) const
        {
            if (row > row_count() || column > column_count()) {
                return false;
            }

            cql_int_t size = 0;
            cql::decode_int(_values[row][column], size);
            output = size <= 0;
            return true;
        }

        bool
        get_bool(
            size_t row,
            size_t column,
            bool&  output) const;

        bool
        get_int(
            size_t      row,
            size_t     column,
            cql_int_t& output) const;

        bool
        get_float(
            size_t row,
            size_t column,
            float& output) const;

        bool
        get_double(
            size_t  row,
            size_t  column,
            double& output) const;

        bool
        get_bigint(
            size_t        row,
            size_t        column,
            cql_bigint_t& output) const;

        bool
        get_string(
            size_t       row,
            size_t       column,
            std::string& output) const;

        bool
        get_data(
            size_t       row,
            size_t       column,
            cql_byte_t** output,
            cql_int_t&   size) const;

        bool
        get_list(
            size_t            row,
            size_t            column,
            cql::cql_list_t** output) const;

        bool
        get_set(
            size_t           row,
            size_t           column,
            cql::cql_set_t** output) const;

        bool
        get_map(
            size_t           row,
            size_t           column,
            cql::cql_map_t** output) const;

        inline bool
        is_valid(
            size_t    row,
            size_t    column,
            cql_int_t column_type) const
        {
            bool index_null = false;
            if (!is_null(row, column, index_null) || index_null) {
                return false;
            }

            cql_int_t t = CQL_COLUMN_TYPE_UNKNOWN;
            if (!_metadata.column_type(column, t) || column_type != t) {
                return false;
            }

            return (*reinterpret_cast<cql_int_t*>(_values[row][column]) != 0);
        }


    private:
        typedef boost::multi_array<cql_byte_t*, 2> value_array_t;

        cql::cql_message_buffer_t                  _buffer;
        value_array_t                              _values;
        std::vector<cql_byte_t>                    _query_id;
        cql_int_t                                  _result_type;
        std::string                                _keyspace_name;
        std::string                                _table_name;
        cql::cql_result_metadata_t                 _metadata;
    };

} // namespace cql

#endif // CQL_MESSAGE_RESULT_H_
