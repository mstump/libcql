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

#ifndef CQL_SOCKET_H_
#define CQL_SOCKET_H_

#include <boost/asio.hpp>

namespace cql {

    class cql_socket_t {
    public:

        cql_socket_t(boost::asio::io_service& io_service);

        boost::asio::io_service&
        get_io_service();

        template<typename ConstBufferSequence, typename WriteHandler>
        void
        async_write_some(const ConstBufferSequence& buffers,
                         WriteHandler handler);

        template<typename HandshakeHandler>
        void
        async_handshake(HandshakeHandler);

        bool
        requires_handshake();

    private:
        boost::asio::ip::tcp::socket _socket;
    };
} // namespace cql

#endif // CQL_SOCKET_H_
