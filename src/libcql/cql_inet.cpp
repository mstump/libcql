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

#include "libcql/cql_serialization.hpp"

#include "libcql/cql_inet.hpp"

cql::cql_inet_t::cql_inet_t() :
    _port(0)
{}

cql::cql_inet_t::cql_inet_t(const std::string& address,
                            cql::cql_int_t port) :
    _address(address),
    _port(port)
{}

const std::string&
cql::cql_inet_t::address() const
{
    return _address;
}

cql::cql_int_t
cql::cql_inet_t::port() const
{
    return _port;
}

void
cql::cql_inet_t::address(const std::string& address)
{
    _address = address;
}

void
cql::cql_inet_t::port(cql_int_t port)
{
    _port = port;
}

std::istream&
cql::cql_inet_t::read(std::istream& input)
{
    return cql::decode_inet(input, _address, _port);
}

std::ostream&
cql::cql_inet_t::write(std::ostream& output) const
{
    return cql::encode_inet(output, _address, _port);
}
