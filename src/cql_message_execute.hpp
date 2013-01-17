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

#ifndef CQL_MESSAGE_EXECUTE_H_
#define CQL_MESSAGE_EXECUTE_H_

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "../include/cql.h"
#include "cql_message.hpp"

namespace cql {

    class cql_message_execute_t :
        boost::noncopyable,
        public cql_message_t
    {

    public:
        typedef  std::vector<cql_byte_t>            param_t;
        typedef  boost::ptr_list<param_t>           params_container_t;
        typedef  param_t*                           value_type;
        typedef  param_t&                           reference;
        typedef  const param_t&                     const_reference;
        typedef  params_container_t::iterator       iterator;
        typedef  params_container_t::const_iterator const_iterator;
        typedef  params_container_t::size_type      size_type;

        cql_message_execute_t();

        cql_message_execute_t(const std::vector<cql_byte_t>& id,
                              cql_short_t consistency);

        const std::vector<cql_byte_t>&
        query_id() const;

        void
        query_id(const std::vector<cql_byte_t>& id);

        cql_short_t
        consistency() const;

        void
        consistency(const cql_short_t consistency);

        void
        push_back(const param_t& val);

        void
        push_back(const std::string& val);

        void
        push_back(const cql_short_t val);

        void
        push_back(const cql_int_t val);

        void
        push_back(const cql_bigint_t val);

        void
        push_back(const float val);

        void
        push_back(const double val);

        void
        push_back(const bool val);

        void
        pop_back();

        const_iterator
        begin() const;

        const_iterator
        end() const;

        cql_byte_t
        opcode() const;

        cql_int_t
        size() const;

        std::string
        str() const;

        std::istream&
        read(std::istream& input);

        std::ostream&
        write(std::ostream& output) const;

    private:
        std::vector<cql_byte_t>  _query_id;
        cql_short_t              _consistency;
        params_container_t       _params;
    };

} // namespace cql

#endif // CQL_MESSAGE_EXECUTE_H_
