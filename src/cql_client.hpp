/*
  Copyright (c) 2012 Matthew Stump

  This file is part of libcql.

  libcql is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  libcql is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.	If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CQL_CLIENT_H_
#define CQL_CLIENT_H_

#include "../include/cql.h"

#include <stdint.h>
#include <istream>
#include <ostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>

typedef struct
{
	bool		 application;
	cql_int_t	 application_error;
	cql_int_t	 transport_error;
	char*		 message;
} cql_error_t;

// Forward declarations
namespace cql {
class cql_message_t;
class cql_message_result_t;

namespace internal {
class cql_header_t;
} // namespace internal


class cql_client_t :
	boost::noncopyable
{

public:

	typedef boost::function<void(cql_client_t&, cql_stream_id_t, const cql_error_t&)> cql_errorback_t;
	typedef boost::function<void(cql_client_t&, cql_stream_id_t, const cql::cql_message_result_t&)> cql_callback_result_t;
	typedef boost::function<void(cql_client_t&)> cql_callback_connection_t;

	cql_client_t(boost::asio::io_service& io_service);

	void
	connect(const std::string& server,
			unsigned int port,
			cql_callback_connection_t callback);


	cql_stream_id_t
	query(const std::string& query,
		  cql_int_t consistency,
		  cql_callback_result_t callback,
		  cql_errorback_t errback);

private:
	typedef boost::tuple<cql_callback_result_t, cql_errorback_t> callback_tuple_t;
	typedef boost::unordered_map<cql_stream_id_t, callback_tuple_t> callback_map_t;
	typedef boost::function<void (const boost::system::error_code&, std::size_t)> write_callback_t;

	cql_stream_id_t
	get_new_stream();

	void
	resolve_handle(const boost::system::error_code& err,
				   boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

	void
	connect_handle(const boost::system::error_code& err);

	cql_byte_t
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
	error_receive();

	void
	supported_receive();

	void
	result_receive(const cql::internal::cql_header_t& header);

	cql_stream_id_t									 _stream_counter;
	boost::asio::ip::tcp::resolver					 _resolver;
	boost::asio::ip::tcp::socket					 _socket;
	boost::asio::streambuf							 _receive_buffer;
	boost::asio::streambuf							 _request_buffer;
	callback_map_t									 _callback_map;
	cql_callback_connection_t						 _connect_callback;
};

} // namespace cql

#endif // CQL_CLIENT_H_
