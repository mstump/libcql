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
#include "libcql/cql_serialization.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_map_impl.hpp"

cql::cql_map_impl_t::cql_map_impl_t(cql::cql_byte_t*          start,
                                    cql::cql_column_type_enum key_type,
                                    cql::cql_column_type_enum value_type,
                                    std::string&              key_custom_class,
                                    std::string&              value_custom_class) :
    _start(start),
    _key_type(key_type),
    _value_type(value_type),
    _key_custom_class(key_custom_class),
    _value_custom_class(value_custom_class)
{
    cql::cql_short_t count = 0;
    cql::cql_byte_t* _pos = cql::decode_short(_start, count);
    _keys.reserve(count);
    _values.reserve(count);

    for (int i = 0; i < count; ++i) {
        _keys.push_back(_pos);
        cql::cql_short_t len = 0;
        _pos = cql::decode_short(_pos, len);
        _pos += len;

        _values.push_back(_pos);
        _pos = cql::decode_short(_pos, len);
        _pos += len;
    }
}

bool
cql::cql_map_impl_t::get_key_bool(int i,
                                  bool& output) const
{
    if (i < 0 || i > _keys.size()) {
        return false;
    }

    output = *(_keys[i] + sizeof(cql_short_t)) != 0x00;
    return true;
}

bool
cql::cql_map_impl_t::get_key_int(int i,
                                 cql_int_t& output) const
{
    if (i < 0 || i > _keys.size()) {
        return false;
    }

    cql::decode_int(_keys[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_map_impl_t::get_key_float(int i,
                                   float& output) const
{
    if (i < 0 || i > _keys.size()) {
        return false;
    }

    cql::decode_float(_keys[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_map_impl_t::get_key_double(int i,
                                    double& output) const
{
    if (i < 0 || i > _keys.size()) {
        return false;
    }

    cql::decode_double(_keys[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_map_impl_t::get_key_bigint(int i,
                                    cql::cql_bigint_t& output) const
{
    if (i < 0 || i > _keys.size()) {
        return false;
    }

    cql::decode_bigint(_keys[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_map_impl_t::get_key_string(int i,
                                    std::string& output) const
{
    cql_byte_t* data = 0;
    cql_short_t size = 0;
    if (get_key_data(i, &data, size)) {
        output.assign(data, data + size);
        return true;
    }
    return false;
}

bool
cql::cql_map_impl_t::get_key_data(int i,
                                  cql::cql_byte_t** output,
                                  cql::cql_short_t& size) const
{
    if (i < 0 || i > _keys.size()) {
        return false;
    }

    *output = cql::decode_short(_keys[i], size);
    return true;
}

bool
cql::cql_map_impl_t::get_value_bool(int i,
                                  bool& output) const
{
    if (i < 0 || i > _values.size()) {
        return false;
    }

    output = *(_values[i] + sizeof(cql_short_t)) != 0x00;
    return true;
}

bool
cql::cql_map_impl_t::get_value_int(int i,
                                 cql_int_t& output) const
{
    if (i < 0 || i > _values.size()) {
        return false;
    }

    cql::decode_int(_values[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_map_impl_t::get_value_float(int i,
                                   float& output) const
{
    if (i < 0 || i > _values.size()) {
        return false;
    }

    cql::decode_float(_values[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_map_impl_t::get_value_double(int i,
                                    double& output) const
{
    if (i < 0 || i > _values.size()) {
        return false;
    }

    cql::decode_double(_values[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_map_impl_t::get_value_bigint(int i,
                                    cql::cql_bigint_t& output) const
{
    if (i < 0 || i > _values.size()) {
        return false;
    }

    cql::decode_bigint(_values[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_map_impl_t::get_value_string(int i,
                                    std::string& output) const
{
    cql_byte_t* data = 0;
    cql_short_t size = 0;
    if (get_value_data(i, &data, size)) {
        output.assign(data, data + size);
        return true;
    }
    return false;
}

bool
cql::cql_map_impl_t::get_value_data(int i,
                                  cql::cql_byte_t** output,
                                  cql::cql_short_t& size) const
{
    if (i < 0 || i > _values.size()) {
        return false;
    }

    *output = cql::decode_short(_values[i], size);
    return true;
}

std::string
cql::cql_map_impl_t::str() const
{
    return "map";
}

cql::cql_column_type_enum
cql::cql_map_impl_t::key_type() const
{
    return _key_type;
}

const std::string&
cql::cql_map_impl_t::key_custom_class() const
{
    return _key_custom_class;
}

cql::cql_column_type_enum
cql::cql_map_impl_t::value_type() const
{
    return _value_type;
}

const std::string&
cql::cql_map_impl_t::value_custom_class() const
{
    return _value_custom_class;
}

size_t
cql::cql_map_impl_t::size() const
{
    return _keys.size();
}
