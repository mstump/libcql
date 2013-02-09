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
    class cql_client_factory_t {
    public:

        /** Instantiate a new cql_client_t. Client is not capable of SSL.

            @param service the boost IO service used for all network IO
        */
        static cql_client_t*
        create_cql_client_t(boost::asio::io_service& service);

        /** Instantiate a new cql_client_t. The client will attempt an SSL handshake on connect.

            @param service the boost IO service used for all network IO
            @param context the boost SSL context, dictates cert validation behavior and SSL version
        */
        static cql_client_t*
        create_cql_client_t(boost::asio::io_service& service,
                            boost::asio::ssl::context& context);

        /** Instantiate a new cql_client_t. Client is not capable of SSL.

            @param service the boost IO service used for all network IO
            @param log_callback a callback which will be triggered for all internally generated log messages
        */
        static cql_client_t*
        create_cql_client_t(boost::asio::io_service& service,
                            cql::cql_client_t::cql_log_callback_t log_callback);

        /** Instantiate a new cql_client_t. The client will attempt an SSL handshake on connect.

            @param service the boost IO service used for all network IO
            @param context the boost SSL context, dictates cert validation behavior and SSL version
            @param log_callback a callback which will be triggered for all internally generated log messages
        */
        static cql_client_t*
        create_cql_client_t(boost::asio::io_service& service,
                            boost::asio::ssl::context& context,
                            cql::cql_client_t::cql_log_callback_t log_callback);

    };
} // namespace cql

#endif // CQL_CLIENT_FACTORY_H_
