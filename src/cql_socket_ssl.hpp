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

#ifndef CQL_SOCKET_SSL_H_
#define CQL_SOCKET_SSL_H_

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/noncopyable.hpp>

namespace cql {

    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> cql_socket_stream_ssl_t;

    class cql_socket_ssl_t :
        boost::noncopyable,
        cql_socket_stream_ssl_t,
        public cql_socket_t
    {
    public:
        cql_socket_ssl_t(boost::asio::io_service& service,
                         boost::asio::ssl::context& context) :
            cql_socket_stream_ssl_t(service, context)
        {
        }

    private:
        boost::asio::ip::tcp::socket&
        socket() {
            return next_layer();
        }
    };

} // namespace cql

#endif // CQL_SOCKET_SSL_H_
