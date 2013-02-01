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

namespace cql {

    class cql_socket_ssl_t :
        boost::noncopyable
    {
    public:

        cql_socket_ssl_t(boost::asio::io_service& io_service,
                         boost::asio::ssl::context& ctx);

        boost::asio::io_service&
        get_io_service();

        template<typename ConstBufferSequence, typename WriteHandler>
        void
        async_write_some(const ConstBufferSequence& buffers,
                         WriteHandler handler)
        {
            _socket->async_write_some(buffers, handler);
        }

        template<typename MutableBufferSequence, typename ReadHandler>
        void
        async_read_some(const MutableBufferSequence& buffers,
                        ReadHandler handler)
        {
            _socket->async_read_some(buffers, handler);
        }

        template<typename HandshakeHandler>
        void
        async_handshake(HandshakeHandler handler)
        {
            _socket->async_handshake(boost::asio::ssl::stream_base::client, handler);
        }

        bool
        requires_handshake();

        boost::asio::ip::tcp::socket&
        lowest_layer();

        void
        reset();

    private:
        typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_t;
        std::auto_ptr<socket_t> _socket;
        boost::asio::ssl::context& _ctx;
    };
} // namespace cql

#endif // CQL_SOCKET_SSL_H_
