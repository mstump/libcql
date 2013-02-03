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

#ifndef CQL_CLIENT_FACTORY_H_
#define CQL_CLIENT_FACTORY_H_

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "libcql/cql_client.hpp"

namespace cql {

    cql_client_t*
    create_cql_client_t(boost::asio::io_service& service);

    cql_client_t*
    create_cql_client_t(boost::asio::io_service& service,
                        boost::asio::ssl::context& context);

    cql_client_t*
    create_cql_client_t(boost::asio::io_service& service,
                        cql::cql_client_t::cql_log_callback_t log_callback);

    cql_client_t*
    create_cql_client_t(boost::asio::io_service& service,
                        boost::asio::ssl::context& context,
                        cql::cql_client_t::cql_log_callback_t log_callback);


} // namespace cql

#endif // CQL_CLIENT_H_
