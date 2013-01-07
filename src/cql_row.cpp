/*
  Copyright (c) 2012 Matthew Stump

  This file is part of libcql.

  libcql is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  libcql is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.	If not, see <http://www.gnu.org/licenses/>.
*/

#include "serialization.hpp"

#include "cql_row.hpp"

cql::cql_row_t::cql_row_t(const cql::cql_result_metadata_t& metadata,
                          std::istream& input) :
    _metadata(metadata),
    _row(_metadata.column_count())
{
    for (int i = 0; i < _metadata.column_count(); ++i)
    {
        std::auto_ptr<cql::cql_row_t::column> c( new cql::cql_row_t::column() );
        cql::internal::decode_bytes(input, *c);
        _row.push_back(c);
    }
}

cql_int_t
cql::cql_row_t::size() const
{
    return _row.size();
}

std::string
cql::cql_row_t::str() const
{
    return "row";
}

const cql::cql_row_t::column&
cql::cql_row_t::operator[](size_type n) const
{
    return _row[n];
}

const cql::cql_row_t::column&
cql::cql_row_t::at(size_type n) const
{
    return _row.at(n);
}

cql::cql_row_t::const_iterator
cql::cql_row_t::begin() const
{
    return _row.begin();
}

cql::cql_row_t::const_iterator
cql::cql_row_t::end() const
{
    return _row.end();
}

bool
cql::cql_row_t::is_null(int i,
                        bool& output) const
{
    if (i > _row.size() || i < 0)
        return false;

    output = _row[i].empty();
    return true;
}

bool
cql::cql_row_t::is_null(const std::string& column,
                        bool& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        output = _row[i].empty();
        return true;
    }
    return false;
}

bool
cql::cql_row_t::is_null(const std::string& keyspace,
                        const std::string& table,
                        const std::string& column,
                        bool& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        output = _row[i].empty();
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_bool(int i,
                         bool& output) const
{
    // if (i > _row.size() || i < 0)
    //     return false;

    // output = _row[i].empty();
    // return true;
    return false;
}

bool
cql::cql_row_t::get_bool(const std::string& column,
                         bool& output) const
{
    return false;
}

bool
cql::cql_row_t::get_bool(const std::string& keyspace,
                         const std::string& table,
                         const std::string& column,
                         bool& output) const
{
    return false;
}

bool
cql::cql_row_t::get_int(int i,
                        cql_int_t& output) const
{
    return false;
}

bool
cql::cql_row_t::get_int(const std::string& column,
                        cql_int_t& output) const
{
    return false;
}

bool
cql::cql_row_t::get_int(const std::string& keyspace,
                        const std::string& table,
                        const std::string& column,
                        cql_int_t& output) const
{
    return false;
}

bool
cql::cql_row_t::get_float(int i,
                          float& output) const
{
    return false;
}

bool
cql::cql_row_t::get_float(const std::string& column,
                          float& output) const
{
    return false;
}

bool
cql::cql_row_t::get_float(const std::string& keyspace,
                          const std::string& table,
                          const std::string& column,
                          float& output) const
{
    return false;
}

bool
cql::cql_row_t::get_double(int i,
                           double& output) const
{
    return false;
}

bool
cql::cql_row_t::get_double(const std::string& column,
                           double& output) const
{
    return false;
}

bool
cql::cql_row_t::get_double(const std::string& keyspace,
                           const std::string& table,
                           const std::string& column,
                           double& output) const
{
    return false;
}

bool
cql::cql_row_t::get_string(int i,
                           std::string& output) const
{
    return false;
}

bool
cql::cql_row_t::get_string(const std::string& column,
                           std::string& output) const
{
    return false;
}

bool
cql::cql_row_t::get_string(const std::string& keyspace,
                           const std::string& table,
                           const std::string& column,
                           std::string& output) const
{
    return false;
}
