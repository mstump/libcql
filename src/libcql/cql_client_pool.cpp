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
#include "libcql/internal/cql_defines.hpp"
#include "libcql/cql_client_pool.hpp"

cql::cql_client_pool_t::cql_client_pool_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                                          cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                                          cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback) :
    _ready(false),
    _defunct(false),
    _client_callback(client_callback),
    _ready_callback(ready_callback),
    _defunct_callback(defunct_callback),
    _log_callback(NULL),
    _reconnect_limit(0)
{}

cql::cql_client_pool_t::cql_client_pool_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                                          cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                                          cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback,
                                          cql::cql_client_pool_t::cql_log_callback_t     log_callback) :
    _ready(false),
    _defunct(false),
    _client_callback(client_callback),
    _ready_callback(ready_callback),
    _defunct_callback(defunct_callback),
    _log_callback(log_callback),
    _reconnect_limit(0)
{}

cql::cql_client_pool_t::cql_client_pool_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                                          cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                                          cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback,
                                          cql::cql_client_pool_t::cql_log_callback_t     log_callback,
                                          size_t                                         reconnect_limit) :
    _ready(false),
    _defunct(false),
    _client_callback(client_callback),
    _ready_callback(ready_callback),
    _defunct_callback(defunct_callback),
    _log_callback(log_callback),
    _reconnect_limit(reconnect_limit)
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
    std::map<std::string, std::string> credentials;
    add_client(server, port, event_callback, events, credentials);
}


void
cql::cql_client_pool_t::add_client(const std::string&                        server,
                                   unsigned int                              port,
                                   cql::cql_client_t::cql_event_callback_t   event_callback,
                                   const std::list<std::string>&             events,
                                   const std::map<std::string, std::string>& credentials)
{
    if (_client_callback) {
        std::auto_ptr<cql::cql_client_pool_t::client_container_t> client_container(new cql::cql_client_pool_t::client_container_t(_client_callback()));
        client_container->client->connect(server,
                                          port,
                                          boost::bind(&cql_client_pool_t::connect_callback, this, _1),
                                          boost::bind(&cql_client_pool_t::connect_errback, this, _1, _2),
                                          event_callback,
                                          events,
                                          credentials);
        _clients.push_back(client_container);
    }
}

cql::cql_stream_id_t
cql::cql_client_pool_t::query(const std::string&                        query,
                              cql::cql_int_t                                 consistency,
                              cql::cql_client_t::cql_message_callback_t callback,
                              cql::cql_client_t::cql_message_errback_t  errback)
{
    cql_client_t* client = next_client();
    if (client) {
        return client->query(query, consistency, callback, errback);
    }
    return 0;
}

cql::cql_stream_id_t
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

cql::cql_stream_id_t
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
    BOOST_FOREACH(cql::cql_client_pool_t::client_container_t& c, _clients) {
        c.client->close();
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

inline void
cql::cql_client_pool_t::log(cql::cql_short_t level,
    const std::string& message)
{
    if (_log_callback) {
        _log_callback(level, message);
    }
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
    clients_collection_t::iterator pos = _clients.begin();
    for (; pos != _clients.end(); ++pos) {
        cql::cql_client_pool_t::client_container_t& client_container = (*pos);

        if (client_container.client.get() == &client) {

            if (++client_container.errors > _reconnect_limit) {

                if (_connect_errback) {
                    _connect_errback(*this, client, error);
                }

                log(CQL_LOG_ERROR, "client has reached error threshold, removing from pool");
                _clients.erase(pos);
            }
            else {
                log(CQL_LOG_INFO, "attempting to reconnect client");
                client_container.client->reconnect();
            }
            break;
        }
    }

    if (_clients.empty()) {
        log(CQL_LOG_ERROR, "no clients left in pool");

        _ready = false;
        _defunct = true;
        if (_defunct_callback) {
            _defunct_callback(*this);
        }
    }

}

cql::cql_client_t*
cql::cql_client_pool_t::next_client()
{
    cql_client_t* client = NULL;
    if (_ready && !_defunct) {
        client = _clients[_index++].client.get();

        if (_index > _clients.size()) {
            _index = 0;
        }
    }
    return client;
}
