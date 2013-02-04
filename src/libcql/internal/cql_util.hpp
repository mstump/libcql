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

#ifndef CQL_UTIL_H_
#define CQL_UTIL_H_

#include <ostream>
#include <streambuf>
#include <vector>
#include "libcql/cql.hpp"

namespace cql {
    namespace internal {

        struct HexCharStruct
        {
            unsigned char c;
            HexCharStruct(unsigned char _c) : c(_c) { }
        };

        inline std::ostream&
        operator<<(std::ostream& o, const HexCharStruct& hs)
        {
            return (o << std::hex << (int)hs.c);
        }

        inline
        HexCharStruct hex(unsigned char _c)
        {
            return HexCharStruct(_c);
        }

        inline std::string
        get_consistency_string(const cql::cql_short_t consistency)
        {
            switch (consistency)
            {
            case CQL_CONSISTENCY_ANY:
                return "CQL_CONSISTENCY_ANY";
                break;
            case CQL_CONSISTENCY_ONE:
                return "CQL_CONSISTENCY_ONE";
                break;
            case CQL_CONSISTENCY_TWO:
                return "CQL_CONSISTENCY_TWO";
                break;
            case CQL_CONSISTENCY_THREE:
                return "CQL_CONSISTENCY_THREE";
                break;
            case CQL_CONSISTENCY_QUORUM:
                return "CQL_CONSISTENCY_QUORUM";
                break;
            case CQL_CONSISTENCY_ALL:
                return "CQL_CONSISTENCY_ALL";
                break;
            case CQL_CONSISTENCY_LOCAL_QUORUM:
                return "CQL_CONSISTENCY_LOCAL_QUORUM";
                break;
            case CQL_CONSISTENCY_EACH_QUORUM:
                return "CQL_CONSISTENCY_EACH_QUORUM";
                break;
            default:
                return "UNKNOWN";
            }
        }

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

    } // namespace internal
} // namespace cql

#endif // CQL_UTIL_H_
