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

#include <boost/asio.hpp>
#include "libcql/cql.hpp"
#include "libcql/cql_client_factory.hpp"
#include "libcql/cql_client_pool.hpp"
#include "libcql/cql_client_pool_factory.hpp"
#include "libcql/c.h"

struct cql_context_t :
    boost::noncopyable
{
    cql_context_t() :
        io_work(io_service),
        io_thread(boost::bind(&boost::asio::io_service::run, &io_service)),
        log_callback(NULL)
    {}

    cql::cql_client_t*
    create_client()
    {
        return cql::cql_client_factory_t::create_cql_client_t(io_service, log_callback);
    }

    cql::cql_client_t*
    create_client_ssl()
    {
        return cql::cql_client_factory_t::create_cql_client_t(io_service, log_callback);
    }

    boost::asio::io_service               io_service;
    boost::asio::io_service::work         io_work;
    boost::thread                         io_thread;
    std::auto_ptr<cql::cql_client_pool_t> pool;
    cql::cql_client_t::cql_log_callback_t log_callback;
};

struct future_base_t
{
    enum type_enum {
        CONNECTION_FUTURE,
        RESULT_FUTURE };

    virtual type_enum
    type() = 0;

    virtual bool
    ready() = 0;

    virtual void
    wait() = 0;

    virtual bool
    is_err() = 0;

    virtual int
    error_code() = 0;

    virtual void
    error_message(
        char** message) = 0;
};

template<typename T>
struct future_t :
    public future_base_t
{
    T                        future;
    future_base_t::type_enum future_type;

    future_t(
        T future,
        future_base_t::type_enum future_type) :
        future(future),
        future_type(future_type)
    {}

    inline future_base_t::type_enum
    type()
    {
        return future_type;
    }

    inline bool
    ready()
    {
        return future.is_ready();
    }

    inline void
    wait()
    {
        future.wait();
    }

    inline bool
    is_err()
    {
        return future.get().error.is_err();
    }

    inline int
    error_code()
    {
        return future.get().error.code;
    }

    inline void
    error_message(
        char** message)
    {
        *message = strdup(future.get().error.message.c_str());
    }
};

void*
cql_context_new()
{
    return new cql_context_t;
}

void
cql_context_free(
    void* context)
{
    delete static_cast<cql_context_t*>(context);
}

bool
cql_init(
    void*  context,
    void** status)
{
    (void) status;
    static_cast<cql_context_t*>(context)->pool.reset(
        cql::cql_client_pool_factory_t::create_client_pool_t(
            boost::bind(&cql_context_t::create_client, static_cast<cql_context_t*>(context)),
            NULL,
            NULL));
    return true;
}

bool
cql_init_ssl(
    void*  context,
    void** status)
{
    (void) status;
    static_cast<cql_context_t*>(context)->pool.reset(
        cql::cql_client_pool_factory_t::create_client_pool_t(
            boost::bind(&cql_context_t::create_client_ssl, static_cast<cql_context_t*>(context)),
            NULL,
            NULL));
    return true;
}

void
cql_close(
    void*  context)
{
    static_cast<cql_context_t*>(context)->pool->close();
}

void*
cql_add_client(
    void*          context,
    void*          host,
    size_t         host_size,
    unsigned short port)
{
    return new future_t<boost::shared_future<cql::cql_future_connection_t> >(
        static_cast<cql_context_t*>(context)->pool->add_client(
            std::string(static_cast<char*>(host), host_size),
            port),
        future_base_t::CONNECTION_FUTURE);
}

bool
cql_future_ready(
    void* context,
    void* future)
{
    (void) context;
    return static_cast<future_base_t*>(future)->ready();
}

void
cql_future_wait(
    void* context,
    void* future)
{
    (void) context;
    return static_cast<future_base_t*>(future)->wait();
}

bool
cql_future_success(
    void* context,
    void* future)
{
    (void) context;
    return !static_cast<future_base_t*>(future)->is_err();
}

int
cql_future_get_error_code(
    void* context,
    void* future)
{
    (void) context;
    return static_cast<future_base_t*>(future)->error_code();
}

void
cql_future_get_error_message(
    void*  context,
    void*  future,
    char** message)
{
    (void) context;
    static_cast<future_base_t*>(future)->error_message(message);
}


void*
cql_query(
    void*       context,
    const char* query,
    size_t      query_size,
    int         consistency)
{
    std::string query_string(query, query_size);
    return new future_t<boost::shared_future<cql::cql_future_result_t> >(
        static_cast<cql_context_t*>(context)->pool->query(query_string, consistency),
        future_base_t::RESULT_FUTURE);
}

void*
cql_prepare(
    void*       context,
    const char* query,
    size_t      query_size)
{
    std::string query_string(query, query_size);
    return new future_t<boost::shared_future<cql::cql_future_result_t> >(
        static_cast<cql_context_t*>(context)->pool->prepare(query_string),
        future_base_t::RESULT_FUTURE);
    return NULL;
}

// void*
// cql_execute_new(
//     void*  context,
//     void*  id,
//     size_t id_size)
// {

// }

// void*
// cql_execute(
//     void* context,
//     void* prepared,
//     int   consistency)
// {

// }
