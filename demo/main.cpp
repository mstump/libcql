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

#include <boost/bind.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "cql.h"
#include "cql_error.hpp"
#include "cql_client.hpp"
#include "cql_client_factory.hpp"
#include "cql_client_pool.hpp"
#include "cql_message_event.hpp"
#include "cql_message_execute.hpp"
#include "cql_message_prepare.hpp"
#include "cql_message_result.hpp"
#include "cql_row.hpp"

void
message_errback(cql::cql_client_t& client,
                int8_t stream,
                const cql::cql_error_t& err)
{
    std::cerr << "ERROR " << err.message() << std::endl;
}

void
connection_errback(cql::cql_client_t& client,
                   const cql::cql_error_t& err)
{
    std::cerr << "ERROR " << boost::lexical_cast<std::string>(err.application_error()) << " " << err.message() << std::endl;
}

void
select_callback(cql::cql_client_t& client,
                int8_t stream,
                const cql::cql_message_result_t& result)
{
    BOOST_FOREACH(const cql::cql_row_t& row, result)
    {
        std::cout << row.str() << std::endl;
    }

    client.close();
}

void
execute_callback(cql::cql_client_t& client,
                 int8_t stream,
                 const cql::cql_message_result_t& result)
{
    BOOST_FOREACH(const cql::cql_row_t& row, result)
    {
        std::cout << row.str() << std::endl;
    }

    client.query("SELECT * from schema_keyspaces;",
                  CQL_CONSISTENCY_ALL,
                  &select_callback,
                  &message_errback);
}

void
prepare_callback(cql::cql_client_t& client,
                 int8_t stream,
                 const cql::cql_message_result_t& result)
{
    cql::cql_message_execute_t m(result.query_id(), CQL_CONSISTENCY_ALL);
    client.execute(m,
                    &execute_callback,
                    &message_errback);
}

void
use_callback(cql::cql_client_t& client,
             int8_t stream,
             const cql::cql_message_result_t& result)
{
    cql::cql_message_prepare_t m("SELECT * from schema_keyspaces;");
    client.prepare(m,
                    &prepare_callback,
                    &message_errback);
}

void
connect_callback(cql::cql_client_pool_t& pool)
{
    pool.query("USE system;",
               CQL_CONSISTENCY_ALL,
               &use_callback,
               &message_errback);
}

void
log_callback(const cql_short_t level,
             const std::string& message)
{
    std::cout << "LOG: " << message << std::endl;
}

void
event_callback(cql::cql_client_t& client,
               const cql::cql_message_event_t& event)
{
    std::cout << "EVENT RECEIVED: " << event.str() << std::endl;
}

struct client_functor_t
{

public:

    client_functor_t(boost::asio::io_service&              service,
                     cql::cql_client_t::cql_log_callback_t log_callback) :
        _io_service(service),
        _log_callback(log_callback)
    {}

    cql::cql_client_t*
    operator()()
    {
        return cql::create_cql_client_t(_io_service, _log_callback);
    }

private:
    boost::asio::io_service&              _io_service;
    cql::cql_client_t::cql_log_callback_t _log_callback;
};

struct client_ssl_functor_t
{

public:

    client_ssl_functor_t(boost::asio::io_service&              service,
                         boost::asio::ssl::context&            context,
                         cql::cql_client_t::cql_log_callback_t log_callback) :
        _io_service(service),
        _ssl_ctx(context),
        _log_callback(log_callback)
    {}

    cql::cql_client_t*
    operator()()
    {
        return cql::create_cql_client_t(_io_service, _ssl_ctx, _log_callback);
    }

private:
    boost::asio::io_service&              _io_service;
    boost::asio::ssl::context&            _ssl_ctx;
    cql::cql_client_t::cql_log_callback_t _log_callback;
};


int
main(int argc,
     char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);

        cql::cql_client_pool_t::cql_client_callback_t client_factory;
        if (argc > 1) {
            client_factory = client_ssl_functor_t(io_service, ctx, &log_callback);
        }
        else {
            client_factory = client_functor_t(io_service, &log_callback);
        }
        cql::cql_client_pool_t pool(client_factory, &connect_callback, NULL);

        std::list<std::string> events;
        events.push_back("SCHEMA_CHANGE");

        pool.add_client("localhost", 9042, NULL, events);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
