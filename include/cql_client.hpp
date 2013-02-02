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

#ifndef CQL_CLIENT_H_
#define CQL_CLIENT_H_

#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <string>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include "cql.hpp"


namespace cql {

    // Forward declarations
    class cql_error_t;
    class cql_message_t;
    class cql_message_event_t;
    class cql_message_result_t;
    class cql_message_prepare_t;
    class cql_message_execute_t;

    class cql_client_t :
        boost::noncopyable
    {

    public:

        typedef boost::function<void(const cql::cql_short_t, const std::string&)> cql_log_callback_t;

        typedef boost::function<void(cql_client_t&)> cql_connection_callback_t;
        typedef boost::function<void(cql_client_t&, const cql_error_t&)> cql_connection_errback_t;
        typedef boost::function<void(cql_client_t&, const cql_message_event_t&)> cql_event_callback_t;

        typedef boost::function<void(cql_client_t&, const cql::cql_stream_id_t, const cql::cql_message_result_t&)> cql_message_callback_t;
        typedef boost::function<void(cql_client_t&, const cql::cql_stream_id_t, const cql_error_t&)> cql_message_errback_t;

        typedef std::map<std::string, std::string> cql_credentials_t;

        virtual
        ~cql_client_t(){};

        virtual void
        connect(const std::string& server,
                unsigned int port,
                cql_connection_callback_t callback,
                cql_connection_errback_t errback) = 0;

        virtual void
        connect(const std::string& server,
                unsigned int port,
                cql_connection_callback_t callback,
                cql_connection_errback_t errback,
                cql_event_callback_t event_callback,
                const std::list<std::string>& events) = 0;

        virtual void
        connect(const std::string& server,
                unsigned int port,
                cql_connection_callback_t callback,
                cql_connection_errback_t errback,
                cql_event_callback_t event_callback,
                const std::list<std::string>& events,
                cql_credentials_t credentials) = 0;

        virtual cql::cql_stream_id_t
        query(const std::string& query,
              cql_int_t consistency,
              cql_message_callback_t callback,
              cql_message_errback_t errback) = 0;

        virtual cql::cql_stream_id_t
        prepare(const cql::cql_message_prepare_t& message,
                cql_message_callback_t callback,
                cql_message_errback_t errback) = 0;

        virtual cql::cql_stream_id_t
        execute(const cql::cql_message_execute_t& message,
                cql_message_callback_t callback,
                cql_message_errback_t errback) = 0;

        virtual bool
        defunct() const = 0;

        virtual bool
        ready() const = 0;

        virtual void
        close() = 0;

        virtual void
        close(cql_error_t& err) = 0;

        virtual const std::string&
        server() const = 0;

        virtual unsigned int
        port() const = 0;

        virtual const std::list<std::string>&
        events() const = 0;

        virtual cql_event_callback_t
        event_callback() const = 0;

        virtual const cql_credentials_t&
        credentials() const = 0;

        virtual void
        reconnect() = 0;
    };

} // namespace cql

#endif // CQL_CLIENT_H_
