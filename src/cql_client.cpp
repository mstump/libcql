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

#include <iomanip>
#include <iostream>
#include <istream>
#include <ostream>
#include <stdint.h>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>

#include "cql_error.hpp"
#include "cql_header.hpp"
#include "cql_message.hpp"
#include "cql_message_error.hpp"
#include "cql_message_event.hpp"
#include "cql_message_execute.hpp"
#include "cql_message_options.hpp"
#include "cql_message_prepare.hpp"
#include "cql_message_query.hpp"
#include "cql_message_ready.hpp"
#include "cql_message_register.hpp"
#include "cql_message_result.hpp"
#include "cql_message_startup.hpp"
#include "cql_message_supported.hpp"
#include "serialization.hpp"

#include "cql_client.hpp"

cql::cql_client_t::cql_client_t(boost::asio::io_service& io_service,
                                cql::cql_client_t::ssl_stream_t& stream) :
    _resolver(io_service),
    _stream(stream),
    _log_callback(0),
    _defunct(false),
    _ssl(false)
{}

cql::cql_client_t::cql_client_t(boost::asio::io_service& io_service,
                                cql::cql_client_t::ssl_stream_t& stream,
                                cql::cql_client_t::cql_log_callback_t log_callback) :
    _resolver(io_service),
    _stream(stream),
    _log_callback(log_callback),
    _defunct(false),
    _ssl(false)
{}

void
cql::cql_client_t::connect(const std::string& server,
                           unsigned int port,
                           bool ssl,
                           cql_connection_callback_t callback,
                           cql_connection_errback_t errback)
{
    std::list<std::string> events;
    connect(server, port, ssl, callback, errback, NULL, events);
}

void
cql::cql_client_t::connect(const std::string& server,
                           unsigned int port,
                           bool ssl,
                           cql_connection_callback_t callback,
                           cql_connection_errback_t errback,
                           cql_event_callback_t event_callback,
                           const std::list<std::string>& events)
{
    _ssl = ssl;
    _events = events;
    _event_callback = event_callback;
    _connect_callback = callback;
    _connect_errback = errback;
    log(CQL_LOG_DEBUG, "resolving remote host " + server + ":" + boost::lexical_cast<std::string>(port));

    boost::asio::ip::tcp::resolver::query query(server, boost::lexical_cast<std::string>(port));
    _resolver.async_resolve(query,
                            boost::bind(&cql_client_t::resolve_handle,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::iterator));
}

cql_stream_id_t
cql::cql_client_t::query(const std::string& query,
                         cql_int_t consistency,
                         cql_message_callback_t callback,
                         cql_message_errback_t errback)
{
    cql::cql_message_query_t m(query, consistency);
    cql_stream_id_t stream = write_message(m,
                                           boost::bind(&cql_client_t::write_handle,
                                                       this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));

    _callback_map.insert(callback_map_t::value_type(stream, callback_pair_t(callback, errback)));
    return stream;
}

cql_stream_id_t
cql::cql_client_t::prepare(const cql::cql_message_prepare_t& message,
                           cql_message_callback_t callback,
                           cql_message_errback_t errback)
{
    cql_stream_id_t stream = write_message(message,
                                           boost::bind(&cql_client_t::write_handle,
                                                       this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));

    _callback_map.insert(callback_map_t::value_type(stream, callback_pair_t(callback, errback)));
    return stream;
}

cql_stream_id_t
cql::cql_client_t::execute(const cql::cql_message_execute_t& message,
                           cql_message_callback_t callback,
                           cql_message_errback_t errback)
{
    cql_stream_id_t stream = write_message(message,
                                           boost::bind(&cql_client_t::write_handle,
                                                       this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));

    _callback_map.insert(callback_map_t::value_type(stream, callback_pair_t(callback, errback)));
    return stream;
}

inline void
cql::cql_client_t::log(cql_short_t level,
                       const std::string& message)
{
    if (_log_callback) {
        _log_callback(level, message);
    }
}

cql_stream_id_t
cql::cql_client_t::get_new_stream()
{
    if (_stream_counter < INT8_MAX) {
        return _stream_counter++;
    }
    else {
        _stream_counter = 0;
        return _stream_counter;
    }
}

