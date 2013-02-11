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

#include <memory>
#include "libcql/internal/cql_client_impl.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_socket.hpp"
#include "libcql/internal/cql_socket_ssl.hpp"

#include "libcql/cql_client_factory.hpp"

typedef cql::cql_client_impl_t<cql::cql_socket_t> client_t;
typedef cql::cql_client_impl_t<cql::cql_socket_ssl_t> client_ssl_t;

cql::cql_client_t*
cql::cql_client_factory_t::create_cql_client_t(boost::asio::io_service& io_service)
{
    std::auto_ptr<client_t> client(
        new client_t(io_service,
                     new cql::cql_socket_t(io_service)));
    return client.release();
}

cql::cql_client_t*
cql::cql_client_factory_t::create_cql_client_t(boost::asio::io_service& io_service,
                                               boost::asio::ssl::context& context)
{
    std::auto_ptr<client_ssl_t> client(
        new client_ssl_t(io_service,
                         new cql::cql_socket_ssl_t(io_service, context)));
    return client.release();
}

cql::cql_client_t*
cql::cql_client_factory_t::create_cql_client_t(boost::asio::io_service& io_service,
                                               cql::cql_client_t::cql_log_callback_t log_callback)
{
    std::auto_ptr<client_t> client(
        new client_t(io_service,
                     new cql::cql_socket_t(io_service),
                     log_callback));
    return client.release();
}

cql::cql_client_t*
cql::cql_client_factory_t::create_cql_client_t(boost::asio::io_service& io_service,
                                               boost::asio::ssl::context& context,
                                               cql::cql_client_t::cql_log_callback_t log_callback)
{
    std::auto_ptr<client_ssl_t> client(
        new client_ssl_t(io_service,
                         new cql::cql_socket_ssl_t(io_service, context),
                         log_callback));
    return client.release();
}
