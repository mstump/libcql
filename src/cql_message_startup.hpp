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

#ifndef CQL_MESSAGE_STARTUP_H_
#define CQL_MESSAGE_STARTUP_H_

#include <sstream>

#include "../include/cql.h"
#include "cql_message.hpp"

#include <boost/algorithm/string/join.hpp>

namespace cql {

class cql_message_startup_t :
		public cql_message_t
{

public:

	cql_message_startup_t() :
		_version(),
		_compression()
	{}

	void
	compression(const std::string& c)
	{
		_compression = c;
	}

	const std::string&
	compression() const
	{
		return _compression;
	}

	void
	version(const std::string& v)
	{
		_version = v;
	}

	const std::string&
	version() const
	{
		return _version;
	}

	uint8_t
	opcode() const
	{
		return CQL_OPCODE_STARTUP;
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
		output << "{version: " << _version << ", compression: " << _compression << "}";
        return output.str();
	}

	std::istream&
	read(std::istream& input)
	{
		std::map<std::string, std::string> startup;
		cql::internal::decode_string_map(input, startup);

		if (startup.find(CQL_VERSION) != startup.end())
			_version = startup[CQL_VERSION];

		if (startup.find(CQL_COMPRESSION) != startup.end())
			_compression = startup[CQL_COMPRESSION];

		return input;
	}

	std::ostream&
	write(std::ostream& output) const
	{
		std::map<std::string, std::string> startup;

		if (!_version.empty())
			startup.insert(std::pair<std::string, std::string>(CQL_VERSION, _version));

		if (!_compression.empty())
			startup.insert(std::pair<std::string, std::string>(CQL_COMPRESSION, _compression));

		cql::internal::encode_string_map(output, startup);
		return output;
	}

private:

	std::string _version;
	std::string _compression;
};

} // namespace cql

#endif // CQL_MESSAGE_STARTUP_H_
