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
#include "libcql/cql_serialization.hpp"
#include "libcql/internal/cql_util.hpp"

#include "libcql/cql_set.hpp"

template<typename T, typename DecodeFunction>
void
decode_set(cql::cql_set_t::column_t column,
           int offset,
           std::set<T>& output,
           DecodeFunction decoder)
{
    cql::internal::vector_stream_t buffer(column, offset);
    std::istream stream(&buffer);

    cql::cql_short_t count = 0;
    cql::decode_short(stream, count);

    for (int i = 0; i < count; ++i) {
        T elem;
        decoder(stream, elem);
        output.insert(elem);
    }
}


cql::cql_set_t::cql_set_t(cql::cql_set_t::column_t column) :
    _column(column),
    _type(-1),
    _offset(0)
{
    cql::internal::vector_stream_t buffer(column, sizeof(cql::cql_short_t));
    std::istream stream(&buffer);

    cql::decode_option(stream, _type, _custom_class);
    _offset = stream.tellg();
}

std::string
cql::cql_set_t::str() const
{
    return "set";
}

cql::cql_short_t
cql::cql_set_t::member_type() const
{
    return _type;
}

const std::string&
cql::cql_set_t::custom_class() const
{
    return _custom_class;
}

void
cql::cql_set_t::get_set(std::set<bool>& output) const
{
    decode_set(_column, _offset, output, &cql::decode_bool);
}

void
cql::cql_set_t::get_set(std::set<cql::cql_int_t>& output) const
{
    decode_set(_column,
               _offset,
               output,
               (std::istream& (&) (std::istream&, int&)) &cql::decode_int);
}

void
cql::cql_set_t::get_set(std::set<float>& output) const
{
    decode_set(_column,
               _offset,
               output,
               (std::istream& (&) (std::istream&, float&)) &cql::decode_float);
}

void
cql::cql_set_t::get_set(std::set<double>& output) const
{
    decode_set(_column,
               _offset,
               output,
               (std::istream& (&) (std::istream&, double&)) &cql::decode_double);
}

void
cql::cql_set_t::get_set(std::set<cql::cql_bigint_t>& output) const
{
    decode_set(_column,
               _offset,
               output,
               (std::istream& (&) (std::istream&, cql::cql_bigint_t&)) &cql::decode_bigint);
}

void
cql::cql_set_t::get_set(std::set<std::string>& output) const
{
    decode_set(_column, _offset, output, &cql::decode_string);
}
