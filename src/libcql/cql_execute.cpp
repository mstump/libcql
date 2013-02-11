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

#include "libcql/internal/cql_message_execute_impl.hpp"

#include "libcql/cql_execute.hpp"

cql::cql_execute_t::cql_execute_t() :
    _impl(new cql_message_execute_impl_t())
{}

cql::cql_execute_t::cql_execute_t(const std::vector<cql::cql_byte_t>& id,
                                  cql::cql_consistency_enum consistency) :
    _impl(new cql_message_execute_impl_t(id, consistency))
{}

cql::cql_execute_t::~cql_execute_t()
{
    delete _impl;
}

const std::vector<cql::cql_byte_t>&
cql::cql_execute_t::query_id() const
{
    return _impl->query_id();
}

void
cql::cql_execute_t::query_id(const std::vector<cql::cql_byte_t>& id)
{
    _impl->query_id(id);
}

cql::cql_consistency_enum
cql::cql_execute_t::consistency() const
{
    return _impl->consistency();
}

void
cql::cql_execute_t::consistency(const cql::cql_consistency_enum consistency)
{
    _impl->consistency(consistency);
}

void
cql::cql_execute_t::push_back(const std::vector<cql::cql_byte_t>& val)
{
    _impl->push_back(val);
}

void
cql::cql_execute_t::push_back(const std::string& val)
{
    _impl->push_back(val);
}

void
cql::cql_execute_t::push_back(const cql::cql_short_t val)
{
    _impl->push_back(val);
}

void
cql::cql_execute_t::push_back(const cql_int_t val)
{
    _impl->push_back(val);
}

void
cql::cql_execute_t::push_back(const cql::cql_bigint_t val)
{
    _impl->push_back(val);
}

void
cql::cql_execute_t::push_back(const float val)
{
    _impl->push_back(val);
}

void
cql::cql_execute_t::push_back(const double val)
{
    _impl->push_back(val);
}

void
cql::cql_execute_t::push_back(const bool val)
{
    _impl->push_back(val);
}

void
cql::cql_execute_t::pop_back()
{
    _impl->pop_back();
}

cql::cql_message_execute_impl_t*
cql::cql_execute_t::impl() const
{
    return _impl;
}
