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

#include "libcql/internal/cql_map_impl.hpp"

cql::cql_map_impl_t::cql_map_impl_t() :
    _key_type(0),
    _value_type(0),
    _offset(0)
{}

void
cql::cql_map_impl_t::read(column_t column)
{
    _column = column;
    cql::vector_stream_t buffer(_column, sizeof(cql::cql_short_t));
    std::istream stream(&buffer);

    cql::decode_option(stream, _key_type, _key_class);
    cql::decode_option(stream, _value_type, _value_class);
    _offset = stream.tellg();
}

std::string
cql::cql_map_impl_t::str() const
{
    return "map";
}

cql::cql_short_t
cql::cql_map_impl_t::key_type() const
{
    return _key_type;
}

const std::string&
cql::cql_map_impl_t::key_class() const
{
    return _key_class;
}

cql::cql_short_t
cql::cql_map_impl_t::value_type() const
{
    return _value_type;
}

const std::string&
cql::cql_map_impl_t::value_class() const
{
    return _value_class;
}

void
cql::cql_map_impl_t::get_map(std::map<std::string, std::string>& output) const
{
    decode_map(output,
               (std::istream& (&) (std::istream&, std::string&)) &cql::decode_string,
               (std::istream& (&) (std::istream&, std::string&)) &cql::decode_string);
}
