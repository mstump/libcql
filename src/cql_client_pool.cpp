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

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include "cql_client_pool.hpp"

cql::cql_client_pool_t::cql_client_pool_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                                          cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                                          cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback) :
    _ready(false),
    _defunct(false),
    _client_callback(client_callback),
    _ready_callback(ready_callback),
    _defunct_callback(defunct_callback)
{}

void
cql::cql_client_pool_t::add_client(const std::string& server,
                                   unsigned int       port)
{
    std::list<std::string> e;
    add_client(server, port, NULL, e);
}

void
cql::cql_client_pool_t::add_client(const std::string&                      server,
                                   unsigned int                            port,
                                   cql::cql_client_t::cql_event_callback_t event_callback,
                                   const std::list<std::string>&           events)
{
    if (_client_callback) {
        cql::cql_client_t* client = _client_callback();
        _clients.push_back(client);
        client->connect(server,
                        port,
                        boost::bind(&cql_client_pool_t::connect_callback, this, _1),
                        boost::bind(&cql_client_pool_t::connect_errback, this, _1, _2),
                        event_callback,
                        events);
    }
}

cql_stream_id_t
cql::cql_client_pool_t::query(const std::string&                        query,
                              cql_int_t                                 consistency,
                              cql::cql_client_t::cql_message_callback_t callback,
                              cql::cql_client_t::cql_message_errback_t  errback)
{
    cql_client_t* client = next_client();
    if (client) {
        return client->query(query, consistency, callback, errback);
    }
    return 0;
}

cql_stream_id_t
cql::cql_client_pool_t::prepare(const cql::cql_message_prepare_t&         message,
                                cql::cql_client_t::cql_message_callback_t callback,
                                cql::cql_client_t::cql_message_errback_t  errback)
{
    cql_client_t* client = next_client();
    if (client) {
        return client->prepare(message, callback, errback);
    }
    return 0;
}

cql_stream_id_t
cql::cql_client_pool_t::execute(const cql::cql_message_execute_t&         message,
                                cql::cql_client_t::cql_message_callback_t callback,
                                cql::cql_client_t::cql_message_errback_t  errback)
{
    cql_client_t* client = next_client();
    if (client) {
        return client->execute(message, callback, errback);
    }
    return 0;
}

bool
cql::cql_client_pool_t::defunct()
{
    return _defunct;
}

bool
cql::cql_client_pool_t::ready()
{
    return _ready;
}

void
cql::cql_client_pool_t::close()
{
    BOOST_FOREACH(cql::cql_client_t& c, _clients) {
        c.close();
    }
}

size_t
cql::cql_client_pool_t::size()
{
    return _clients.size();
}

bool
cql::cql_client_pool_t::empty()
{
    return _clients.empty();
}

void
cql::cql_client_pool_t::connect_callback(cql::cql_client_t&)
{
    if (_defunct) {
        _defunct = false;
    }

    if (!_ready) {
        _ready = true;
        if (_ready_callback) {
            _ready_callback(*this);
        }
    }
}

void
cql::cql_client_pool_t::connect_errback(cql::cql_client_t& client,
                                        const cql_error_t& error)
{
    bool keep = false;
    if (_connect_errback) {
        keep = _connect_errback(*this, client, error);
    }
    if (!keep) {

        clients_collection_t::iterator pos = _clients.begin();
        for (; pos != _clients.end(); ++pos) {
            if (&(*pos) == &client) {
                _clients.erase(pos);
                break;
            }
        }

        if (_clients.empty()) {
            _ready = false;
            _defunct = true;
            if (_defunct_callback) {
                _defunct_callback(*this);
            }
        }
    }
}

cql::cql_client_t*
cql::cql_client_pool_t::next_client()
{
    cql_client_t* client = NULL;
    if (_ready && !_defunct) {
        client = &(_clients[_index++]);

        if (_index > _clients.size()) {
            _index = 0;
        }
    }
    return client;
}
