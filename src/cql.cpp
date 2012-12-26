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
#include <boost/algorithm/string/join.hpp>

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

class cql_base_t
{

public:

	virtual std::string
	pretty() const = 0;

	virtual std::ostream&
	write(std::ostream& output) const = 0;

	virtual std::istream&
	read(std::istream& input) = 0;

    virtual uint32_t
	wire_size() const = 0;

	friend std::ostream
	&operator<<(std::ostream& output,
				const cql_base_t& h)
	{
		output << h.pretty();
        return output;
    }
};


class cql_header_t :
	public cql_base_t
{

public:

	cql_header_t() :
		_version(0),
		_flags(0),
		_stream(0),
		_opcode(0),
		_length(0)
	{}

	cql_header_t(uint8_t version,
				 uint8_t flags,
				 int8_t stream,
				 uint8_t opcode,
				 uint32_t length) :
		_version(version),
		_flags(flags),
		_stream(stream),
		_opcode(opcode),
		_length(length)
	{}

	std::string
	pretty() const
	{
		std::stringstream output;
		output << std::setfill('0');
		output << "{version: 0x" << std::setw(2) << hex(_version);
		output << ", flags: 0x" << std::setw(2) << hex(_flags);
		output << ", stream: 0x" << std::setw(2) << hex(_stream);
		output << ", opcode: 0x" << std::setw(2) << hex(_opcode);
		output << ", length: " << boost::lexical_cast<std::string>(_length) << "}";
		return output.str();
	}

	std::ostream&
	write(std::ostream& output) const
	{
		output.put(_version);
		output.put(_flags);
		output.put(_stream);
		output.put(_opcode);

		uint32_t l = htonl(_length);
		output.write(reinterpret_cast<char*>(&l), sizeof(l));
		return output;
	}

	std::istream&
	read(std::istream& input)
	{
		_version = input.get();
		_flags = input.get();
		_stream = input.get();
		_opcode = input.get();
		input.read(reinterpret_cast<char*>(&_length), sizeof(_length));
		_length = ntohl(_length);
		return input;
	}

    uint32_t
	wire_size() const
	{
		return sizeof(_version) + sizeof(_flags) + sizeof(_stream) + sizeof(_opcode) + sizeof(_length);
    }

	uint8_t
	version() const
	{
		return _version;
	}

	uint8_t
	flags() const
	{
		return _flags;
	}

	int8_t
	stream() const
	{
		return _stream;
	}

	uint8_t
	opcode() const
	{
		return _opcode;
	}

	uint32_t
	length() const
	{
		return _length;
	}

	void
	version(uint8_t v)
	{
		_version = v;
	}

	void
	flags(uint8_t v)
	{
		_flags = v;
	}

	void
	stream(int8_t v)
	{
		_stream = v;
	}

	void
	opcode(uint8_t v)
	{
	    _opcode = v;
	}

	void
	length(uint32_t v)
	{
		_length = v;
	}

private:
	uint8_t  _version;
	uint8_t  _flags;
	int8_t   _stream;
	uint8_t  _opcode;
	uint32_t _length;
};

class cql_string_t :
	public cql_base_t
{

public:

	cql_string_t() :
		_value()
	{}

	cql_string_t(std::string& s) :
		_value(s)
	{}

	std::string
	pretty() const
	{
		std::stringstream output;
        output << "[" << _value.size() << "]" << _value;
		return output.str();
	}

	std::ostream&
	write(std::ostream& output) const
	{
		uint16_t len = htons(_value.size());
		output.write(reinterpret_cast<char*>(&len), sizeof(len));
		output.write(reinterpret_cast<const char*>(_value.c_str()), _value.size());
		return output;
	}

	std::istream&
	read(std::istream& input)
	{
		uint16_t len;
		input.read(reinterpret_cast<char*>(&len), sizeof(len));
		len = ntohs(len);

		std::vector<char> buffer(len);
		input.read(&buffer[0], len);
		_value.assign(buffer.begin(), buffer.end());
		return input;
	}

	uint32_t
	wire_size() const
	{
		return sizeof(uint16_t) + _value.size();
	}

private:
	std::string _value;
};

class cql_string_list_t :
	public cql_base_t
{

public:

	std::string
	pretty() const
	{
		std::stringstream output;
		std::list<std::string> my_pretties;
		std::transform(_values.begin(), _values.end(), back_inserter(my_pretties), boost::bind(&cql_string_t::pretty, _1));
        output << "[" << _values.size() << "][" << boost::algorithm::join(my_pretties, ", ") << "]";
		return output.str();
	}

	std::ostream&
	write(std::ostream& output) const
	{
		uint16_t len = htons(_values.size());
		output.write(reinterpret_cast<char*>(&len), sizeof(len));
		BOOST_FOREACH(const cql_string_t& s, _values)
			s.write(output);
		return output;
	}

	std::istream&
	read(std::istream& input)
	{
		uint16_t len;
		input.read(reinterpret_cast<char*>(&len), sizeof(len));
		len = ntohs(len);

		for (int i = 0; i < len; i++) {
			cql_string_t s;
			s.read(input);
			_values.push_back(s);
		}

		return input;
	}

private:
	std::list<cql_string_t> _values;
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

	int8_t
	write(uint8_t opcode,
		  boost::function<void (const boost::system::error_code&, std::size_t)>  handler)
	{
		std::ostream request_stream(&_request_buffer);
		cql_header_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, get_new_stream(), opcode, 0);
		header.write(request_stream);
		boost::asio::async_write(_socket, _request_buffer, handler);
		return header.stream();
	}

	int8_t
	write(uint8_t opcode,
		  cql_base_t& data,
		  boost::function<void (const boost::system::error_code&, std::size_t)>  handler)
	{
		std::ostream request_stream(&_request_buffer);
		cql_header_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, get_new_stream(), opcode, data.wire_size());
		header.write(request_stream);
		data.write(request_stream);
		boost::asio::async_write(_socket, _request_buffer, handler);
		return header.stream();
	}

	void
	read() {
		boost::asio::async_read(_socket,
								_receive_buffer,
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
			write(CQL_OPCODE_OPTIONS,
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
			std::istream response_stream(&_receive_buffer);
			header.read(response_stream);
			std::cout << header << std::endl;
		}
		else
		{
			std::cout << "Error: " << err.message() << "\n";
		}
	}

	int8_t                          _stream_counter;
	boost::asio::ip::tcp::resolver  _resolver;
	boost::asio::ip::tcp::socket    _socket;
	boost::asio::streambuf          _receive_buffer;
	boost::asio::streambuf          _request_buffer;
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
