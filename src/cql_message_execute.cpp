/*
  Copyright (c) 2013 Matthew Stump

  This file is part of libcql.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>

#include "cql_vector_stream.hpp"
#include "cql_serialization.hpp"
#include "cql_defines.hpp"
#include "cql_util.hpp"

#include "cql_message_execute.hpp"

cql::cql_message_execute_t::cql_message_execute_t() :
    _buffer(new std::vector<cql_byte_t>()),
    _consistency(CQL_CONSISTENCY_ONE)
{}

cql::cql_message_execute_t::cql_message_execute_t(
    size_t size) :
    _buffer(new std::vector<cql_byte_t>(size)),
    _consistency(CQL_CONSISTENCY_ONE)
{}

cql::cql_message_execute_t::cql_message_execute_t(
    const cql_byte_t* id,
    size_t            id_size) :
    _buffer(new std::vector<cql_byte_t>(id, id + id_size)),
    _consistency(CQL_CONSISTENCY_ONE)
{}

cql::cql_message_execute_t::cql_message_execute_t(
    const std::vector<cql_byte_t>& id,
    cql_short_t                    consistency) :
    _buffer(new std::vector<cql_byte_t>()),
    _query_id(id),
    _consistency(consistency)
{}

cql::cql_message_buffer_t
cql::cql_message_execute_t::buffer()
{
    return _buffer;
}

const std::vector<cql_byte_t>&
cql::cql_message_execute_t::query_id() const
{
    return _query_id;
}

void
cql::cql_message_execute_t::query_id(
    const std::vector<cql_byte_t>& id)
{
    _query_id = id;
}

cql_short_t
cql::cql_message_execute_t::consistency() const
{
    return _consistency;
}

void
cql::cql_message_execute_t::consistency(
    const cql_short_t consistency)
{
    _consistency = consistency;
}

void
cql::cql_message_execute_t::push_back(
    const param_t& val)
{
    _params.push_back(param_t(val.begin(), val.end()));
}

void
cql::cql_message_execute_t::push_back(
    const std::string& val)
{
    _params.push_back(param_t(val.begin(), val.end()));
}

void
cql::cql_message_execute_t::push_back(
    const cql_short_t val)
{
    cql::cql_message_execute_t::param_t p;
    cql::encode_short(p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(
    const cql_int_t val)
{
    cql::cql_message_execute_t::param_t p;
    cql::encode_int(p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(
    const cql_bigint_t val)
{
    cql::cql_message_execute_t::param_t p;
    cql::encode_bigint(p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(
    const float val)
{
    cql::cql_message_execute_t::param_t p;
    cql::encode_float(p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(
    const double val)
{
    cql::cql_message_execute_t::param_t p;
    cql::encode_double(p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(
    const bool val)
{
    cql::cql_message_execute_t::param_t p;
    cql::encode_bool(p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(
    const cql_byte_t* data,
    size_t            data_size)
{
    _params.push_back(cql::cql_message_execute_t::param_t(data, data + data_size));
}

void
cql::cql_message_execute_t::pop_back()
{
    _params.pop_back();
}

cql_byte_t
cql::cql_message_execute_t::opcode() const
{
    return CQL_OPCODE_EXECUTE;
}

cql_int_t
cql::cql_message_execute_t::size() const
{
    return _buffer->size();
}

std::string
cql::cql_message_execute_t::str() const
{
    std::stringstream output;
    output << "EXECUTE";

    if (! _query_id.empty()) {
        output << " 0x";
        output << std::setfill('0');
        BOOST_FOREACH(cql_byte_t c, _query_id) {
            output << std::setw(2) << cql::hex(c);
        }
    }
    output << " " << boost::lexical_cast<std::string>(_consistency);
    return output.str();
}

bool
cql::cql_message_execute_t::consume(
    cql::cql_error_t*)
{
    cql::vector_stream_t buffer(*_buffer);
    std::istream stream(&buffer);

    _params.clear();

    cql_short_t count = 0;
    cql::decode_short_bytes(stream, _query_id);
    cql::decode_short(stream, count);

    for (int i = 0; i < count; ++i) {
        cql::cql_message_execute_t::param_t p;
        cql::decode_bytes(stream, p);
        _params.push_back(p);
    }

    cql::decode_short(stream, _consistency);
    return true;
}

bool
cql::cql_message_execute_t::prepare(
    cql::cql_error_t*)
{
    size_t size = (3 * sizeof(cql_short_t)) + _query_id.size();
    BOOST_FOREACH(const param_t& p, _params) {
        size += p.size() + sizeof(cql_int_t);
    }
    _buffer->resize(size);

    cql::vector_stream_t buffer(*_buffer);
    std::ostream stream(&buffer);

    cql::encode_short_bytes(stream, _query_id);
    cql::encode_short(stream, _params.size());

    BOOST_FOREACH(const param_t& p, _params) {
        cql::encode_bytes(stream, p);
    }

    cql::encode_short(stream, _consistency);
    return true;
}
