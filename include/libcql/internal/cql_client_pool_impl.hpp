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

#ifndef CQL_CLIENT_POOL_IMPL_H_
#define CQL_CLIENT_POOL_IMPL_H_

#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <string>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "libcql/cql.hpp"
#include "libcql/cql_client.hpp"
#include "libcql/cql_client_pool.hpp"

namespace cql {

    // Forward declarations
    class cql_error_t;
    class cql_event_t;
    class cql_result_t;
    class cql_execute_t;

    class cql_client_pool_impl_t :
        public cql_client_pool_t,
        boost::noncopyable
    {

    public:
        cql_client_pool_impl_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                               cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                               cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback);

        cql_client_pool_impl_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                               cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                               cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback,
                               cql::cql_client_pool_t::cql_log_callback_t     log_callback);

        cql_client_pool_impl_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                               cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                               cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback,
                               cql::cql_client_pool_t::cql_log_callback_t     log_callback,
                               size_t                                         reconnect_limit);

        void
        add_client(const std::string& server,
                   unsigned int       port);

        void
        add_client(const std::string&                      server,
                   unsigned int                            port,
                   cql::cql_client_t::cql_event_callback_t event_callback,
                   const std::list<std::string>&           events);

        void
        add_client(const std::string&                        server,
                   unsigned int                              port,
                   cql::cql_client_t::cql_event_callback_t   event_callback,
                   const std::list<std::string>&             events,
                   const std::map<std::string, std::string>& credentials);

        cql::cql_stream_id_t
        query(const std::string&                        query,
              cql_int_t                                 consistency,
              cql::cql_client_t::cql_message_callback_t callback,
              cql::cql_client_t::cql_message_errback_t  errback);

        cql::cql_stream_id_t
        prepare(const std::string&                        query,
                cql::cql_client_t::cql_message_callback_t callback,
                cql::cql_client_t::cql_message_errback_t  errback);

        cql::cql_stream_id_t
        execute(cql::cql_execute_t*                       message,
                cql::cql_client_t::cql_message_callback_t callback,
                cql::cql_client_t::cql_message_errback_t  errback);

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

        struct client_container_t
        {
            client_container_t(cql::cql_client_t* c) :
                client(c),
                errors(0)
            {}

            std::auto_ptr<cql::cql_client_t> client;
            size_t errors;
        };

        typedef boost::ptr_vector<client_container_t> clients_collection_t;

        inline void
        log(cql::cql_short_t level,
            const std::string& message);

        void
        connect_callback(cql::cql_client_t&);

        void
        connect_errback(cql::cql_client_t&,
                        const cql_error_t&);

        cql::cql_client_t*
        next_client();

        unsigned int                                     _index;
        bool                                             _ready;
        bool                                             _defunct;
        clients_collection_t                             _clients;
        cql::cql_client_pool_t::cql_client_callback_t    _client_callback;
        cql::cql_client_pool_t::cql_ready_callback_t     _ready_callback;
        cql::cql_client_pool_t::cql_defunct_callback_t   _defunct_callback;
        cql::cql_client_pool_t::cql_log_callback_t       _log_callback;
        cql::cql_client_pool_t::cql_connection_errback_t _connect_errback;
        size_t                                           _reconnect_limit;
    };

} // namespace cql

#endif // CQL_CLIENT_POOL_IMPL_H_
