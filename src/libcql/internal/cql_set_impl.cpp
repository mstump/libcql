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
#include "libcql/internal/cql_set_impl.hpp"

cql::cql_set_impl_t::cql_set_impl_t(cql::cql_byte_t*          start,
                                    cql::cql_column_type_enum element_type,
                                    std::string&              custom_class) :
    _start(start),
    _element_type(element_type),
    _custom_class(custom_class)
{
    cql::cql_short_t count = 0;
    cql::cql_byte_t* _pos = cql::decode_short(_start, count);
    _elements.reserve(count);

    for (int i = 0; i < count; ++i) {
        _elements.push_back(_pos);
        cql::cql_short_t len = 0;
        _pos = cql::decode_short(_pos, len);
        _pos += len;
    }
}

bool
cql::cql_set_impl_t::get_bool(size_t i,
                              bool& output) const
{
    if (i > _elements.size()) {
        return false;
    }

    output = *(_elements[i] + sizeof(cql_short_t)) != 0x00;
    return true;
}

bool
cql::cql_set_impl_t::get_int(size_t i,
                             cql_int_t& output) const
{
    if (i > _elements.size()) {
        return false;
    }

    cql::decode_int(_elements[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_set_impl_t::get_float(size_t i,
                               float& output) const
{
    if (i > _elements.size()) {
        return false;
    }

    cql::decode_float(_elements[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_set_impl_t::get_double(size_t i,
                                double& output) const
{
    if (i > _elements.size()) {
        return false;
    }

    cql::decode_double(_elements[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_set_impl_t::get_bigint(size_t i,
                                cql::cql_bigint_t& output) const
{
    if (i > _elements.size()) {
        return false;
    }

    cql::decode_bigint(_elements[i] + sizeof(cql_short_t), output);
    return true;
}

bool
cql::cql_set_impl_t::get_string(size_t i,
                                std::string& output) const
{
    cql_byte_t* data = 0;
    cql_short_t size = 0;
    if (get_data(i, &data, size)) {
        output.assign(data, data + size);
        return true;
    }
    return false;
}

bool
cql::cql_set_impl_t::get_data(size_t i,
                              cql::cql_byte_t** output,
                              cql::cql_short_t& size) const
{
    if (i > _elements.size()) {
        return false;
    }

    *output = cql::decode_short(_elements[i], size);
    return true;
}

std::string
cql::cql_set_impl_t::str() const
{
    return "set";
}

cql::cql_column_type_enum
cql::cql_set_impl_t::element_type() const
{
    return _element_type;
}

const std::string&
cql::cql_set_impl_t::custom_class() const
{
    return _custom_class;
}

size_t
cql::cql_set_impl_t::size() const
{
    return _elements.size();
}