void
cql::cql_client_t::resolve_handle(const boost::system::error_code& err,
                                  boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
    if (!err) {
        log(CQL_LOG_DEBUG, "resolved remote host, attempting to connect");
        boost::asio::async_connect(_stream.lowest_layer(),
                                   endpoint_iterator,
                                   boost::bind(&cql_client_t::connect_handle,
                                               this,
                                               boost::asio::placeholders::error));
    }
    else {
        log(CQL_LOG_CRITICAL, "error resolving remote host " + err.message());
        check_transport_err(err);
    }
}

void
cql::cql_client_t::connect_handle(const boost::system::error_code& err)
{
    if (!err) {

        log(CQL_LOG_DEBUG, "connection successful to remote host");
        if (_ssl) {
            _stream.async_handshake(boost::asio::ssl::stream_base::client,
                                    boost::bind(&cql_client_t::handshake_handle,
                                                this,
                                                boost::asio::placeholders::error));
        }
        else {
            options_write();
        }
    }
    else {
        log(CQL_LOG_CRITICAL, "error connecting to remote host " + err.message());
        check_transport_err(err);
    }
}

void
cql::cql_client_t::handshake_handle(const boost::system::error_code& err)
{
    if (!err) {
        log(CQL_LOG_DEBUG, "successful ssl handshake with remote host");
        options_write();
    }
    else {
        log(CQL_LOG_CRITICAL, "error performing ssl handshake " + err.message());
        check_transport_err(err);
    }
}

cql_stream_id_t
cql::cql_client_t::write_message(const cql::cql_message_t& data,
                                 const write_callback_t& callback)
{
    log(CQL_LOG_DEBUG, "sending message: " + data.str());
    std::ostream request_stream(&_request_buffer);
    cql::internal::cql_header_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, get_new_stream(), data.opcode(), data.size());
    header.write(request_stream);
    data.write(request_stream);
    boost::asio::async_write(_stream, _request_buffer, callback);
    return header.stream();
}

cql_stream_id_t
cql::cql_client_t::write_message(const cql::cql_message_t& data,
                                 cql_message_callback_t callback,
                                 cql_message_errback_t errback)
{
    cql_stream_id_t stream = write_message(data,
                                           boost::bind(&cql_client_t::write_handle,
                                                       this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));

    _callback_map.insert(callback_map_t::value_type(stream, callback_pair_t(callback, errback)));
    return stream;
}

void
cql::cql_client_t::write_handle(const boost::system::error_code& err,
                                std::size_t num_bytes)
{
    if (!err) {
        log(CQL_LOG_DEBUG, "wrote to socket " + boost::lexical_cast<std::string>(num_bytes) + " bytes");
    }
    else {
        log(CQL_LOG_ERROR, "error writing to socket " + err.message());
        check_transport_err(err);
    }
}

void
cql::cql_client_t::header_read()
{
    boost::asio::async_read(_stream,
                            _receive_buffer,
                            boost::asio::transfer_exactly(sizeof(cql::internal::cql_header_t)),
                            boost::bind(&cql_client_t::header_read_handle, this, boost::asio::placeholders::error));
}

void
cql::cql_client_t::header_read_handle(const boost::system::error_code& err)
{
    if (!err) {
        cql::internal::cql_header_t header;
        std::istream response_stream(&_receive_buffer);
        header.read(response_stream);
        log(CQL_LOG_DEBUG, "received header for message " + header.str());
        body_read(header);
    }
    else {
        log(CQL_LOG_ERROR, "error reading header " + err.message());
        check_transport_err(err);
    }
}

void
cql::cql_client_t::body_read(const cql::internal::cql_header_t& header)
{
    boost::asio::async_read(_stream,
                            _receive_buffer,
                            boost::asio::transfer_exactly(header.length()),
                            boost::bind(&cql_client_t::body_read_handle, this, header, boost::asio::placeholders::error));
}

void
cql::cql_client_t::body_read_handle(const cql::internal::cql_header_t& header,
                                    const boost::system::error_code& err)
{
    log(CQL_LOG_DEBUG, "received body for message " + header.str());

    if (!err) {
        switch (header.opcode()) {

        case CQL_OPCODE_ERROR:
            error_receive(header);
            break;

        case CQL_OPCODE_RESULT:
            result_receive(header);
            break;

        case CQL_OPCODE_SUPPORTED:
            supported_receive();
            break;

        case CQL_OPCODE_READY:
            ready_receive();
            break;

        case CQL_OPCODE_EVENT:
            event_receive();
            break;

        case CQL_OPCODE_CREDENTIALS:

        default:
            log(CQL_LOG_ERROR, "unhandled opcode " + header.str());
        }
    }
    else {
        log(CQL_LOG_ERROR, "error reading body " + err.message());
        check_transport_err(err);
    }
    header_read(); // loop
}

