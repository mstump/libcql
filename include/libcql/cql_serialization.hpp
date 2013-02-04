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
#include <list>
#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "libcql/cql.hpp"

namespace cql {

    class cql_inet_t;

    std::ostream&
    encode_bool(std::ostream& output,
                bool value);

    void
    encode_bool(std::vector<cql::cql_byte_t>& output,
                const bool value);

    std::istream&
    decode_bool(std::istream& input,
                bool& value);


    std::ostream&
    encode_short(std::ostream& output,
                 cql::cql_short_t value);

    void
    encode_short(std::vector<cql::cql_byte_t>& output,
                 const cql::cql_short_t value);

    std::istream&
    decode_short(std::istream& input,
                 cql::cql_short_t& value);

    cql::cql_short_t
    decode_short(const std::vector<cql::cql_byte_t>& input);

    std::ostream&
    encode_int(std::ostream& output,
               const cql_int_t value);

    void
    encode_int(std::vector<cql::cql_byte_t>& output,
               const cql_int_t value);

    std::istream&
    decode_int(std::istream& input,
               cql_int_t& value);

    cql_int_t
    decode_int(const std::vector<cql::cql_byte_t>& input);

    std::ostream&
    encode_double(std::ostream& output,
                  const double value);

    void
    encode_double(std::vector<cql::cql_byte_t>& output,
                  const double value);

    std::istream&
    decode_double(std::istream& input,
                  double& value);

    std::ostream&
    encode_float(std::ostream& output,
                 const float value);

    void
    encode_float(std::vector<cql::cql_byte_t>& output,
                 const float value);

    std::istream&
    decode_float(std::istream& input,
                 float& value);

    float
    decode_float(const std::vector<cql::cql_byte_t>& input);

    std::ostream&
    encode_double(std::ostream& output,
                  const double value);

    void
    encode_double(std::vector<cql::cql_byte_t>& output,
                  const double value);

    std::istream&
    decode_double(std::istream& input,
                  double& value);

    double
    decode_double(const std::vector<cql::cql_byte_t>& input);

    std::ostream&
    encode_bigint(std::ostream& output,
                  const cql::cql_bigint_t value);

    void
    encode_bigint(std::vector<cql::cql_byte_t>& output,
                  const cql::cql_bigint_t value);

    std::istream&
    decode_bigint(std::istream& input,
                  cql::cql_bigint_t& value);

    cql::cql_bigint_t
    decode_bigint(const std::vector<cql::cql_byte_t>& input);


    std::ostream&
    encode_string(std::ostream& output,
                  const std::string& value);

    std::istream&
    decode_string(std::istream& input,
                  std::string& value);

    std::ostream&
    encode_bytes(std::ostream& output,
                 const std::vector<cql::cql_byte_t>& value);

    std::istream&
    decode_bytes(std::istream& input,
                 std::vector<cql::cql_byte_t>& value);

    std::ostream&
    encode_short_bytes(std::ostream& output,
                       const std::vector<cql::cql_byte_t>& value);

    std::istream&
    decode_short_bytes(std::istream& input,
                       std::vector<cql::cql_byte_t>& value);

    std::ostream&
    encode_long_string(std::ostream& output,
                       const std::string& value);

    std::istream&
    decode_long_string(std::istream& input,
                       std::string& value);

    std::ostream&
    encode_string_list(std::ostream& output,
                       const std::list<std::string>& list);

    std::istream&
    decode_string_list(std::istream& input,
                       std::list<std::string>& list);

    std::ostream&
    encode_string_map(std::ostream& output,
                      const std::map<std::string, std::string>& map);

    std::istream&
    decode_string_map(std::istream& input,
                      std::map<std::string, std::string>& map);

    std::ostream&
    encode_string_multimap(std::ostream& output,
                           const std::map<std::string, std::list<std::string> >& map);

    std::istream&
    decode_string_multimap(std::istream& input,
                           std::map<std::string, std::list<std::string> >& map);

    std::ostream&
    encode_option(std::ostream& output,
                  cql::cql_short_t id,
                  const std::string& value);

    std::istream&
    decode_option(std::istream& input,
                  cql::cql_short_t& id,
                  std::string& value);

    std::ostream&
    encode_inet(std::ostream& output,
                const std::string& ip,
                const cql_int_t port);

    std::istream&
    decode_inet(std::istream& input,
                std::string& ip,
                cql_int_t& port);

    std::ostream&
    encode_inet(std::ostream& output,
                const cql::cql_inet_t& inet);

    std::istream&
    decode_inet(std::istream& input,
                cql::cql_inet_t& inet);

} // namespace cql

#endif // CQL_SERIALIZATION_H_
