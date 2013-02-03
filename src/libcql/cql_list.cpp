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

#include <boost/function.hpp>

#include "libcql/cql.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_serialization.hpp"
#include "libcql/internal/cql_util.hpp"

#include "libcql/cql_list.hpp"

template<typename T, typename DecodeFunction>
void
decode_list(cql::cql_list_t::column_t column,
            std::list<T>& output,
            DecodeFunction decoder)
{
    cql::internal::vector_stream_t buffer(column, sizeof(cql::cql_short_t));
    std::istream stream(&buffer);

    cql::cql_short_t count = 0;
    cql::internal::decode_short(stream, count);
    output.resize(count);

    for (int i = 0; i < count; ++i) {
        T elem;
        decoder(stream, elem);
        output.push_back(elem);
    }
}


cql::cql_list_t::cql_list_t(cql::cql_list_t::column_t column) :
    _column(column),
    _type(cql::internal::decode_short(column))
{}

std::string
cql::cql_list_t::str() const
{
    return "list";
}

cql::cql_short_t
cql::cql_list_t::member_type() const
{
    return _type;
}

void
cql::cql_list_t::get_list(std::list<bool>& output) const
{
    decode_list(_column, output, &cql::internal::decode_bool);
}

void
cql::cql_list_t::get_list(std::list<cql::cql_int_t>& output) const
{
    decode_list(_column,
                output,
                (std::istream& (&) (std::istream&, int&)) &cql::internal::decode_int);
}

void
cql::cql_list_t::get_list(std::list<float>& output) const
{
    decode_list(_column,
                output,
                (std::istream& (&) (std::istream&, float&)) &cql::internal::decode_float);
}

void
cql::cql_list_t::get_list(std::list<double>& output) const
{
    decode_list(_column,
                output,
                (std::istream& (&) (std::istream&, double&)) &cql::internal::decode_double);
}

void
cql::cql_list_t::get_list(std::list<cql::cql_bigint_t>& output) const
{
    decode_list(_column,
                output,
                (std::istream& (&) (std::istream&, cql::cql_bigint_t&)) &cql::internal::decode_bigint);
}

void
cql::cql_list_t::get_list(std::list<std::string>& output) const
{
    decode_list(_column, output, &cql::internal::decode_string);
}
