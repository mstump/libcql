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

#ifndef CQL_MESSAGE_H_
#define CQL_MESSAGE_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "libcql/cql.hpp"

namespace cql {
    struct cql_error_t;

    typedef boost::shared_ptr<std::vector<cql::cql_byte_t> > cql_message_buffer_t;

    class cql_message_t {
    public:

        virtual cql::cql_opcode_enum
        opcode() const = 0;

        virtual cql_int_t
        size() const = 0;

        virtual std::string
        str() const = 0;

        virtual bool
        consume(cql::cql_error_t* err) = 0;

        virtual bool
        prepare(cql::cql_error_t* err) = 0;

        virtual cql_message_buffer_t
        buffer() = 0;

        virtual
        ~cql_message_t(){};
    };

} // namespace cql

#endif // CQL_MESSAGE_H_
