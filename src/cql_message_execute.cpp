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

#include <boost/foreach.hpp>
#include <memory>
#include <sstream>

#include "serialization.hpp"
#include "cql_message_execute.hpp"


cql::cql_message_execute_t::cql_message_execute_t() :
    _consistency(0)
{
}

cql::cql_message_execute_t::cql_message_execute_t(const std::vector<cql_byte_t>& id,
                                                  cql_short_t consistency) :
    _query_id(id),
    _consistency(consistency)
{
}

const std::vector<cql_byte_t>&
cql::cql_message_execute_t::query_id() const
{
    return _query_id;
}

void
cql::cql_message_execute_t::query_id(const std::vector<cql_byte_t>& id)
{
    _query_id = id;
}

cql_short_t
cql::cql_message_execute_t::consistency() const
{
    return _consistency;
}

void
cql::cql_message_execute_t::consistency(const cql_short_t consistency)
{
    _consistency = consistency;
}

void
cql::cql_message_execute_t::push_back(const param_t& val)
{
    _params.push_back(new param_t(val.begin(), val.end()));
}

void
cql::cql_message_execute_t::push_back(const std::string& val)
{
    _params.push_back(new param_t(val.begin(), val.end()));
}

void
cql::cql_message_execute_t::push_back(const cql_short_t val)
{
    std::auto_ptr<param_t> p(new param_t);
    cql::internal::encode_short(*p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(const cql_int_t val)
{
    std::auto_ptr<param_t> p(new param_t);
    cql::internal::encode_int(*p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(const cql_bigint_t val)
{
    std::auto_ptr<param_t> p(new param_t);
    cql::internal::encode_bigint(*p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(const float val)
{
    std::auto_ptr<param_t> p(new param_t);
    cql::internal::encode_float(*p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(const double val)
{
    std::auto_ptr<param_t> p(new param_t);
    cql::internal::encode_double(*p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::push_back(const bool val)
{
    std::auto_ptr<param_t> p(new param_t);
    cql::internal::encode_bool(*p, val);
    _params.push_back(p);
}

void
cql::cql_message_execute_t::pop_back()
{
    _params.pop_back();
}

cql::cql_message_execute_t::const_iterator
cql::cql_message_execute_t::begin() const
{
    return _params.begin();
}

cql::cql_message_execute_t::const_iterator
cql::cql_message_execute_t::end() const
{
    return _params.end();
}

cql_byte_t
cql::cql_message_execute_t::opcode() const
{
    return CQL_OPCODE_EXECUTE;
}

cql_int_t
cql::cql_message_execute_t::size() const
{
    std::stringstream ss(std::stringstream::out);
    write(ss);
    return ss.str().size();
}

std::string
cql::cql_message_execute_t::str() const
{
    return "EXECUTE";
}

std::istream&
cql::cql_message_execute_t::read(std::istream& input)
{
    _params.clear();
    cql_short_t count = 0;
    cql::internal::decode_short_bytes(input, _query_id);
    cql::internal::decode_short(input, count);

    for (int i = 0; i < count; ++i) {
        std::auto_ptr<param_t> p(new param_t);
        cql::internal::decode_bytes(input, *p);
        _params.push_back(p);
    }

    cql::internal::decode_short(input, _consistency);
    return input;
}

std::ostream&
cql::cql_message_execute_t::write(std::ostream& output) const
{
    cql::internal::encode_short_bytes(output, _query_id);
    cql::internal::encode_short(output, _params.size());

    BOOST_FOREACH(const param_t& p, _params) {
        cql::internal::encode_bytes(output, p);
    }

    cql::internal::encode_short(output, _consistency);
    return output;
}
