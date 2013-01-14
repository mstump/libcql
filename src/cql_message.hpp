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

#ifndef CQL_MESSAGE_H_
#define CQL_MESSAGE_H_

#include <istream>
#include <ostream>
#include <string>

namespace cql {

class cql_message_t
{

public:

    virtual cql_byte_t
    opcode() const = 0;

    virtual cql_int_t
    size() const = 0;

    virtual std::istream&
    read(std::istream&) = 0;

    virtual std::ostream&
    write(std::ostream&) const = 0;

    virtual std::string
    str() const = 0;
};

} // namespace cql

#endif // CQL_MESSAGE_H_
