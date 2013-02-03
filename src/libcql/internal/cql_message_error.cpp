#include <sstream>
#include <boost/foreach.hpp>
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_serialization.hpp"

#include "libcql/internal/cql_message_error.hpp"

cql::cql_message_error_t::cql_message_error_t() :
    _code(0),
    _message()
{}

cql::cql_message_error_t::cql_message_error_t(cql::cql_int_t code,
                                              const std::string& message) :
    _code(code),
    _message(message)
{}

const std::string&
cql::cql_message_error_t::message() const
{
    return _message;
}

void
cql::cql_message_error_t::message(const std::string& m)
{
    _message = m;
}

cql::cql_int_t
cql::cql_message_error_t::code() const
{
    return _code;
}

void
cql::cql_message_error_t::code(cql::cql_int_t c)
{
    _code = c;
}

cql::cql_byte_t
cql::cql_message_error_t::opcode() const
{
    return CQL_OPCODE_ERROR;
}

cql::cql_int_t
cql::cql_message_error_t::size() const
{
    std::stringstream ss(std::stringstream::out);
    write(ss);
    return ss.str().size();
}

std::string
cql::cql_message_error_t::str() const
{
    return _message;
}

std::istream&
cql::cql_message_error_t::read(std::istream& input)
{
    input.read(reinterpret_cast<char*>(&_code), sizeof(_code));
    _code = ntohl(_code);
    cql::internal::decode_string(input, _message);
    return input;
}

std::ostream&
cql::cql_message_error_t::write(std::ostream& output) const
{
    cql::cql_int_t code = htonl(_code);
    output.write(reinterpret_cast<char*>(&code), sizeof(code));
    cql::internal::encode_string(output, _message);
    return output;
}
