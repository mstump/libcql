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

#ifndef CQL_HEADER_H_
#define CQL_HEADER_H_

#include "util.hpp"

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

namespace cql {
namespace internal {

#pragma pack(push, 1)
class cql_header_t {

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
	str() const
	{
		std::stringstream output;
		output << std::setfill('0');
		output << "{version: 0x" << std::setw(2) << cql::internal::hex(_version);
		output << ", flags: 0x" << std::setw(2) << cql::internal::hex(_flags);
		output << ", stream: 0x" << std::setw(2) << cql::internal::hex(_stream);
		output << ", opcode: 0x" << std::setw(2) << cql::internal::hex(_opcode);
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

	friend std::ostream
	&operator<<(std::ostream& output,
				const cql_header_t& h)
	{
		output << h.str();
        return output;
    }

private:
	uint8_t  _version;
	uint8_t  _flags;
	int8_t   _stream;
	uint8_t  _opcode;
	int32_t  _length;
};
#pragma pack(pop)


} // namespace internal
} // namespace cql

#endif // CQL_HEADER_H_
