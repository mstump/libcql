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

#ifndef CQL_MESSAGE_ERROR_H_
#define CQL_MESSAGE_ERROR_H_

#include <sstream>

#include "../include/cql.h"
#include "cql_message.hpp"

namespace cql {

class cql_message_error_t :
		public cql_message_t
{

public:

	cql_message_error_t() :
		_code(0),
		_message()
	{}

	const std::string&
	message() const
	{
		return _message;
	}

	int32_t
	code() const
	{
        return _code;
	}

	uint8_t
	opcode() const
	{
		return CQL_OPCODE_ERROR;
	}

	uint32_t
	size() const
	{
		std::stringstream ss(std::stringstream::out);
		write(ss);
		ss.seekg(0, std::ios::end);
		return ss.tellg();
	}

	std::string
	str() const
	{
		return _message;
	}

	std::istream&
	read(std::istream& input)
	{
		input.read(reinterpret_cast<char*>(&_code), sizeof(_code));
		_code = ntohl(_code);
        cql::internal::decode_string(input, _message);
		return input;
	}

	std::ostream&
	write(std::ostream& output) const
	{
		int32_t code = htonl(_code);
		output.write(reinterpret_cast<char*>(&code), sizeof(code));
        cql::internal::encode_string(output, _message);
		return output;
	}

private:
	int32_t		 _code;
	std::string	 _message;
};

} // namespace cql

#endif // CQL_MESSAGE_ERROR_H_
