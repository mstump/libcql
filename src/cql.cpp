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

#include <boost/algorithm/string/join.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>

#include "util.hpp"
#include "serialization.hpp"
#include "cql_header.hpp"


// class cql_error_t
// {
// 	cql_error_t() :
// 		_code(0),
// 		_message()
// 	{}

// 	std::string
// 	pretty() const
// 	{
// 		std::stringstream output;
//         output << "[" << _code << "]" << _message;
// 		return output.str();
// 	}

// 	std::ostream&
// 	write(std::ostream& output) const
// 	{
// 		return output;
// 	}

// 	std::istream&
// 	read(std::istream& input)
// 	{
// 		input.read(reinterpret_cast<char*>(&_code), sizeof(_code));
// 		_code = ntohl(_code);
//         cql::internal::decode_string(input, _message);
// 		return input;
// 	}

// 	const std::string&
// 	message() const
// 	{
// 		return _message.str();
// 	}

// 	int32_t
// 	code() const
// 	{
//         return _code;
// 	}

// 	uint32_t
// 	wire_size() const
// 	{
// 		return sizeof(_code) + _message.wire_size();
// 	}

// private:
// 	int32_t			 _code;
// 	std::string      _message;
// };

// class cql_response_t
// {
// private:
// 	bool									 _success;
// 	cql::internal::cql_header_t				 _header;
// 	cql_error_t								 _error;
// 	boost::shared_ptr<cql_serializable_t>	 _response;
// };


//
// cql::internal::cql_header_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, get_new_stream(), opcode, data.wire_size());
// header.write(request_stream);

class cql_message_t
{

public:

virtual void
read(std::istream&) = 0;

virtual void
write(std::ostream&,
	  int8_t stream_id) const = 0;

};

class cql_protocol_t {

public:

	cql_protocol_t(boost::asio::io_service& io_service,
				   const std::string& server,
				   unsigned int port)
		: _resolver(io_service),
		  _socket(io_service)
	{
		boost::asio::ip::tcp::resolver::query query(server, boost::lexical_cast<std::string>(port));
		_resolver.async_resolve(query,
								boost::bind(&cql_protocol_t::handle_resolve,
											this,
											boost::asio::placeholders::error,
											boost::asio::placeholders::iterator));
	}

private:

	typedef boost::function<void (const boost::system::error_code&)> read_callback_t;
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
	enqueue_write(const cql_message_t& data,
				  write_callback_t& callback)
	{
        std::ostream request_stream(&_request_buffer);
        data.write(request_stream, get_new_stream());
        boost::asio::async_write(_socket, _request_buffer, callback);
    }

	void
	read_header()
	{
		boost::asio::async_read(_socket,
								_receive_buffer,
								boost::asio::transfer_exactly(sizeof(cql::internal::cql_header_t)),
								boost::bind(&cql_protocol_t::handle_read_header, this, boost::asio::placeholders::error));
	}

	void
	read_body(const cql::internal::cql_header_t& header)
	{
		boost::asio::async_read(_socket,
								_receive_buffer,
								boost::asio::transfer_exactly(header.length()),
								boost::bind(&cql_protocol_t::handle_read_body, this, header, boost::asio::placeholders::error));
	}

	void
	handle_read_header(const boost::system::error_code& err)
	{
		if (!err)
		{
			cql::internal::cql_header_t header;
			std::istream response_stream(&_receive_buffer);
			header.read(response_stream);
			read_body(header);
		}
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
	}

	void
	handle_read_body(const cql::internal::cql_header_t header,
					 const boost::system::error_code& err)
	{
		if (!err)
		{
			std::istream response_stream(&_receive_buffer);
			switch (header.opcode()) {

				// client only
			case CQL_OPCODE_STARTUP:
			case CQL_OPCODE_CREDENTIALS:
			case CQL_OPCODE_SUPPORTED:
			case CQL_OPCODE_QUERY:
			case CQL_OPCODE_PREPARE:
			case CQL_OPCODE_EXECUTE:
			case CQL_OPCODE_REGISTER:

				break;

				// unprompted
			case CQL_OPCODE_AUTHENTICATE:
			case CQL_OPCODE_EVENT:
			case CQL_OPCODE_READY:
				break;

				// result of user/proto behavior
			case CQL_OPCODE_OPTIONS:
			case CQL_OPCODE_ERROR:
			case CQL_OPCODE_RESULT:
				break;

			default:
				std::cout << "Error, unhandled opcode: " << header << std::endl;
			}
			// if (_callbacks.find(header.stream) != my_map.end()) {
			// 	callback = _callbacks[header.stream]();
			// }
		}
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
	}

	void
	handle_resolve(const boost::system::error_code& err,
				   boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		if (!err)
		{
			// Attempt a connection to each endpoint in the list until we
			// successfully establish a connection.
			boost::asio::async_connect(_socket,
									   endpoint_iterator,
									   boost::bind(&cql_protocol_t::handle_connect,
												   this,
												   boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
	}

	void
	handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			// enqueue_write(CQL_OPCODE_OPTIONS,
			// 			  boost::bind(&cql_protocol_t::handle_options,
			// 						  this,
			// 						  boost::asio::placeholders::error,
			// 						  boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
	}


	void
	handle_options(const boost::system::error_code& err,
				   std::size_t)
	{
		if (!err)
		{
			read_header();
		}
		else
		{
			std::cout << "Error: " << err.message() << std::endl;
		}
	}

	int8_t											 _stream_counter;
	boost::asio::ip::tcp::resolver					 _resolver;
	boost::asio::ip::tcp::socket					 _socket;
	boost::asio::streambuf							 _receive_buffer;
	boost::asio::streambuf							 _request_buffer;
	//	boost::unordered_map<int8_t, cql_callback_t>	 _callbacks;
};

int main(int argc, char* argv[]) {
	try
	{
		boost::asio::io_service io_service;
		cql_protocol_t p(io_service, "localhost", 9042);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;
}
