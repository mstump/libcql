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

#ifndef CQL_INET_H_
#define CQL_INET_H_

#include <string>
#include "libcql/cql.hpp"

namespace cql {

    class cql_inet_t
    {

    public:
        cql_inet_t();

        cql_inet_t(const std::string& address,
                   cql::cql_int_t port);

        const std::string&
        address() const;

        cql_int_t
        port() const;

        void
        address(const std::string& address);

        void
        port(cql_int_t port);

        std::istream&
        read(std::istream& input);

        std::ostream&
        write(std::ostream& output) const;

    private:
        std::string    _address;
        cql::cql_int_t _port;
    };

} // namespace cql

#endif // CQL_INET_H_
