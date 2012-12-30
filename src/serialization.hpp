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

#ifndef CQL_SERIALIZATION_H_
#define CQL_SERIALIZATION_H_

#include <istream>
#include <ostream>

#include <list>
#include <string>
#include <vector>

namespace cql {
namespace internal {

std::ostream&
encode_string(std::ostream& output,
			  const std::string& value)
{
	uint16_t len = htons(value.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));
	output.write(reinterpret_cast<const char*>(value.c_str()), value.size());
	return output;
}

std::istream&
decode_string(std::istream& input,
			  std::string& value)
{
	uint16_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohs(len);

	std::vector<char> buffer(len);
	input.read(&buffer[0], len);
	value.assign(buffer.begin(), buffer.end());
	return input;
}

std::ostream&
encode_string_list(std::ostream& output,
				   const std::list<std::string>& list)
{
	uint16_t len = htons(list.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));
	BOOST_FOREACH(const std::string& s, list)
		encode_string(output, s);
	return output;
}

std::istream&
decode_string_list(std::istream& input,
				   std::list<std::string>& list)
{
	uint16_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohs(len);

	list.clear();
	for (int i = 0; i < len; i++)
	{
		std::string s;
		decode_string(input, s);
		list.push_back(s);
	}

	return input;
}

std::ostream&
encode_string_map(std::ostream& output,
				  const std::map<std::string, std::string>& map)
{
	uint16_t len = htons(map.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));

	std::map<std::string, std::string>::const_iterator it = map.begin();
	for (; it != map.end(); it++)
	{
		encode_string(output, (*it).first);
		encode_string(output, (*it).second);
	}

	return output;
}

std::istream&
decode_string_map(std::istream& input,
				  std::map<std::string, std::string>& map)
{
	uint16_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohs(len);

	map.clear();
	for (int i = 0; i < len; i++)
	{
		std::string key;
		std::string value;
		decode_string(input, key);
		decode_string(input, value);
		map.insert(std::pair<std::string, std::string>(key, value));
	}

	return input;
}

std::ostream&
encode_string_multimap(std::ostream& output,
					   const std::map<std::string, std::list<std::string> >& map)
{
	uint16_t len = htons(map.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));

	std::map<std::string, std::list<std::string> >::const_iterator it = map.begin();
	for (; it != map.end(); it++)
	{
		encode_string(output, (*it).first);
		encode_string_list(output, (*it).second);
	}

	return output;
}

std::istream&
decode_string_multimap(std::istream& input,
					   std::map<std::string, std::list<std::string> >& map)
{
	uint16_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohs(len);

	map.clear();
	for (int i = 0; i < len; i++)
	{
		std::string key;
		decode_string(input, key);

		std::list<std::string> values;
		decode_string_list(input, values);
		map.insert(std::pair<std::string, std::list<std::string> >(key, values));
	}

	return input;
}


} // namespace internal
} // namespace cql

#endif // CQL_SERIALIZATION_H_
