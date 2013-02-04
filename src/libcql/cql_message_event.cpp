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

#include "libcql/internal/cql_defines.hpp"
#include "libcql/cql_serialization.hpp"

#include "libcql/cql_message_event.hpp"
#include "libcql/cql_message_event_schema.hpp"
#include "libcql/cql_message_event_status.hpp"
#include "libcql/cql_message_event_topology.hpp"

cql::cql_message_event_t*
cql::read_cql_event(std::istream& input)
{
    std::string name;
    cql::decode_string(input, name);

    if (name == CQL_EVENT_TOPOLOGY_CHANGE) {
        std::auto_ptr<cql::cql_message_event_topology_t> e(new cql::cql_message_event_topology_t());
        e->read(input);
        return e.release();
    }
    else if (name == CQL_EVENT_STATUS_CHANGE) {
        std::auto_ptr<cql::cql_message_event_status_t> e(new cql::cql_message_event_status_t());
        e->read(input);
        return e.release();
    }
    else if (name == CQL_EVENT_SCHEMA_CHANGE) {
        std::auto_ptr<cql::cql_message_event_schema_t> e(new cql::cql_message_event_schema_t());
        e->read(input);
        return e.release();
    }

    return NULL;
}
