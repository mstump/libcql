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

#include <boost/bind.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <libcql/cql.hpp>
#include <libcql/cql_error.hpp>
#include <libcql/cql_event.hpp>
#include <libcql/cql_client.hpp>
#include <libcql/cql_client_factory.hpp>
#include <libcql/cql_client_pool.hpp>
#include <libcql/cql_client_pool_factory.hpp>
#include <libcql/cql_execute.hpp>
#include <libcql/cql_result.hpp>

// helper function to print query results
void
print_rows(
    cql::cql_result_t& result)
{
    while (result.next()) {
        for (size_t i = 0; i < result.column_count(); ++i) {
            cql::cql_byte_t* data = NULL;
            cql::cql_int_t size = 0;
            result.get_data(i, &data, size);
            std::cout.write(reinterpret_cast<char*>(data), size);
            std::cout << " | ";
        }
        std::cout << std::endl;
    }
}

// This function is called asynchronously every time an event is logged
void
log_callback(
    const cql::cql_short_t,
    const std::string& message)
{
    std::cout << "LOG: " << message << std::endl;
}

// This is a non-SSL client factory
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
        // called every time the pool needs to initiate a new connection to a host
        return cql::cql_client_factory_t::create_cql_client_t(_io_service, _log_callback);
    }

private:
    boost::asio::io_service&              _io_service;
    cql::cql_client_t::cql_log_callback_t _log_callback;
};


// This is an SSL client factory
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
        // called every time the pool needs to initiate a new connection to a host
        return cql::cql_client_factory_t::create_cql_client_t(_io_service, _ssl_ctx, _log_callback);
    }

private:
    boost::asio::io_service&              _io_service;
    boost::asio::ssl::context&            _ssl_ctx;
    cql::cql_client_t::cql_log_callback_t _log_callback;
};

int
main(int argc,
     char**)
{
    try
    {
        // Initialize the IO service, this allows us to perform network operations asyncronously
        boost::asio::io_service io_service;

        // Typically async operations are performed in the thread performing the request, because we want synchronous behavior
        // we're going to spawn a thread whose sole purpose is to perform network communication, and we'll use this thread to
        // initiate and check the status of requests.
        //
        // Also, typically the boost::asio::io_service::run will exit as soon as it's work is done, which we want to prevent
        // because it's in it's own thread.  Using boost::asio::io_service::work prevents the thread from exiting.
        std::auto_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(io_service));
        boost::thread thread(boost::bind(static_cast<size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run), &io_service));

        // decide which client factory we want, SSL or non-SSL.  This is a hack, if you pass any commandline arg to the
        // binary it will use the SSL factory, non-SSL by default
        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
        cql::cql_client_pool_t::cql_client_callback_t client_factory;
        if (argc > 1) {
            client_factory = client_ssl_functor_t(io_service, ctx, &log_callback);
        }
        else {
            client_factory = client_functor_t(io_service, &log_callback);
        }

        // Construct the pool
        std::auto_ptr<cql::cql_client_pool_t> pool(cql::cql_client_pool_factory_t::create_client_pool_t(client_factory, NULL, NULL));

        // Add a client to the pool, this operation returns a future.
        boost::shared_future<cql::cql_future_connection_t> connect_future = pool->add_client("localhost", 9042);

        // Wait until the connection is complete, or has failed.
        connect_future.wait();

        // Check whether or not the connection was successful.
        std::cout << "connect successfull? ";
        if (!connect_future.get().error.is_err()) {
            // The connection succeeded
            std::cout << "TRUE" << std::endl;

            // execute a query, switch keyspaces
            boost::shared_future<cql::cql_future_result_t> future = pool->query("USE system;", cql::CQL_CONSISTENCY_ONE);

            // wait for the query to execute
            future.wait();

            // check whether the query succeeded
            std::cout << "switch keyspace successfull? " << (!future.get().error.is_err() ? "true" : "false") << std::endl;

            // execute a query, select all rows from the keyspace
            future = pool->query("SELECT * from schema_keyspaces;", cql::CQL_CONSISTENCY_ONE);

            // wait for the query to execute
            future.wait();

            // check whether the query succeeded
            std::cout << "select successfull? " << (!future.get().error.is_err() ? "true" : "false") << std::endl;
            if (future.get().result) {
                // print the rows return by the successful query
                print_rows(*future.get().result);
            }

            // close the connection pool
            pool->close();
        }
        else {
            // The connection failed
            std::cout << "FALSE" << std::endl;
        }

        work.reset();
        thread.join();

        std::cout << "THE END" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
