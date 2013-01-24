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

#include "cql_socket_ssl.hpp"

cql::cql_socket_ssl_t::cql_socket_ssl_t(boost::asio::io_service& io_service,
                                        boost::asio::ssl::context& ctx) :
    _socket(io_service, ctx)
{
}

boost::asio::io_service&
cql::cql_socket_ssl_t::get_io_service()
{
    return _socket.get_io_service();
}

template<typename ConstBufferSequence, typename WriteHandler>
void
cql::cql_socket_ssl_t::async_write_some(const ConstBufferSequence& buffers,
                                    WriteHandler handler)
{
    _socket.async_write_some(buffers, handler);
}

template<typename HandshakeHandler>
void
cql::cql_socket_ssl_t::async_handshake(HandshakeHandler handler)
{
    _socket.async_handshake(boost::asio::ssl::stream_base::client, handler);
}

bool
cql::cql_socket_ssl_t::requires_handshake()
{
    return true;
}
