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

#ifndef CQL_CALLBACK_STORAGE_H_
#define CQL_CALLBACK_STORAGE_H_

#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <new>

namespace cql {
    // this template class stores up to 128 values no more
    template<typename value_t>
    class small_indexed_storage {
    private:
        template<typename t>
        struct bucket {
            t value;
            int is_allocated;
        };

        std::vector< bucket<value_t>* > array;
        std::vector<int32_t> frees;
    public:
        small_indexed_storage(int32_t size)
        {
            array.reserve(size);
        }
        ~small_indexed_storage()
        {
            for(size_t i=0; i<array.size(); i++)
            {
                release(i);
                free(array[i]);
            }
        }

        int8_t
        allocate()
        {
            if (frees.size()) {
                int32_t ret = frees.back();
                frees.pop_back();
                return ret;
            } else {
                bucket<value_t>* ptr = (bucket<value_t>*)calloc(1, sizeof(bucket<value_t>));
                array.push_back(ptr);
                return array.size() - 1;
            }
        }

        void
        release(int32_t index)
        {
            if (array[index]->is_allocated) {
                array[index]->value.release();
                array[index]->value.~value_t();
                array[index]->is_allocated = 0;
            }
            frees.push_back(index);
        }

        bool
        has(int32_t index) const
        {
            return (int32_t)array.size() > index && array[index]->is_allocated;
        }

        value_t&
        get(int32_t index)
        {
            return array[index]->value;
        }

        const value_t&
        get(int32_t index) const
        {
            return array[index]->value;
        }

        void
        put(int32_t index,
            const value_t& val)
        {
            if (array[index]->is_allocated) {
                array[index]->value.~value_t();
            }
            array[index]->is_allocated = 1;
            new (&array[index]->value) value_t(val);
        }

    };
}

#endif // CQL_CALLBACK_STORAGE_H_
