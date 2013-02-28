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

#include <vector>
#include <boost/foreach.hpp>
#include <boost/detail/endian.hpp>
#include "libcql/internal/cql_defines.hpp"


#include "libcql/cql_serialization.hpp"

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
cql::encode_bool(std::ostream& output,
                 bool value)
{
    if (value) {
        output.put(0x01);
    }
    else {
        output.put(0x00);
    }
    return output;
}

void
cql::encode_bool(std::vector<cql::cql_byte_t>& output,
                 const bool value)
{
    output.resize(1, 0);
    if (value) {
        output[0] = 0x01;
    }
    else {
        output[0] = 0x00;
    }
}

std::istream&
cql::decode_bool(std::istream& input,
                 bool& value)
{
    std::vector<char> v(1, 0);
    input.read(reinterpret_cast<char*>(&v), 1);
    value = v[0] == 0x01;
    return input;
}

bool
cql::decode_bool(const std::vector<cql::cql_byte_t>& input)
{
    return input[0];
}

std::ostream&
cql::encode_short(std::ostream& output,
                  const cql::cql_short_t value)
{
    cql::cql_short_t s = htons(value);
    output.write(reinterpret_cast<char*>(&s), sizeof(s));
    return output;
}

void
cql::encode_short(std::vector<cql::cql_byte_t>& output,
                  const cql::cql_short_t value)
{
    cql::cql_short_t s = htons(value);
    output.resize(sizeof(s));
    output.assign(&s, &s + sizeof(s));
}

std::istream&
cql::decode_short(std::istream& input,
                  cql::cql_short_t& value)
{
    input.read(reinterpret_cast<char*>(&value), sizeof(value));
    value = ntohs(value);
    return input;
}

cql::cql_short_t
cql::decode_short(const std::vector<cql::cql_byte_t>& input)
{
    return ntohs(*(reinterpret_cast<const cql_short_t*>(&input[0])));
}

cql::cql_byte_t*
cql::decode_short(cql::cql_byte_t* input,
                  cql::cql_short_t& value)
{
    value = ntohs(*(reinterpret_cast<const cql_short_t*>(input)));
    return input + sizeof(cql::cql_short_t);
}

std::ostream&
cql::encode_int(std::ostream& output,
                const cql::cql_int_t value)
{
    cql::cql_int_t l = htonl(value);
    output.write(reinterpret_cast<char*>(&l), sizeof(l));
    return output;
}

void
cql::encode_int(std::vector<cql::cql_byte_t>& output,
                const cql::cql_int_t value)
{
    cql::cql_int_t l = htonl(value);
    output.resize(sizeof(l));
    output.assign(&l, &l + sizeof(l));
}

std::istream&
cql::decode_int(std::istream& input,
                cql::cql_int_t& value)
{
    input.read(reinterpret_cast<char*>(&value), sizeof(value));
    value = ntohl(value);
    return input;
}

cql::cql_int_t
cql::decode_int(const std::vector<cql::cql_byte_t>& input)
{
    return ntohl(*(reinterpret_cast<const cql::cql_int_t*>(&input[0])));
}

cql::cql_byte_t*
cql::decode_int(cql::cql_byte_t* input,
                cql::cql_int_t& output)
{
    output = ntohl(*(reinterpret_cast<const cql::cql_int_t*>(input)));
    return input + sizeof(cql::cql_int_t);
}

std::ostream&
cql::encode_float(std::ostream& output,
                  const float value)
{
    cql::cql_int_t l = htonl(value);
    output.write(reinterpret_cast<char*>(&l), sizeof(l));
    return output;
}

void
cql::encode_float(std::vector<cql::cql_byte_t>& output,
                  const float value)
{
    cql::cql_int_t l = htonl(value);
    output.resize(sizeof(l));
    output.assign(&l, &l + sizeof(l));
}

std::istream&
cql::decode_float(std::istream& input,
                  float& value)
{
    input.read(reinterpret_cast<char*>(&value), sizeof(value));
    value = ntohl(value);
    return input;
}

float
cql::decode_float(const std::vector<cql::cql_byte_t>& input)
{
    return ntohl(*(reinterpret_cast<const float*>(&input[0])));
}

cql::cql_byte_t*
cql::decode_float(cql::cql_byte_t* input,
                  float& output)
{
    output = ntohl(*(reinterpret_cast<const float*>(input)));
    return input + sizeof(float);
}

std::ostream&
cql::encode_double(std::ostream& output,
                   const double value)
{
    double d = htond(value);
    output.write(reinterpret_cast<char*>(&d), sizeof(d));
    return output;
}

void
cql::encode_double(std::vector<cql::cql_byte_t>& output,
                   const double value)
{
    double d = ntohd(value);
    output.resize(sizeof(d));
    output.assign(&d, &d + sizeof(d));
}

