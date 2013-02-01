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

#include "cql_socket.hpp"

cql::cql_socket_t::cql_socket_t(boost::asio::io_service& io_service) :
    _socket(new boost::asio::ip::tcp::socket(io_service))
{
}

boost::asio::io_service&
cql::cql_socket_t::get_io_service()
{
    return _socket->get_io_service();
}

bool
cql::cql_socket_t::requires_handshake()
{
    return false;
}

boost::asio::ip::tcp::socket&
cql::cql_socket_t::lowest_layer()
{
    return *_socket;
}

void
cql::cql_socket_t::reset()
{
    _socket.reset(new boost::asio::ip::tcp::socket(_socket->get_io_service()));
}
