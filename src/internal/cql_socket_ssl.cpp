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

#include "internal/cql_socket_ssl.hpp"

cql::cql_socket_ssl_t::cql_socket_ssl_t(boost::asio::io_service& io_service,
                                        boost::asio::ssl::context& ctx) :
    _socket(new cql::cql_socket_ssl_t::socket_t(io_service, ctx)),
    _ctx(ctx)
{
}

boost::asio::io_service&
cql::cql_socket_ssl_t::get_io_service()
{
    return _socket->get_io_service();
}

bool
cql::cql_socket_ssl_t::requires_handshake()
{
    return true;
}

boost::asio::ip::tcp::socket&
cql::cql_socket_ssl_t::lowest_layer()
{
    return _socket->next_layer();
}

void
cql::cql_socket_ssl_t::reset()
{
    _socket.reset(new cql::cql_socket_ssl_t::socket_t(_socket->get_io_service(), _ctx));
}