void
cql::cql_client_t::events_register()
{
    cql::cql_message_register_t m;
    m.events(_events);

    cql_stream_id_t stream = write_message(m,
                                           boost::bind(&cql_client_t::write_handle,
                                                       this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));
    _events.clear();
}

void
cql::cql_client_t::options_write()
{
    cql::cql_message_options_t m;
    write_message(m,
                  boost::bind(&cql_client_t::write_handle,
                              this,
                              boost::asio::placeholders::error,
                              boost::asio::placeholders::bytes_transferred));

    // start listening
    header_read();
}

void
cql::cql_client_t::startup_write()
{
    cql::cql_message_startup_t m;
    m.version(CQL_VERSION_IMPL);
    write_message(m,
                  boost::bind(&cql_client_t::write_handle,
                              this,
                              boost::asio::placeholders::error,
                              boost::asio::placeholders::bytes_transferred));
}

void
cql::cql_client_t::ready_receive()
{
    log(CQL_LOG_DEBUG, "received ready message");
    std::istream response_stream(&_receive_buffer);
    cql::cql_message_ready_t m;
    m.read(response_stream);

    if (!_events.empty()) {
        events_register();
    }
    else if (_connect_callback) {
        // let the caller know that the connection is ready
        _connect_callback(*this);
    }
}

void
cql::cql_client_t::event_receive()
{
    log(CQL_LOG_DEBUG, "received event message");
    std::istream response_stream(&_receive_buffer);
    std::auto_ptr<cql::cql_message_event_t> m(cql::read_cql_event(response_stream));

    if (_event_callback) {
        _event_callback(*this, *m);
    }
}

void
cql::cql_client_t::error_receive(const cql::internal::cql_header_t& header)
{
    std::istream response_stream(&_receive_buffer);
    cql::cql_message_error_t m;
    log(CQL_LOG_ERROR, m.message());
    m.read(response_stream);

    callback_map_t::iterator it = _callback_map.find(header.stream());
    if (it != _callback_map.end()) {
        cql_error_t err(true, m.code(), 0, m.message());
        (*it).second.second(*this, header.stream(), err);
        _callback_map.erase(it);
    }
    else {
        log(CQL_LOG_INFO, "no callback found for message " + header.str());
    }
}

void
cql::cql_client_t::supported_receive()
{
    std::istream response_stream(&_receive_buffer);
    cql::cql_message_supported_t m;
    m.read(response_stream);
    log(CQL_LOG_DEBUG, "received supported message " + m.str());
    startup_write();
}

void
cql::cql_client_t::result_receive(const cql::internal::cql_header_t& header)
{
    std::istream response_stream(&_receive_buffer);
    cql::cql_message_result_t m;
    m.read(response_stream);
    log(CQL_LOG_DEBUG, "received result message " + m.str());

    callback_map_t::iterator it = _callback_map.find(header.stream());
    if (it != _callback_map.end()) {
        (*it).second.first(*this, header.stream(), m);
        _callback_map.erase(it);
    }
    else {
        log(CQL_LOG_ERROR, "no callback found for message " + header.str());
    }
}

bool
cql::cql_client_t::defunct()
{
    return _defunct;
}

void
cql::cql_client_t::close(cql_error_t& err)
{
    boost::system::error_code ec;
    _stream.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    _stream.lowest_layer().close();
    err.application(false);
    err.application_error(0);
    err.transport_error(ec.value());
    err.message(ec.message());
}

void
cql::cql_client_t::close()
{
    log(CQL_LOG_INFO, "closing connection");
    boost::system::error_code ec;
    _stream.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    _stream.lowest_layer().close();
}

inline void
cql::cql_client_t::check_transport_err(const boost::system::error_code& err)
{
    if (!_stream.lowest_layer().is_open()) {
        _defunct = true;
    }

    if (_connect_errback) {
        cql_error_t e(false, 0, err.value(), err.message());
        _connect_errback(*this, e);
    }
}