std::istream&
cql::decode_double(std::istream& input,
                   double& value)
{
    input.read(reinterpret_cast<char*>(&value), sizeof(value));
    value = ntohd(value);
    return input;
}

double
cql::decode_double(const std::vector<cql::cql_byte_t>& input)
{
    return ntohd(*(reinterpret_cast<const double*>(&input[0])));
}

cql::cql_byte_t*
cql::decode_double(cql::cql_byte_t* input,
                   double& output)
{
    output = ntohl(*(reinterpret_cast<const double*>(input)));
    return input + sizeof(double);
}

std::ostream&
cql::encode_bigint(std::ostream& output,
                   const cql::cql_bigint_t value)
{
    cql::cql_bigint_t d = htond(value);
    output.write(reinterpret_cast<char*>(&d), sizeof(d));
    return output;
}

void
cql::encode_bigint(std::vector<cql::cql_byte_t>& output,
                   const cql::cql_bigint_t value)
{
    double d = ntohd(value);
    output.resize(sizeof(d));
    output.assign(&d, &d + sizeof(d));
}

std::istream&
cql::decode_bigint(std::istream& input,
                   cql::cql_bigint_t& value)
{
    input.read(reinterpret_cast<char*>(&value), sizeof(value));
    value = ntohd(value);
    return input;
}

cql::cql_bigint_t
cql::decode_bigint(const std::vector<cql::cql_byte_t>& input)
{
    return ntohd(*(reinterpret_cast<const cql::cql_bigint_t*>(&input[0])));
}

cql::cql_byte_t*
cql::decode_bigint(cql::cql_byte_t* input,
                   cql::cql_bigint_t& output)
{
    output = ntohl(*(reinterpret_cast<const cql::cql_bigint_t*>(input)));
    return input + sizeof(cql::cql_bigint_t);
}

std::ostream&
cql::encode_string(std::ostream& output,
                   const std::string& value)
{
    cql::cql_short_t len = htons(value.size());
    output.write(reinterpret_cast<char*>(&len), sizeof(len));
    output.write(reinterpret_cast<const char*>(value.c_str()), value.size());
    return output;
}

std::istream&
cql::decode_string(std::istream& input,
                   std::string& value)
{
    cql::cql_short_t len;
    cql::decode_short(input, len);

    std::vector<char> buffer(len, 0);
    input.read(&buffer[0], len);
    value.assign(buffer.begin(), buffer.end());
    return input;
}

std::string
cql::decode_string(const std::vector<cql::cql_byte_t>& input)
{
    return std::string(input.begin(), input.end());
}

cql::cql_byte_t*
cql::decode_string(cql::cql_byte_t* input,
                   std::string& value)
{
    cql::cql_short_t len;
    input = cql::decode_short(input, len);
    value.assign(input, input + len);
    return input + len;
}

std::ostream&
cql::encode_bytes(std::ostream& output,
                  const std::vector<cql::cql_byte_t>& value)
{
    cql::cql_int_t len = htonl(value.size());
    output.write(reinterpret_cast<char*>(&len), sizeof(len));
    output.write(reinterpret_cast<const char*>(&value[0]), value.size());
    return output;
}

std::istream&
cql::decode_bytes(std::istream& input,
                  std::vector<cql::cql_byte_t>& value)
{
    cql::cql_int_t len;
    input.read(reinterpret_cast<char*>(&len), sizeof(len));
    len = ntohl(len);

    value.resize(len, 0);
    input.read(reinterpret_cast<char*>(&value[0]), len);
    return input;
}

std::ostream&
cql::encode_short_bytes(std::ostream& output,
                        const std::vector<cql::cql_byte_t>& value)
{
    cql::cql_short_t len = htons(value.size());
    output.write(reinterpret_cast<char*>(&len), sizeof(len));
    output.write(reinterpret_cast<const char*>(&value[0]), value.size());
    return output;
}

std::istream&
cql::decode_short_bytes(std::istream& input,
                        std::vector<cql::cql_byte_t>& value)
{
    cql::cql_short_t len;
    input.read(reinterpret_cast<char*>(&len), sizeof(len));
    len = ntohs(len);

    value.resize(len, 0);
    input.read(reinterpret_cast<char*>(&value[0]), len);
    return input;
}

cql::cql_byte_t*
cql::decode_short_bytes(cql::cql_byte_t* input,
                        std::vector<cql::cql_byte_t>& value)
{
    cql::cql_short_t len;
    input = cql::decode_short(input, len);

    value.resize(len, 0);
    value.assign(input, input + len);
    return input;
}

