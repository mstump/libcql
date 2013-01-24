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

#ifndef CQL_CLIENT_IMPL_H_
#define CQL_CLIENT_IMPL_H_

#include <stdint.h>
#include <istream>
#include <ostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>

#include "cql.h"
#include "cql_client.hpp"

namespace cql {
    namespace internal {

        class cql_header_t;

        template <typename cql_transport_t>
        class cql_client_impl_t :
            boost::noncopyable,
            public cql::cql_client_t
        {

        public:

            cql_client_impl_t(boost::asio::io_service& io_service,
                              cql_transport_t* transport);

            cql_client_impl_t(boost::asio::io_service& io_service,
                              cql_transport_t* transport,
                              cql::cql_client_t::cql_log_callback_t log_callback);

            void
            connect(const std::string& server,
                    unsigned int port,
                    cql::cql_client_t::cql_connection_callback_t callback,
                    cql::cql_client_t::cql_connection_errback_t errback);

            void
            connect(const std::string& server,
                    unsigned int port,
                    cql::cql_client_t::cql_connection_callback_t callback,
                    cql::cql_client_t::cql_connection_errback_t errback,
                    cql::cql_client_t::cql_event_callback_t event_callback,
                    const std::list<std::string>& events);

            cql_stream_id_t
            query(const std::string& query,
                  cql_int_t consistency,
                  cql::cql_client_t::cql_message_callback_t callback,
                  cql::cql_client_t::cql_message_errback_t errback);

            cql_stream_id_t
            prepare(const cql::cql_message_prepare_t& message,
                    cql::cql_client_t::cql_message_callback_t callback,
                    cql::cql_client_t::cql_message_errback_t errback);

            cql_stream_id_t
            execute(const cql::cql_message_execute_t& message,
                    cql::cql_client_t::cql_message_callback_t callback,
                    cql::cql_client_t::cql_message_errback_t errback);

            bool
            defunct();

            void
            close();

            void
            close(cql_error_t& err);

        private:
            typedef std::pair<cql_message_callback_t, cql_message_errback_t> callback_pair_t;
            typedef boost::unordered_map<cql_stream_id_t, callback_pair_t> callback_map_t;
            typedef boost::function<void(const boost::system::error_code&, std::size_t)> write_callback_t;

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

            void
            handshake_handle(const boost::system::error_code& err);

            cql_stream_id_t
            write_message(const cql::cql_message_t& data,
                          write_callback_t& callback);

            cql_stream_id_t
            write_message(const cql::cql_message_t& data,
                          cql::cql_client_t::cql_message_callback_t callback,
                          cql::cql_client_t::cql_message_errback_t errback);

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
            events_register();

            void
            options_write();

            void
            startup_write();

            void
            event_receive();

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
            std::auto_ptr<cql_transport_t> _transport;

            boost::asio::streambuf         _receive_buffer;
            boost::asio::streambuf         _request_buffer;
            callback_map_t                 _callback_map;

            cql_connection_callback_t      _connect_callback;
            cql_connection_errback_t       _connect_errback;
            cql_log_callback_t             _log_callback;

            std::list<std::string>         _events;
            cql_event_callback_t           _event_callback;

            bool                           _defunct;
        };

    } // namespace internal
} // namespace cql

#endif // CQL_CLIENT_IMPL_H_
