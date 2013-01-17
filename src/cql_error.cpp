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

#include "cql_error.hpp"


cql::cql_error_t::cql_error_t(
    bool application,
    cql_int_t application_error,
    cql_int_t transport_error,
    const std::string& message)
{
}

bool
cql::cql_error_t::application() const
{
    return _application;
}

cql_int_t
cql::cql_error_t::application_error() const
{
    return _application_error;
}

cql_int_t
cql::cql_error_t::transport_error() const
{
    return _transport_error;
}

const std::string&
cql::cql_error_t::message() const
{
    return _message;
}

void
cql::cql_error_t::application(bool v)
{
    _application = v;
}

void
cql::cql_error_t::application_error(cql_int_t err)
{
    _application_error = err;
}

void
cql::cql_error_t::transport_error(cql_int_t err)
{
    _transport_error = err;
}

void
cql::cql_error_t::message(const std::string& msg)
{
    _message = msg;
}
