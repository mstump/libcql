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

#include <libcql/cql.hpp>
#include "c.h"

struct cql_context_t
{
    cql_client_pool_t* pool;
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
    return false;
}

bool
cql_init_ssl(
    void*  context,
    void** status)
{
    return false;
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
    return static_cast<cql_context_t*>(context)->pool->add_client(std::string(host, host_size), port);
}

void*
cql_prepared_new(
    void*  context,
    void*  query,
    size_t query_size)
{

}

void
cql_prepared_push_data(
    void*  context,
    void*  prepared,
    void*  data,
    size_t data_size)
{

}

void
cql_prepared_push_bool(
    void* context,
    void* prepared,
    bool  param)
{

}

void
cql_prepared_push_short(
    void* context,
    void* prepared,
    short param)
{

}

void
cql_prepared_push_int(
    void* context,
    void* prepared,
    int   param)
{

}

void
cql_prepared_push_float(
    void* context,
    void* prepared,
    float param)
{

}

void
cql_prepared_push_double(
    void* context,
    void* prepared,
    float param)
{

}

void
cql_prepared_push_bigint(
    void*   context,
    void*   prepared,
    int64_t param)
{

}

void*
cql_execute_prepared(
    void*  context,
    void*  prepared,
    int    consistency)
{

}

bool
cql_future_ready(
    void* context,
    void* future)
{

}

void
cql_future_wait(
    void* context,
    void* future)
{

}

bool
cql_future_success(
    void* context,
    void* future)
{

}

int
cql_future_get_error_code(
    void* context,
    void* future)
{

}

void
cql_future_get_error_message(
    void*  context,
    void*  future,
    char** message)
{

}
