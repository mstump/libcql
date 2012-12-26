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

#include <iomanip>
#include <iostream>
#include <istream>
#include <ostream>
#include <stdint.h>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;

#define CQL_FRAME_MAX_SIZE 1024 * 1024 * 256
#define CQL_VERSION_1_REQUEST 0x01
#define CQL_VERSION_1_RESPONSE 0x81

#define CQL_FLAG_NOFLAG 0x00
#define CQL_FLAG_COMPRESSION 0x01
#define CQL_FLAG_TRACE 0x02

#define CQL_OPCODE_ERROR 0x00
#define CQL_OPCODE_STARTUP 0x01
#define CQL_OPCODE_READY 0x02
#define CQL_OPCODE_AUTHENTICATE 0x03
#define CQL_OPCODE_CREDENTIALS 0x04
#define CQL_OPCODE_OPTIONS 0x05
#define CQL_OPCODE_SUPPORTED 0x06
#define CQL_OPCODE_QUERY 0x07
#define CQL_OPCODE_RESULT 0x08
#define CQL_OPCODE_PREPARE 0x09
#define CQL_OPCODE_EXECUTE 0x0A
#define CQL_OPCODE_REGISTER 0x0B
#define CQL_OPCODE_EVENT 0x0C

struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
  return HexCharStruct(_c);
}

#pragma pack(push)
#pragma pack(1)
struct cql_header_t {
	uint8_t  version;
	uint8_t  flags;
	int8_t   stream;
	uint8_t  opcode;
	uint32_t length;

	cql_header_t() :
		version(0),
		flags(0),
		stream(0),
		opcode(0),
		length(0)
	{}

	cql_header_t(uint8_t version,
				 uint8_t flags,
				 int8_t stream,
				 uint8_t opcode,
				 uint32_t length) :
		version(version),
		flags(flags),
		stream(stream),
		opcode(opcode),
		length(length)
	{}

	std::string
	str()
	{
		std::stringstream output;
		output << std::setfill('0');
		output << "{version: 0x" << std::setw(2) << hex(version);
		output << ", flags: 0x" << std::setw(2) << hex(flags);
		output << ", stream: 0x" << std::setw(2) << hex(stream);
		output << ", opcode: 0x" << std::setw(2) << hex(opcode);
		output << ", length: " << boost::lexical_cast<std::string>(length) << "}";
		return output.str();
	}

	friend std::ostream
	&operator<<(std::ostream& output,
				const cql_header_t& h)
	{
		output.put(h.version);
		output.put(h.flags);
		output.put(h.stream);
		output.put(h.opcode);

		uint32_t l = htonl(h.length);
		output.write(reinterpret_cast<char*>(&l), sizeof(l));
		return output;
	}

	friend std::istream
	&operator>>(std::istream& input,
				cql_header_t& h)
	{
		h.version = input.get();
		h.flags = input.get();
		h.stream = input.get();
		h.opcode = input.get();
		input.read(reinterpret_cast<char*>(&h.length), sizeof(h.length));
		h.length = ntohl(h.length);
		return input;
	}

};
#pragma pack(pop)

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

	int8_t
	write(uint8_t opcode,
		  void* data,
		  std::size_t size,
		  boost::function<void (const boost::system::error_code&, std::size_t)>  handler)
	{
		cql_header_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, get_new_stream(), opcode, size);
		std::vector<boost::asio::const_buffer> buffers;
		buffers.push_back(boost::asio::buffer(&header, sizeof(header)));
		buffers.push_back(boost::asio::buffer(data, size));
		boost::asio::async_write(_socket, buffers, handler);
		return header.stream;
	}

	void
	read() {
		boost::asio::async_read(_socket,
								_buffer,
								boost::asio::transfer_exactly(sizeof(cql_header_t)),
								boost::bind(&cql_protocol_t::handle_read_header, this, boost::asio::placeholders::error));
	}


private:

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
			std::cout << "Error: " << err.message() << "\n";
		}
	}

	void
	handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			write(CQL_OPCODE_OPTIONS, NULL, 0,
				  boost::bind(&cql_protocol_t::handle_options,
							  this,
							  boost::asio::placeholders::error,
							  boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			std::cout << "Error: " << err.message() << "\n";
		}
	}

	void
	handle_options(const boost::system::error_code& err,
				   std::size_t)
	{
		if (!err)
		{
			read();
		}
		else
		{
			std::cout << "Error: " << err.message() << "\n";
		}
	}

	void
	handle_write(const boost::system::error_code& err)
	{
		if (!err)
		{
			std::cout << "Write succeeded: " << err.message() << "\n";
		}
		else {
			std::cout << "Write Error: " << err.message() << "\n";
		}
	}

	void
	handle_read_header(const boost::system::error_code& err)
	{
		if (!err)
		{
			cql_header_t header;
			std::istream response_stream(&_buffer);
			response_stream >> header;
			std::cout << header.str() << std::endl;
		}
		else
		{
			std::cout << "Error: " << err.message() << "\n";
		}
	}

	int8_t                          _stream_counter;
	boost::asio::ip::tcp::resolver  _resolver;
	boost::asio::ip::tcp::socket    _socket;
	boost::asio::streambuf          _buffer;
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
		std::cout << "Exception: " << e.what() << "\n";
	}
	return 0;
}
