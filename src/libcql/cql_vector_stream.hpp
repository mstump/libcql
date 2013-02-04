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

#ifndef CQL_VECTOR_STREAM_H_
#define CQL_VECTOR_STREAM_H_

#include <streambuf>
#include <vector>
#include "libcql/cql.hpp"

namespace cql {

        struct vector_stream_t : std::streambuf
        {
            vector_stream_t(std::vector<cql::cql_byte_t>& vec)
            {
                char* start = reinterpret_cast<char*>(&vec[0]);
                this->setg(start, start, start + vec.size());
            }

            vector_stream_t(std::vector<cql::cql_byte_t>& vec,
                            size_t offset)
            {
                char* start = reinterpret_cast<char*>(&vec[0]);
                this->setg(start, start + offset, start + vec.size());
            }
        };
} // namespace cql

#endif // CQL_VECTOR_STREAM_H_
