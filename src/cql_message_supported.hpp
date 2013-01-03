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

#ifndef CQL_MESSAGE_SUPPORTED_H_
#define CQL_MESSAGE_SUPPORTED_H_

#include <sstream>

#include "../include/cql.h"
#include "cql_message.hpp"

#include <boost/algorithm/string/join.hpp>

namespace cql {

class cql_message_supported_t :
		public cql_message_t
{

public:

	cql_message_supported_t() :
		_versions(),
		_compressions()
	{}

	void
	compressions(const std::list<std::string>& c)
	{
		_compressions = c;
	}

	const std::list<std::string>&
	compressions() const
	{
		return _compressions;
	}

	void
	versions(const std::list<std::string>& v)
	{
		_versions = v;
	}

	const std::list<std::string>&
	version() const
	{
		return _versions;
	}

	cql_byte_t
	opcode() const
	{
		return CQL_OPCODE_SUPPORTED;
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
		output << "{versions: [" << boost::algorithm::join(_versions, ", ");
		output << "], compressions: [" << boost::algorithm::join(_compressions, ", ") << "]}";
        return output.str();
	}

	std::istream&
	read(std::istream& input)
	{
		std::map<std::string, std::list<std::string> > supported;
		cql::internal::decode_string_multimap(input, supported);

		if (supported.find(CQL_VERSION) != supported.end())
			_versions = supported[CQL_VERSION];

		if (supported.find(CQL_COMPRESSION) != supported.end())
			_compressions = supported[CQL_COMPRESSION];

		return input;
	}

	std::ostream&
	write(std::ostream& output) const
	{
		std::map<std::string, std::list<std::string> > supported;

		if (!_versions.empty())
			supported.insert(std::pair<std::string, std::list<std::string> >(CQL_VERSION, _versions));

		if (!_compressions.empty())
			supported.insert(std::pair<std::string, std::list<std::string> >(CQL_COMPRESSION, _compressions));

		cql::internal::encode_string_multimap(output, supported);
		return output;
	}

private:

	std::list<std::string> _versions;
	std::list<std::string> _compressions;
};

} // namespace cql

#endif // CQL_MESSAGE_SUPPORTED_H_
