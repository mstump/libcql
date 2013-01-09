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

#include <vector>
#include <boost/foreach.hpp>
#include <boost/detail/endian.hpp>
#include "serialization.hpp"

inline double
swap_double(double source)
{
#if defined _WIN32
    return byteswap_uint64(source);
#else
    return __builtin_bswap64(source);
#endif
}

inline double
ntohd(double source)
{
#if defined(BOOST_LITTLE_ENDIAN)
    return swap_double(source);
#elif defined(BOOST_BIG_ENDIAN)
    return source;
#else
#error "unable to determine system endianness"
#endif
}

inline double
htond(double source)
{
    return ntohd(source);
}

std::ostream&
cql::internal::encode_short(std::ostream& output,
							cql_short_t value)
{
	cql_int_t s = htons(value);
	output.write(reinterpret_cast<char*>(&s), sizeof(s));
	return output;
}

std::istream&
cql::internal::decode_short(std::istream& input,
							cql_short_t& value)
{
	input.read(reinterpret_cast<char*>(&value), sizeof(value));
    value = ntohs(value);
	return input;
}

std::ostream&
cql::internal::encode_int(std::ostream& output,
						  cql_int_t value)
{
	cql_int_t l = htonl(value);
	output.write(reinterpret_cast<char*>(&l), sizeof(l));
	return output;
}

std::istream&
cql::internal::decode_int(std::istream& input,
						  cql_int_t& value)
{
	input.read(reinterpret_cast<char*>(&value), sizeof(value));
    value = ntohl(value);
	return input;
}

cql_int_t
cql::internal::decode_int(const std::vector<cql_byte_t>& input)
{
    return ntohl(*(reinterpret_cast<const float*>(&input[0])));
}

std::ostream&
cql::internal::encode_float(std::ostream& output,
                            float value)
{
	cql_int_t l = htonl(value);
	output.write(reinterpret_cast<char*>(&l), sizeof(l));
	return output;
}

std::istream&
cql::internal::decode_float(std::istream& input,
                            float& value)
{
	input.read(reinterpret_cast<char*>(&value), sizeof(value));
    value = ntohl(value);
	return input;
}

float
cql::internal::decode_float(const std::vector<cql_byte_t>& input)
{
    return ntohl(*(reinterpret_cast<const float*>(&input[0])));
}

std::ostream&
cql::internal::encode_double(std::ostream& output,
                             double value)
{
	double d = htond(value);
	output.write(reinterpret_cast<char*>(&d), sizeof(d));
	return output;
}

std::istream&
cql::internal::decode_double(std::istream& input,
                             double& value)
{
	input.read(reinterpret_cast<char*>(&value), sizeof(value));
    value = ntohd(value);
	return input;
}

double
cql::internal::decode_double(const std::vector<cql_byte_t>& input)
{
    return ntohd(*(reinterpret_cast<const double*>(&input[0])));
}

std::ostream&
cql::internal::encode_string(std::ostream& output,
							 const std::string& value)
{
	cql_short_t len = htons(value.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));
	output.write(reinterpret_cast<const char*>(value.c_str()), value.size());
	return output;
}

std::istream&
cql::internal::decode_string(std::istream& input,
							 std::string& value)
{
	cql_short_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohs(len);

	std::vector<char> buffer(len, 0);
	input.read(&buffer[0], len);
	value.assign(buffer.begin(), buffer.end());
	return input;
}


std::ostream&
cql::internal::encode_bytes(std::ostream& output,
                            const std::vector<cql_byte_t>& value)
{
	cql_int_t len = htonl(value.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));
	output.write(reinterpret_cast<const char*>(&value[0]), value.size());
    return output;
}

std::istream&
cql::internal::decode_bytes(std::istream& input,
                            std::vector<cql_byte_t>& value)
{
	cql_int_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohl(len);

    value.resize(len, 0);
	input.read(reinterpret_cast<char*>(&value[0]), len);
    return input;
}

std::ostream&
cql::internal::encode_long_string(std::ostream& output,
								  const std::string& value)
{
	cql_int_t len = htonl(value.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));
	output.write(reinterpret_cast<const char*>(value.c_str()), value.size());
	return output;
}

std::istream&
cql::internal::decode_long_string(std::istream& input,
								  std::string& value)
{
	cql_int_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohl(len);

	std::vector<char> buffer(len);
	input.read(&buffer[0], len);
	value.assign(buffer.begin(), buffer.end());
	return input;
}

std::ostream&
cql::internal::encode_string_list(std::ostream& output,
								  const std::list<std::string>& list)
{
	cql_short_t len = htons(list.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));
	BOOST_FOREACH(const std::string& s, list)
		cql::internal::encode_string(output, s);
	return output;
}

std::istream&
cql::internal::decode_string_list(std::istream& input,
								  std::list<std::string>& list)
{
	cql_short_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohs(len);

	list.clear();
	for (int i = 0; i < len; i++)
	{
		std::string s;
		cql::internal::decode_string(input, s);
		list.push_back(s);
	}

	return input;
}

std::ostream&
cql::internal::encode_string_map(std::ostream& output,
				  const std::map<std::string, std::string>& map)
{
	cql_short_t len = htons(map.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));

	std::map<std::string, std::string>::const_iterator it = map.begin();
	for (; it != map.end(); it++)
	{
		cql::internal::encode_string(output, (*it).first);
		cql::internal::encode_string(output, (*it).second);
	}

	return output;
}

std::istream&
cql::internal::decode_string_map(std::istream& input,
								 std::map<std::string, std::string>& map)
{
	cql_short_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohs(len);

	map.clear();
	for (int i = 0; i < len; i++)
	{
		std::string key;
		std::string value;
		cql::internal::decode_string(input, key);
		cql::internal::decode_string(input, value);
		map.insert(std::pair<std::string, std::string>(key, value));
	}

	return input;
}

std::ostream&
cql::internal::encode_string_multimap(std::ostream& output,
									  const std::map<std::string, std::list<std::string> >& map)
{
	cql_short_t len = htons(map.size());
	output.write(reinterpret_cast<char*>(&len), sizeof(len));

	std::map<std::string, std::list<std::string> >::const_iterator it = map.begin();
	for (; it != map.end(); it++)
	{
		cql::internal::encode_string(output, (*it).first);
		cql::internal::encode_string_list(output, (*it).second);
	}

	return output;
}

std::istream&
cql::internal::decode_string_multimap(std::istream& input,
									  std::map<std::string, std::list<std::string> >& map)
{
	cql_short_t len;
	input.read(reinterpret_cast<char*>(&len), sizeof(len));
	len = ntohs(len);

	map.clear();
	for (int i = 0; i < len; i++)
	{
		std::string key;
		cql::internal::decode_string(input, key);

		std::list<std::string> values;
		cql::internal::decode_string_list(input, values);
		map.insert(std::pair<std::string, std::list<std::string> >(key, values));
	}

	return input;
}


std::ostream&
cql::internal::encode_option(std::ostream& output,
                             cql_short_t id,
                             const std::string& value)
{
    cql::internal::encode_short(output, id);
	if (id == CQL_COLUMN_TYPE_CUSTOM)
		cql::internal::encode_string(output, value);
	return output;
}

std::istream&
cql::internal::decode_option(std::istream& input,
                             cql_short_t& id,
                             std::string& value)
{
    cql::internal::decode_short(input, id);
	if (id == CQL_COLUMN_TYPE_CUSTOM)
		cql::internal::decode_string(input, value);
	return input;
}
