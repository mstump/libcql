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

#ifndef CQL_MESSAGE_ERROR_IMPL_H_
#define CQL_MESSAGE_ERROR_IMPL_H_

#include "libcql/cql.hpp"
#include "libcql/internal/cql_message.hpp"

namespace cql {

    class cql_message_error_impl_t :
        public cql_message_t
    {

    public:

        cql_message_error_impl_t();

        cql_message_error_impl_t(cql_int_t code,
                                 const std::string& message);

        const std::string&
        message() const;

        void
        message(const std::string& m);

        cql_int_t
        code() const;

        void
        code(cql_int_t c);

        cql::cql_opcode_enum
        opcode() const;

        std::string
        str() const;

        bool
        consume(cql::cql_error_t* err);

        bool
        prepare(cql::cql_error_t* err);

        cql_message_buffer_t&
        buffer();

    private:
        cql_int_t                 _code;
        std::string               _message;
    };

} // namespace cql

#endif // CQL_MESSAGE_ERROR_IMPL_H_
