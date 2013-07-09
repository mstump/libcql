/*
  Copyright (c) 2013 Matthew Stump

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

#ifndef CQL_CALLBACK_STORAGE_H_
#define CQL_CALLBACK_STORAGE_H_

#include <stdlib.h>
#include <stdint.h>
#include <new>

namespace cql {
    // this template class stores up to 128 values no more
    template<typename value_t>
    class small_indexed_storage {
    private:
        template <typename _value_t>
        class entry_t {
            struct {
                struct {
                    // -1 - there is no next free index
                    // -2 - flags that his one has been allocated (for checks and stuff)
                    int32_t index; // signed because might be -1
                    int32_t count; // doesn't matter never should be negative (see index restrictions)
                } next_free;
                _value_t value;
            } e;

        public:
            _value_t&
            value()
            {
                return e.value;
            }

            const _value_t&
            value() const
            {
                return e.value;
            }

            int8_t
            next_free_index() const
            {
                return e.next_free.index;
            }

            // not counting this (so 0 means there are no free blocks behind this one)
            int8_t
            next_free_cnt() const
            {
                return e.next_free.count;
            }

            // count does not include this (so 0 means there are no free blocks behind this one)
            // you won't need to set cnt in any case except initial allocation
            void
            set_next_free(int32_t index,
                          int32_t cnt = 0)
            {
                e.next_free.index = index;
                e.next_free.count = cnt;
            }

            void
            set_value(const _value_t& val)
            {
                e.value = val;
            }

            bool
            is_allocated()
            {
                return e.next_free.index == -2;
            }

            void
            set_allocated()
            {
                e.next_free.index = -2;
            }
        };

        typedef entry_t<value_t> array_entry_t;
        array_entry_t* array;
        int32_t next_free_index;
    public:
        explicit
        small_indexed_storage(uint16_t size) :
                next_free_index(0)
        {
            array = new array_entry_t[size];
            array[0].set_next_free(-1, size-1);
        }

        ~small_indexed_storage()
        {
            delete [] array;
        }

        int32_t
        allocate()
        {
            int32_t result;
            if ( (result = next_free_index) >= 0) {
                if (array[next_free_index].next_free_cnt() > 0) {
                    array[++next_free_index].set_next_free(array[result].next_free_index(), array[result].next_free_cnt()-1);
                }
                else {
                    next_free_index = array[next_free_index].next_free_index();
                }
                // mark it allocated
                array[result].set_allocated();
            }

            return result;
        }

        void
        release(int32_t index)
        {
            array[index].set_next_free(next_free_index);
            next_free_index = index;
        }

        bool
        has(int32_t index) const
        {
            return array[index].is_allocated();
        }

        value_t&
        get(int32_t index)
        {
            return array[index].value();
        }

        const value_t&
        get(int32_t index) const
        {
            return array[index].value();
        }

        void
        put(int32_t index,
            const value_t& val)
        {
            array[index].set_value(val);
        }

    };
}

#endif // CQL_CALLBACK_STORAGE_H_

