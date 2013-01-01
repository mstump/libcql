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

#include "../include/cql.h"

#include <stdint.h>

#include <iomanip>
#include <iostream>
#include <istream>
#include <ostream>

#include <list>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>

#include "cql_header.hpp"
#include "cql_message.hpp"
#include "cql_message_error.hpp"
#include "cql_message_options.hpp"
#include "cql_message_query.hpp"
#include "cql_message_ready.hpp"
#include "cql_message_register.hpp"
#include "cql_message_result.hpp"
#include "cql_message_startup.hpp"
#include "cql_message_supported.hpp"

#define CQL_FRAME_MAX_SIZE 1024 * 1024 * 256

typedef struct
{
	bool		 application;
	uint32_t	 application_error;
	uint32_t	 transport_error;
	char*		 message;
} cql_error_t;


class cql_client_t :
	boost::noncopyable
{

public:

	typedef boost::function<void(cql_client_t&, int8_t, const cql_error_t&)> cql_errorback_t;
	typedef boost::function<void(cql_client_t&, int8_t, const cql::cql_message_result_t&)> cql_callback_result_t;
	typedef boost::function<void(cql_client_t&)> cql_callback_connection_t;

	cql_client_t(boost::asio::io_service& io_service)
		: _resolver(io_service),
		  _socket(io_service)
	{}

	void
	connect(const std::string& server,
			unsigned int port,
			cql_callback_connection_t callback)
	{
		_connect_callback = callback;
		boost::asio::ip::tcp::resolver::query query(server, boost::lexical_cast<std::string>(port));
		_resolver.async_resolve(query,
								boost::bind(&cql_client_t::resolve_handle,
											this,
											boost::asio::placeholders::error,
											boost::asio::placeholders::iterator));
	}


	int8_t
	query(const std::string& query,
		  int32_t consistency,
		  cql_callback_result_t callback,
		  cql_errorback_t errback)
	{
		cql::cql_message_query_t m(query, consistency);
		std::cout << "send query: " << m.str() << std::endl;

        int8_t stream = write_message(m,
									  boost::bind(&cql_client_t::write_handle,
												  this,
												  boost::asio::placeholders::error,
												  boost::asio::placeholders::bytes_transferred));

		_callback_map.insert(callback_map_t::value_type(stream, callback_tuple_t(callback, errback)));
		return stream;
	}

private:
	typedef boost::tuple<cql_callback_result_t, cql_errorback_t> callback_tuple_t;
	typedef boost::unordered_map<int8_t, callback_tuple_t> callback_map_t;
	typedef boost::function<void (const boost::system::error_code&, std::size_t)> write_callback_t;

	int8_t
	get_new_stream()
	{
		if (_stream_counter < INT8_MAX)
		{
			return _stream_counter++;
		}
		else
		{
			_stream_counter = 0;
			return _stream_counter;
		}
	}

	void
	resolve_handle(const boost::system::error_code& err,
				   boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		std::cout << "handle resolve: " << err << std::endl;
		if (!err)
		{
			// Attempt a connection to each endpoint in the list until we
			// successfully establish a connection.
			boost::asio::async_connect(_socket,
									   endpoint_iterator,
									   boost::bind(&cql_client_t::connect_handle,
												   this,
												   boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
	}

	void
	connect_handle(const boost::system::error_code& err)
	{
		std::cout << "handle connect: " << err << std::endl;
		if (!err)
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
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
	}

	int8_t
	write_message(cql::cql_message_t& data,
				  const write_callback_t& callback)
	{
        std::ostream request_stream(&_request_buffer);
        cql::internal::cql_header_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, get_new_stream(), data.opcode(), data.size());
        header.write(request_stream);
        data.write(request_stream);
        boost::asio::async_write(_socket, _request_buffer, callback);
		std::cout << "write: " << header.str() << std::endl;
		return header.stream();
    }

	void
	write_handle(const boost::system::error_code& err,
				 std::size_t)
	{
		std::cout << "handle write: " << err << std::endl;
		if (!err)
		{
		}
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
	}

	void
	header_read()
	{
		boost::asio::async_read(_socket,
								_receive_buffer,
								boost::asio::transfer_exactly(sizeof(cql::internal::cql_header_t)),
								boost::bind(&cql_client_t::header_read_handle, this, boost::asio::placeholders::error));
	}

	void
	header_read_handle(const boost::system::error_code& err)
	{
		std::cout << "header receive: " << err << std::endl;
		if (!err)
		{
			cql::internal::cql_header_t header;
			std::istream response_stream(&_receive_buffer);
			header.read(response_stream);
			body_read(header);
		}
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
	}

	void
	body_read(const cql::internal::cql_header_t& header)
	{
		boost::asio::async_read(_socket,
								_receive_buffer,
								boost::asio::transfer_exactly(header.length()),
								boost::bind(&cql_client_t::body_read_handle, this, header, boost::asio::placeholders::error));
	}

	void
	body_read_handle(const cql::internal::cql_header_t& header,
					 const boost::system::error_code& err)
	{
		std::cout << "body receive: " << header.str() << std::endl;
		if (!err)
		{
			switch (header.opcode()) {

			case CQL_OPCODE_ERROR:
				error_receive();
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

			case CQL_OPCODE_STARTUP:
			case CQL_OPCODE_CREDENTIALS:
			case CQL_OPCODE_QUERY:
			case CQL_OPCODE_PREPARE:
			case CQL_OPCODE_EXECUTE:
			case CQL_OPCODE_REGISTER:

			case CQL_OPCODE_AUTHENTICATE:
			case CQL_OPCODE_EVENT:


			case CQL_OPCODE_OPTIONS:


			default:
				std::cout << "Error, unhandled opcode: " << header << std::endl;
			}
		}
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
		header_read(); // loop
	}

	void
	startup_write()
	{
		cql::cql_message_startup_t m;
		m.version(CQL_VERSION_IMPL);
		std::cout << "send startup: " << m.str() << std::endl;

		write_message(m,
					  boost::bind(&cql_client_t::write_handle,
								  this,
								  boost::asio::placeholders::error,
								  boost::asio::placeholders::bytes_transferred));
	}

	void
	ready_receive()
	{
		std::istream response_stream(&_receive_buffer);
		cql::cql_message_ready_t m;
		m.read(response_stream);
		std::cout << "ready received: " << m.str() << std::endl;
		if (_connect_callback)
			_connect_callback(*this); // let the call know that the connection is ready
	}

	void
	error_receive()
	{
		std::istream response_stream(&_receive_buffer);
		cql::cql_message_error_t m;
		m.read(response_stream);
		std::cout << "error received: " << m.str() << std::endl;
	}

	void
	supported_receive()
	{
		std::istream response_stream(&_receive_buffer);
		cql::cql_message_supported_t m;
		m.read(response_stream);
		std::cout << "supported received: " << m.str() << std::endl;
		startup_write();
	}

	void
	result_receive(const cql::internal::cql_header_t& header)
	{
		std::istream response_stream(&_receive_buffer);
		cql::cql_message_result_t m;
		m.read(response_stream);

		std::cout << "result received: " << m.str() << std::endl;
		callback_map_t::iterator it = _callback_map.find(header.stream());
		if (it != _callback_map.end())
		{
			(*it).second.get<0>()(*this, header.stream(), m);
			_callback_map.erase(it);
		}
	}

	int8_t											 _stream_counter;
	boost::asio::ip::tcp::resolver					 _resolver;
	boost::asio::ip::tcp::socket					 _socket;
	boost::asio::streambuf							 _receive_buffer;
	boost::asio::streambuf							 _request_buffer;
	callback_map_t                                   _callback_map;
	cql_callback_connection_t                        _connect_callback;
};



void
errback(cql_client_t& client,
		int8_t stream,
		const cql_error_t& err)
{

}

void
use_callback(cql_client_t& client,
			 int8_t stream,
			 const cql::cql_message_result_t& result)
{
	client.query("SELECT * from schema_keyspaces;",
				 CQL_CONSISTENCY_ALL,
				 &use_callback,
				 &errback);
}

void
connect_callback(cql_client_t& client)
{
	client.query("use system;",
				 CQL_CONSISTENCY_ALL,
				 &use_callback,
				 &errback);
}

int
main(int argc,
	 char* argv[])
{
	try
	{
		boost::asio::io_service io_service;
		cql_client_t c(io_service);
		c.connect("localhost", 9042, &connect_callback);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;
}