std::ostream&
cql::encode_long_string(std::ostream& output,
                        const std::string& value)
{
    cql::cql_int_t len = htonl(value.size());
    output.write(reinterpret_cast<char*>(&len), sizeof(len));
    output.write(reinterpret_cast<const char*>(value.c_str()), value.size());
    return output;
}

std::istream&
cql::decode_long_string(std::istream& input,
                        std::string& value)
{
    cql::cql_int_t len;
    input.read(reinterpret_cast<char*>(&len), sizeof(len));
    len = ntohl(len);

    std::vector<char> buffer(len);
    input.read(&buffer[0], len);
    value.assign(buffer.begin(), buffer.end());
    return input;
}

std::ostream&
cql::encode_string_list(std::ostream& output,
                        const std::list<std::string>& list)
{
    cql::cql_short_t len = htons(list.size());
    output.write(reinterpret_cast<char*>(&len), sizeof(len));
    BOOST_FOREACH(const std::string& s, list) {
        cql::encode_string(output, s);
    }
    return output;
}

std::istream&
cql::decode_string_list(std::istream& input,
                        std::list<std::string>& list)
{
    cql::cql_short_t len;
    input.read(reinterpret_cast<char*>(&len), sizeof(len));
    len = ntohs(len);

    list.clear();
    for (int i = 0; i < len; i++) {
        std::string s;
        cql::decode_string(input, s);
        list.push_back(s);
    }

    return input;
}

std::ostream&
cql::encode_string_map(std::ostream& output,
                       const std::map<std::string, std::string>& map)
{
    cql::cql_short_t len = htons(map.size());
    output.write(reinterpret_cast<char*>(&len), sizeof(len));

    std::map<std::string, std::string>::const_iterator it = map.begin();
    for (; it != map.end(); it++) {
        cql::encode_string(output, (*it).first);
        cql::encode_string(output, (*it).second);
    }

    return output;
}

std::istream&
cql::decode_string_map(std::istream& input,
                       std::map<std::string, std::string>& map)
{
    cql::cql_short_t len;
    input.read(reinterpret_cast<char*>(&len), sizeof(len));
    len = ntohs(len);

    map.clear();
    for (int i = 0; i < len; i++) {
        std::string key;
        std::string value;
        cql::decode_string(input, key);
        cql::decode_string(input, value);
        map.insert(std::pair<std::string, std::string>(key, value));
    }

    return input;
}

std::ostream&
cql::encode_string_multimap(std::ostream& output,
                            const std::map<std::string, std::list<std::string> >& map)
{
    cql::cql_short_t len = htons(map.size());
    output.write(reinterpret_cast<char*>(&len), sizeof(len));

    std::map<std::string, std::list<std::string> >::const_iterator it = map.begin();
    for (; it != map.end(); it++) {
        cql::encode_string(output, (*it).first);
        cql::encode_string_list(output, (*it).second);
    }

    return output;
}

std::istream&
cql::decode_string_multimap(std::istream& input,
                            std::map<std::string, std::list<std::string> >& map)
{
    cql::cql_short_t len;
    input.read(reinterpret_cast<char*>(&len), sizeof(len));
    len = ntohs(len);

    map.clear();
    for (int i = 0; i < len; i++) {
        std::string key;
        cql::decode_string(input, key);

        std::list<std::string> values;
        cql::decode_string_list(input, values);
        map.insert(std::pair<std::string, std::list<std::string> >(key, values));
    }

    return input;
}


std::ostream&
cql::encode_option(std::ostream& output,
                   cql::cql_short_t id,
                   const std::string& value)
{
    cql::encode_short(output, id);
    if (id == CQL_COLUMN_TYPE_CUSTOM) {
        cql::encode_string(output, value);
    }
    return output;
}

std::istream&
cql::decode_option(std::istream& input,
                   cql::cql_short_t& id,
                   std::string& value)
{
    cql::decode_short(input, id);
    if (id == CQL_COLUMN_TYPE_CUSTOM) {
        cql::decode_string(input, value);
    }
    return input;
}

cql::cql_byte_t*
cql::decode_option(cql::cql_byte_t* input,
                   cql::cql_short_t& id,
                   std::string& value)
{
    input = cql::decode_short(input, id);
    if (id == CQL_COLUMN_TYPE_CUSTOM) {
        input = cql::decode_string(input, value);
    }
    return input;
}

std::ostream&
cql::encode_inet(std::ostream& output,
                 const std::string& ip,
                 const cql::cql_int_t port)
{
    cql::encode_string(output, ip);
    cql::encode_int(output, port);
    return output;
}

std::istream&
cql::decode_inet(std::istream& input,
                 std::string& ip,
                 cql::cql_int_t& port)
{
    cql::decode_string(input, ip);
    cql::decode_int(input, port);
    return input;
}
