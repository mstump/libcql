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

#include "libcql/internal/cql_client_pool_impl.hpp"

#include "libcql/cql_client_pool_factory.hpp"



cql::cql_client_pool_t*
cql::cql_client_pool_factory_t::create_client_pool_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                                                     cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                                                     cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback)
{
    return new cql::cql_client_pool_impl_t(client_callback, ready_callback, defunct_callback);
}

cql::cql_client_pool_t*
cql::cql_client_pool_factory_t::create_client_pool_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                                                     cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                                                     cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback,
                                                     cql::cql_client_pool_t::cql_log_callback_t     log_callback)
{
    return new cql::cql_client_pool_impl_t(client_callback, ready_callback, defunct_callback, log_callback);
}

cql::cql_client_pool_t*
cql::cql_client_pool_factory_t::create_client_pool_t(cql::cql_client_pool_t::cql_client_callback_t  client_callback,
                                                     cql::cql_client_pool_t::cql_ready_callback_t   ready_callback,
                                                     cql::cql_client_pool_t::cql_defunct_callback_t defunct_callback,
                                                     cql::cql_client_pool_t::cql_log_callback_t     log_callback,
                                                     size_t                                         reconnect_limit)
{
    return new cql::cql_client_pool_impl_t(client_callback, ready_callback, defunct_callback, log_callback, reconnect_limit);
}
