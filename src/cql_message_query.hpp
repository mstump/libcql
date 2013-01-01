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

#ifndef CQL_MESSAGE_QUERY_H_
#define CQL_MESSAGE_QUERY_H_

#include <sstream>

#include "../include/cql.h"
#include "cql_message.hpp"

namespace cql {

class cql_message_query_t :
		public cql_message_t
{

public:

	cql_message_query_t() :
		_consistency(0),
		_query()
	{}

	cql_message_query_t(const std::string& query,
						int32_t consistency) :
		_consistency(consistency),
		_query(query)
	{}

	const std::string&
	query() const
	{
		return _query;
	}

	int32_t
	consistency() const
	{
        return _consistency;
	}

	void
	query(const std::string& q)
	{
		_query = q;
	}

	void
	consistency(int32_t consistency)
	{
        _consistency = consistency;
	}

	uint8_t
	opcode() const
	{
		return CQL_OPCODE_QUERY;
	}

	uint32_t
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
		output << _query;
        output << " " << std::setfill('0') << std::string("0x") << std::setw(2);
		output << cql::internal::hex(_consistency);
		return output.str();
	}

	std::istream&
	read(std::istream& input)
	{
        cql::internal::decode_long_string(input, _query);
		input.read(reinterpret_cast<char*>(&_consistency), sizeof(_consistency));
		_consistency = ntohl(_consistency);
		return input;
	}

	std::ostream&
	write(std::ostream& output) const
	{
        cql::internal::encode_long_string(output, _query);
		int32_t consistency = htonl(_consistency);
		output.write(reinterpret_cast<char*>(&consistency), sizeof(consistency));
		return output;
	}

private:
	int16_t		 _consistency;
	std::string	 _query;
};

} // namespace cql

#endif // CQL_MESSAGE_QUERY_H_
