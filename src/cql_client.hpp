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

#ifndef CQL_CLIENT_H_
#define CQL_CLIENT_H_

#include <stdint.h>
#include <istream>
#include <ostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>

#include "cql.h"

// Forward declarations
namespace boost {
    namespace asio {
        namespace ssl {
            class context;
        }
    }
}

namespace cql {

    // Forward declarations
    class cql_error_t;
    class cql_message_t;
    class cql_message_result_t;
    class cql_socket_t;

    namespace internal {
        class cql_header_t;
    } // namespace internal


    class cql_client_t :
        boost::noncopyable
    {

    public:
        typedef boost::function<void(cql_client_t&, const cql_stream_id_t, const cql::cql_message_result_t&)> cql_message_callback_t;
        typedef boost::function<void(cql_client_t&, const cql_stream_id_t, const cql_error_t&)> cql_message_errback_t;

        typedef boost::function<void(cql_client_t&)> cql_connection_callback_t;
        typedef boost::function<void(cql_client_t&, const cql_error_t&)> cql_connection_errback_t;

        typedef boost::function<void(const cql_short_t, const std::string&)> cql_log_callback_t;

        cql_client_t(boost::asio::io_service& io_service);

        cql_client_t(boost::asio::io_service& io_service,
                     cql_log_callback_t log_callback);

        cql_client_t(boost::asio::io_service& io_service,
                     boost::asio::ssl::context& context,
                     cql_log_callback_t log_callback);

        void
        connect(const std::string& server,
                unsigned int port,
                cql_connection_callback_t callback,
                cql_connection_errback_t errback);

        cql_stream_id_t
        query(const std::string& query,
              cql_int_t consistency,
              cql_message_callback_t callback,
              cql_message_errback_t errback);

        cql_stream_id_t
        write(cql::cql_message_t& data,
              cql_message_callback_t callback,
              cql_message_errback_t errback);

        bool
        defunct();

        void
        close();

        void
        close(cql_error_t& err);

    private:
        typedef boost::tuple<cql_message_callback_t, cql_message_errback_t> callback_tuple_t;
        typedef boost::unordered_map<cql_stream_id_t, callback_tuple_t> callback_map_t;
        typedef boost::function<void (const boost::system::error_code&, std::size_t)> write_callback_t;

        inline void
        log(cql_short_t level,
            const std::string& message);

        cql_stream_id_t
        get_new_stream();

        void
        resolve_handle(const boost::system::error_code& err,
                       boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

        void
        connect_handle(const boost::system::error_code& err);

        cql_stream_id_t
        write_message(cql::cql_message_t& data,
                      const write_callback_t& callback);

        void
        write_handle(const boost::system::error_code& err,
                     std::size_t);

        void
        header_read();

        void
        header_read_handle(const boost::system::error_code& err);

        void
        body_read(const cql::internal::cql_header_t& header);

        void
        body_read_handle(const cql::internal::cql_header_t& header,
                         const boost::system::error_code& err);

        void
        startup_write();

        void
        ready_receive();

        void
        supported_receive();

        void
        error_receive(const cql::internal::cql_header_t& header);

        void
        result_receive(const cql::internal::cql_header_t& header);

        inline void
        check_transport_err(const boost::system::error_code& err);

        cql_stream_id_t                _stream_counter;
        boost::asio::ip::tcp::resolver _resolver;
        std::auto_ptr<cql_socket_t>    _socket;
        bool                           _ssl;

        boost::asio::streambuf         _receive_buffer;
        boost::asio::streambuf         _request_buffer;
        callback_map_t                 _callback_map;

        cql_connection_callback_t      _connect_callback;
        cql_connection_errback_t       _connect_errback;
        cql_log_callback_t             _log_callback;
        bool                           _defunct;
    };

} // namespace cql

#endif // CQL_CLIENT_H_
