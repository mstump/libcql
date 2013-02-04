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

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include "libcql/internal/cql_defines.hpp"
#include "libcql/cql_serialization.hpp"

#include "libcql/cql_row.hpp"

struct column_to_str
{
    typedef std::string result_type;

    const char*
    operator()(const cql::cql_row_t::column_t& c) const {
        return reinterpret_cast<const char*>(&c[0]);
    }
};

cql::cql_row_t::cql_row_t(const cql::cql_result_metadata_t& metadata,
                          std::istream& input) :
    _metadata(metadata),
    _row(_metadata.column_count())
{
    for (int i = 0; i < _metadata.column_count(); ++i)
    {
        std::auto_ptr<cql::cql_row_t::column_t> c( new cql::cql_row_t::column_t() );
        cql::decode_bytes(input, *c);
        _row.push_back(c);
    }
}

cql::cql_int_t
cql::cql_row_t::size() const
{
    return _row.size();
}

std::string
cql::cql_row_t::str() const
{
    std::list<std::string> columns;
    boost::copy(
        _row | boost::adaptors::transformed(column_to_str()),
        std::back_inserter(columns));

    std::stringstream output;
    output << "[" << boost::algorithm::join(columns, ", ") << "]";
    return output.str();
}

const cql::cql_row_t::column_t&
cql::cql_row_t::operator[](size_type n) const
{
    return _row[n];
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
    if (i > _row.size() || i < 0)
        return false;

    const cql::cql_row_t::column_t column = _row[i];
    if (column.empty())
        return false;

    output = column[0] != 0x00;
    return true;
}

bool
cql::cql_row_t::get_bool(const std::string& column,
                         bool& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        const cql::cql_row_t::column_t column = _row[i];
        if (column.empty())
            return false;

        output = column[0] != 0x00;
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_bool(const std::string& keyspace,
                         const std::string& table,
                         const std::string& column,
                         bool& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        const cql::cql_row_t::column_t column = _row[i];
        if (column.empty())
            return false;

        output = column[0] != 0x00;
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_int(int i,
                        cql::cql_int_t& output) const
{
    if (i > _row.size() || i < 0)
        return false;

    output = cql::decode_int(_row[i]);
    return true;
}

bool
cql::cql_row_t::get_int(const std::string& column,
                        cql::cql_int_t& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        output = cql::decode_int(_row[i]);
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_int(const std::string& keyspace,
                        const std::string& table,
                        const std::string& column,
                        cql::cql_int_t& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        output = cql::decode_int(_row[i]);
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_float(int i,
                          float& output) const
{
    if (i > _row.size() || i < 0)
        return false;

    output = cql::decode_float(_row[i]);
    return true;
}

bool
cql::cql_row_t::get_float(const std::string& column,
                          float& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        output = cql::decode_float(_row[i]);
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_float(const std::string& keyspace,
                          const std::string& table,
                          const std::string& column,
                          float& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        output = cql::decode_float(_row[i]);
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_double(int i,
                           double& output) const
{
    if (i > _row.size() || i < 0)
        return false;

    output = cql::decode_double(_row[i]);
    return true;
}

bool
cql::cql_row_t::get_double(const std::string& column,
                           double& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        output = cql::decode_double(_row[i]);
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_double(const std::string& keyspace,
                           const std::string& table,
                           const std::string& column,
                           double& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        output = cql::decode_double(_row[i]);
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_bigint(int i,
                           cql::cql_bigint_t& output) const
{
    if (i > _row.size() || i < 0)
        return false;

    output = cql::decode_bigint(_row[i]);
    return true;
}

bool
cql::cql_row_t::get_bigint(const std::string& column,
                           cql::cql_bigint_t& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        output = cql::decode_bigint(_row[i]);
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_bigint(const std::string& keyspace,
                           const std::string& table,
                           const std::string& column,
                           cql::cql_bigint_t& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        output = cql::decode_bigint(_row[i]);
        return true;
    }
    return false;
}

bool
cql::cql_row_t::get_string(int i,
                           std::string& output) const
{
    if (i > _row.size() || i < 0)
        return false;

    const column_t& r = _row[i];
    output.assign(r.begin(), r.end());
    return true;
}

bool
cql::cql_row_t::get_string(const std::string& column,
                           std::string& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        return get_string(i, output);
    }
    return false;
}

bool
cql::cql_row_t::get_string(const std::string& keyspace,
                           const std::string& table,
                           const std::string& column,
                           std::string& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        return get_string(i, output);
    }
    return false;
}

bool
cql::cql_row_t::get_list(int i,
                         cql::cql_list_t& output) const
{
    if (i > _row.size() || i < 0)
        return false;

    const column_t& r = _row[i];
    output.read(r);
    return true;
}

bool
cql::cql_row_t::get_list(const std::string& column,
                         cql::cql_list_t& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        return get_list(i, output);
    }
    return false;
}

bool
cql::cql_row_t::get_list(const std::string& keyspace,
                         const std::string& table,
                         const std::string& column,
                         cql::cql_list_t& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        return get_list(i, output);
    }
    return false;
}

bool
cql::cql_row_t::get_set(int i,
                        cql::cql_set_t& output) const
{
    if (i > _row.size() || i < 0)
        return false;

    const column_t& r = _row[i];
    output.read(r);
    return true;
}

bool
cql::cql_row_t::get_set(const std::string& column,
                        cql::cql_set_t& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        return get_set(i, output);
    }
    return false;
}

bool
cql::cql_row_t::get_set(const std::string& keyspace,
                        const std::string& table,
                        const std::string& column,
                        cql::cql_set_t& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        return get_set(i, output);
    }
    return false;
}

bool
cql::cql_row_t::get_map(int i,
                        cql::cql_map_t& output) const
{
    if (i > _row.size() || i < 0)
        return false;

    const column_t& r = _row[i];
    output.read(r);
    return true;
}

bool
cql::cql_row_t::get_map(const std::string& column,
                        cql::cql_map_t& output) const
{
    int i = 0;
    if (_metadata.get_index(column, i))
    {
        return get_map(i, output);
    }
    return false;
}

bool
cql::cql_row_t::get_map(const std::string& keyspace,
                        const std::string& table,
                        const std::string& column,
                        cql::cql_map_t& output) const
{
    int i = 0;
    if (_metadata.get_index(keyspace, table, column, i))
    {
        return get_map(i, output);
    }
    return false;
}
