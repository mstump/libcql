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

#ifndef CQL_CLIENT_POOL_H_
#define CQL_CLIENT_POOL_H_

#include <istream>
#include <list>
#include <ostream>
#include <string>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "cql.h"
#include "cql_client.hpp"



namespace cql {

    // Forward declarations
    class cql_error_t;
    class cql_message_t;
    class cql_message_event_t;
    class cql_message_result_t;
    class cql_message_prepare_t;
    class cql_message_execute_t;

    class cql_client_pool_t :
        boost::noncopyable
    {

    public:

        typedef boost::function<void(cql_client_pool_t&)> cql_ready_callback_t;
        typedef boost::function<void(cql_client_pool_t&)> cql_defunct_callback_t;
        typedef boost::function<bool(cql_client_pool_t&, cql::cql_client_t&, const cql::cql_error_t&)> cql_connection_errback_t;

        cql_client_pool_t(cql_ready_callback_t ready_callback,
                          cql_defunct_callback_t defunct_callback);

        void
        add_client(cql::cql_client_t* client,
                   const std::string& server,
                   unsigned int port);

        void
        add_client(cql::cql_client_t* client,
                   const std::string& server,
                   unsigned int port,
                   cql::cql_client_t::cql_event_callback_t event_callback,
                   const std::list<std::string>& events);

        cql_stream_id_t
        query(const std::string& query,
              cql_int_t consistency,
              cql::cql_client_t::cql_message_callback_t callback,
              cql::cql_client_t::cql_message_errback_t errback);

        cql_stream_id_t
        prepare(const cql::cql_message_prepare_t& message,
                cql::cql_client_t::cql_message_callback_t callback,
                cql::cql_client_t::cql_message_errback_t errback);

        cql_stream_id_t
        execute(const cql::cql_message_execute_t& message,
                cql::cql_client_t::cql_message_callback_t callback,
                cql::cql_client_t::cql_message_errback_t errback);

        bool
        defunct();

        bool
        ready();

        void
        close();

        size_t
        size();

        bool
        empty();

    private:
        typedef boost::ptr_vector<cql::cql_client_t> clients_collection_t;

        void
        connect_callback(cql::cql_client_t&);

        void
        connect_errback(cql::cql_client_t&,
                        const cql_error_t&);

        cql::cql_client_t*
        next_client();

        unsigned int             _index;
        bool                     _ready;
        bool                     _defunct;
        clients_collection_t     _clients;
        cql_ready_callback_t     _ready_callback;
        cql_defunct_callback_t   _defunct_callback;
        cql_connection_errback_t _connect_errback;
    };

} // namespace cql

#endif // CQL_CLIENT_POOL_H_
