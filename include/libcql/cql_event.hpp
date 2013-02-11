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

#ifndef CQL_EVENT_H_
#define CQL_EVENT_H_

#include "libcql/cql.hpp"

namespace cql {

    class cql_event_t {
    public:
        virtual cql_event_enum
        event_type() const = 0;

        virtual cql_event_topology_enum
        topology_change() const = 0;

        virtual cql_event_status_enum
        status_change() const = 0;

        virtual cql_event_schema_enum
        schema_change() const = 0;

        virtual const std::string&
        keyspace() const = 0;

        virtual const std::string&
        column_family() const = 0;

        virtual const std::string&
        ip() const = 0;

        virtual cql_int_t
        port() const = 0;

        virtual
        ~cql_event_t(){};
    };

} // namespace cql

#endif // CQL_EVENT_H_
