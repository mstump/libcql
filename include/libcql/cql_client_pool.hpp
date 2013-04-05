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

#include <list>
#include <map>
#include <string>

#include <boost/function.hpp>

#include "libcql/cql.hpp"

namespace cql {

    // Forward declarations
    class cql_client_t;
    class cql_event_t;
    class cql_result_t;
    class cql_execute_t;
    struct cql_error_t;

    class cql_client_pool_t
    {

    public:
        typedef boost::function<cql::cql_client_t*()>                                                  cql_client_callback_t;
        typedef boost::function<void(cql_client_pool_t*)>                                              cql_ready_callback_t;
        typedef boost::function<void(cql_client_pool_t*)>                                              cql_defunct_callback_t;
        typedef boost::function<void(cql_client_pool_t*, cql::cql_client_t&, const cql::cql_error_t&)> cql_connection_errback_t;
        typedef boost::function<void(const cql::cql_short_t, const std::string&)>                      cql_log_callback_t;

        virtual
        ~cql_client_pool_t(){};

        virtual void
        add_client(const std::string& server,
                   unsigned int       port) = 0;

        virtual void
        add_client(const std::string&                      server,
                   unsigned int                            port,
                   cql::cql_client_t::cql_event_callback_t event_callback,
                   const std::list<std::string>&           events) = 0;

        virtual void
        add_client(const std::string&                        server,
                   unsigned int                              port,
                   cql::cql_client_t::cql_event_callback_t   event_callback,
                   const std::list<std::string>&             events,
                   const std::map<std::string, std::string>& credentials) = 0;

        virtual cql::cql_stream_id_t
        query(const std::string&                        query,
              cql_int_t                                 consistency,
              cql::cql_client_t::cql_message_callback_t callback,
              cql::cql_client_t::cql_message_errback_t  errback) = 0;

        virtual cql::cql_stream_id_t
        prepare(const std::string&                        query,
                cql::cql_client_t::cql_message_callback_t callback,
                cql::cql_client_t::cql_message_errback_t  errback) = 0;

        virtual cql::cql_stream_id_t
        execute(cql::cql_execute_t*                       message,
                cql::cql_client_t::cql_message_callback_t callback,
                cql::cql_client_t::cql_message_errback_t  errback) = 0;

        virtual bool
        defunct() = 0;

        virtual bool
        ready() = 0;

        virtual void
        close() = 0;

        virtual size_t
        size() = 0;

        virtual bool
        empty() = 0;
    };

} // namespace cql

#endif // CQL_CLIENT_POOL_H_
