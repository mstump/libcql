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

#include <libcql/cql.h>

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

void
log_callback(
    const cql::cql_short_t,
    const std::string& message)
{
    std::cout << "LOG: " << message << std::endl;
}

void
event_callback(
    cql::cql_client_t&,
    cql::cql_event_t* event)
{
    std::cout << "EVENT RECEIVED: " << event->event_type() << std::endl;
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
        return cql::cql_client_factory_t::create_cql_client_t(_io_service, _log_callback);
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
        boost::asio::io_service io_service;

        std::auto_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(io_service));
        boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_service));

        // decide if we need SSL
        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
        cql::cql_client_pool_t::cql_client_callback_t client_factory;
        if (argc > 1) {
            client_factory = client_ssl_functor_t(io_service, ctx, &log_callback);
        }
        else {
            client_factory = client_functor_t(io_service, &log_callback);
        }
        std::auto_ptr<cql::cql_client_pool_t> pool(cql::cql_client_pool_factory_t::create_client_pool_t(client_factory, NULL, NULL));
        boost::shared_future<cql::cql_future_connection_t> connect_future = pool->add_client("localhost", 9042);
        connect_future.wait();
        std::cout << "connect successfull? ";
        if (!connect_future.get().error.is_err()) {
            std::cout << "TRUE" << std::endl;

            boost::shared_future<cql::cql_future_result_t> future = pool->query("USE system;", cql::CQL_CONSISTENCY_ONE);
            future.wait();
            std::cout << "switch keyspace successfull? " << (!future.get().error.is_err() ? "true" : "false") << std::endl;

            future = pool->query("SELECT * from schema_keyspaces;", cql::CQL_CONSISTENCY_ONE);
            future.wait();
            std::cout << "select successfull? " << (!future.get().error.is_err() ? "true" : "false") << std::endl;
            if (future.get().result) {
                print_rows(*future.get().result);
            }
            pool->close();
        }
        else {
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
