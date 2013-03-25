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

#ifndef CQL_MAP_IMPL_H_
#define CQL_MAP_IMPL_H_

#include <map>
#include <vector>
#include <boost/noncopyable.hpp>

#include "libcql/cql.hpp"
#include "libcql/cql_vector_stream.hpp"
#include "libcql/cql_serialization.hpp"

namespace cql {

    class cql_map_impl_t :
        boost::noncopyable
    {

    public:
        typedef std::vector<cql::cql_byte_t> column_t;

        cql_map_impl_t();

        void
        read(column_t column);

        std::string
        str() const;

        cql::cql_short_t
        key_type() const;

        const std::string&
        key_class() const;

        cql::cql_short_t
        value_type() const;

        const std::string&
        value_class() const;

        void
        get_map(std::map<std::string, std::string>& output) const;

        template<typename KeyT, typename ValueT, typename KeyDecodeFunction, typename ValueDecodeFunction>
        void
        decode_map(std::map<KeyT, ValueT>& output,
                   KeyDecodeFunction key_decoder,
                   ValueDecodeFunction value_decoder)
        {
            cql::vector_stream_t buffer(_column, _offset);
            std::istream stream(&buffer);

            cql::cql_short_t count = 0;
            cql::decode_short(stream, count);

            for (int i = 0; i < count; ++i) {
                KeyT key;
                ValueT value;

                key_decoder(stream, key);
                value_decoder(stream, key);
                output[key] = value;
            }
        }

    private:
        column_t                  _column;
        cql::cql_column_type_enum _key_type;
        cql::cql_column_type_enum _value_type;
        std::string               _key_class;
        std::string               _value_class;
        int                       _offset;
    };

} // namespace cql

#endif // CQL_MAP_IMPL_H_
