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

#include <sstream>

#include "../include/cql.h"
#include "cql_message.hpp"
#include "serialization.hpp"
#include "util.hpp"

namespace cql {

class cql_message_result_t :
		public cql_message_t
{

public:

	cql_message_result_t() :
		_result_type(0)
	{}

	cql_int_t
	result_type() const
	{
        return _result_type;
	}

	cql_byte_t
	opcode() const
	{
		return CQL_OPCODE_RESULT;
	}

	cql_int_t
	size() const
	{
		std::stringstream ss(std::stringstream::out);
		write(ss);
		return ss.str().size();
	}

	std::string
	str() const
	{
		std::stringstream output;
		output << std::setfill('0');
        output << std::string("RESULT 0x") << std::setw(2) << cql::internal::hex(_result_type);
		output << " " << _keyspace_name;
		output << " " << _table_name;
		return output.str();
	}

	std::istream&
	read(std::istream& input)
	{
		_keyspace_name.clear();
		_row_count = 0;

		input.read(reinterpret_cast<char*>(&_result_type), sizeof(_result_type));
		_result_type = ntohl(_result_type);
		switch(_result_type) {

		case CQL_RESULT_ROWS:
			read_metadata(input);
			break;

		case CQL_RESULT_SET_KEYSPACE:
			cql::internal::decode_string(input, _keyspace_name);
			break;

		case CQL_RESULT_VOID:
			break;

		}

		return input;
	}

	std::ostream&
	write(std::ostream& output) const
	{
		int32_t result_type = htonl(_result_type);
		output.write(reinterpret_cast<char*>(&result_type), sizeof(result_type));
		return output;
	}

private:

	inline std::istream&
	read_metadata(std::istream& input)
	{
		input.read(reinterpret_cast<char*>(&_row_flags), sizeof(_row_flags));
		_row_flags = ntohl(_row_flags);

		input.read(reinterpret_cast<char*>(&_column_count), sizeof(_column_count));
		_column_count = ntohl(_column_count);

		if (_row_flags & CQL_RESULT_ROWS_FLAGS_GLOBAL_TABLES_SPEC)
		{
			cql::internal::decode_string(input, _keyspace_name);
			cql::internal::decode_string(input, _table_name);
		}
		return input;
	}


	int32_t		 _result_type;
	int32_t		 _row_count;
	int32_t		 _row_flags;
	int32_t		 _column_count;
    std::string  _keyspace_name;
    std::string  _table_name;
};

} // namespace cql

#endif // CQL_MESSAGE_RESULT_H_
