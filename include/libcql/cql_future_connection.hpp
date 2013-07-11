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

#ifndef CQL_FUTURE_CONNECTION_H_
#define CQL_FUTURE_CONNECTION_H_

#include "libcql/cql.hpp"
#include "libcql/cql_error.hpp"

namespace cql {

    // Forward declarations
    class cql_client_t;

    struct cql_future_connection_t
    {
        cql_future_connection_t() :
            client(NULL)
        {}

        cql_future_connection_t(
            cql::cql_client_t* client) :
            client(client)
        {}

        cql_future_connection_t(
            cql::cql_client_t* client,
            cql::cql_error_t   error) :
            client(client),
            error(error)
        {}

        cql::cql_client_t*                   client;
        cql::cql_error_t                     error;
    };


} // namespace cql

#endif // CQL_FUTURE_CONNECTION_H_
