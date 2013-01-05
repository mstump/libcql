/*
  Copyright (c) 2012 Matthew Stump

  This file is part of libcql.

  libcql is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  libcql is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.	If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CQL_MESSAGE_RESULT_H_
#define CQL_MESSAGE_RESULT_H_

#include "cql.h"
#include "cql_message.hpp"
#include "cql_result_metadata.hpp"

namespace cql {

    class cql_message_result_t :
		public cql_message_t
    {

    public:
        cql_message_result_t();

        cql_int_t
        result_type() const;

        cql_byte_t
        opcode() const;

        cql_int_t
        size() const;

        std::string
        str() const;

        std::istream&
        read(std::istream& input);

        std::ostream&
        write(std::ostream& output) const;

    private:
        cql_int_t                            _result_type;
        cql_int_t                            _row_count;
        std::string                          _keyspace_name;
        std::string                          _table_name;
        cql::internal::cql_result_metadata_t _metadata;
    };

} // namespace cql

#endif // CQL_MESSAGE_RESULT_H_
