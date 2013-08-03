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

#ifndef CQL_MESSAGE_EXECUTE_H_
#define CQL_MESSAGE_EXECUTE_H_

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "libcql/cql.h"
#include "cql_message.hpp"

namespace cql {

    class cql_message_execute_t :
        boost::noncopyable,
        public cql_message_t
    {

    public:
        typedef std::vector<cql_byte_t> param_t;

        cql_message_execute_t();

        cql_message_execute_t(
            size_t size);

        cql_message_execute_t(
            const cql_byte_t* id,
            size_t            id_size);

        cql_message_execute_t(
            const std::vector<cql_byte_t>& id,
            cql_short_t consistency);

        const std::vector<cql_byte_t>&
        query_id() const;

        void
        query_id(
            const std::vector<cql_byte_t>& id);

        cql_short_t
        consistency() const;

        void
        consistency(
            const cql_short_t consistency);

        void
        push_back(
            const param_t& val);

        void
        push_back(
            const std::string& val);

        void
        push_back(
            const cql_short_t val);

        void
        push_back(
            const cql_int_t val);

        void
        push_back(
            const cql_bigint_t val);

        void
        push_back(
            const float val);

        void
        push_back(
            const double val);

        void
        push_back(
            const bool val);

        void
        push_back(
            const cql_byte_t* data,
            size_t            data_size);

        void
        pop_back();

        cql_byte_t
        opcode() const;

        cql_int_t
        size() const;

        std::string
        str() const;

        bool
        consume(
            cql::cql_error_t* err);

        bool
        prepare(
            cql::cql_error_t* err);

        cql_message_buffer_t
        buffer();

    private:
        typedef std::list<param_t> params_container_t;

        cql::cql_message_buffer_t _buffer;
        std::vector<cql_byte_t>   _query_id;
        cql_short_t               _consistency;
        params_container_t        _params;
    };

} // namespace cql

#endif // CQL_MESSAGE_EXECUTE_H_
