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

#ifndef CQL_MESSAGE_OPTIONS_H_
#define CQL_MESSAGE_OPTIONS_H_

#include "../include/cql.h"
#include "cql_message.hpp"

namespace cql {

class cql_message_options_t : public cql_message_t
{

public:

	cql_message_options_t()
	{}

	uint8_t
	opcode() const
	{
		return CQL_OPCODE_OPTIONS;
	}

	cql_int_t
	size() const
	{
		return 0;
	}

	std::string
	str() const
	{
		return "OPTIONS";
	}

	std::istream&
	read(std::istream& input)
	{
		return input;
	}

	std::ostream&
	write(std::ostream& output) const
	{
		return output;
	}
};

} // namespace cql

#endif // CQL_MESSAGE_OPTIONS_H_
