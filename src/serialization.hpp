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

#include "cql.h"

namespace cql {
    namespace internal {


        std::ostream&
        encode_short(std::ostream& output,
                     cql_short_t value);

        std::istream&
        decode_short(std::istream& input,
                     cql_short_t& value);

        std::ostream&
        encode_int(std::ostream& output,
                   cql_int_t value);

        std::istream&
        decode_int(std::istream& input,
                   cql_int_t& value);

        std::ostream&
        encode_string(std::ostream& output,
                      const std::string& value);

        std::istream&
        decode_string(std::istream& input,
                      std::string& value);

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
                      cql_short_t id,
                      const std::string& value);

        std::istream&
        decode_option(std::istream& input,
                      cql_short_t& id,
                      std::string& value);

    } // namespace internal
} // namespace cql

#endif // CQL_SERIALIZATION_H_
