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

#ifndef CQL_ERROR_H_
#define CQL_ERROR_H_

#include <string>
#include "cql.hpp"

namespace cql {

    class cql_error_t
    {

    public:
        cql_error_t(
            bool application,
            cql_int_t application_error,
            cql_int_t transport_error,
            const std::string& message);

        bool
        application() const;

        cql_int_t
        application_error() const;

        cql_int_t
        transport_error() const;

        const std::string&
        message() const;

        void
        application(bool v);

        void
        application_error(cql_int_t err);

        void
        transport_error(cql_int_t err);

        void
        message(const std::string& msg);

    private:
        bool         _application;
        cql_int_t    _application_error;
        cql_int_t    _transport_error;
        std::string  _message;
    };


} // namespace cql

#endif // CQL_ERROR_H_
