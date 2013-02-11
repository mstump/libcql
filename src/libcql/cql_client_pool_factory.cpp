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
