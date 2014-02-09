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

#ifndef CQL_MESSAGE_H_
#define CQL_MESSAGE_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "libcql/cql.hpp"

namespace cql {
    struct cql_error_t;

    typedef std::vector<cql::cql_byte_t> cql_message_buffer_t;

    class cql_message_t {
    public:

        cql_message_t() {}
        cql_message_t(size_t size) : _buffer(size) {}

        virtual cql::cql_opcode_enum
        opcode() const = 0;

        virtual cql_int_t
        size() const
        {
          return _buffer.size();
        }

        virtual std::string
        str() const = 0;

        virtual bool
        consume(cql::cql_error_t* err) = 0;

        virtual bool
        prepare(cql::cql_error_t* err) = 0;

        cql_message_buffer_t&
        buffer()
        {
          return _buffer;
        }

        virtual
        ~cql_message_t(){};
    protected:
        cql::cql_message_buffer_t _buffer;

    };

} // namespace cql

#endif // CQL_MESSAGE_H_
