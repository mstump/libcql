#include "gtest/gtest.h"
#include "libcql/cql.hpp"
#include "libcql/cql_error.hpp"
#include "libcql/internal/cql_util.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "../src/libcql/internal/cql_serialization.cpp"


TEST(cql_serialization_cpp, decode_double_ptr)
{
    double d = 0;
    cql::cql_byte_t d_buffer[] = {0x40, 0x09, 0x25, 0xca, 0xcb, 0xeb, 0xa6, 0x57};
    cql::decode_double(d_buffer, d);
    EXPECT_DOUBLE_EQ(3.1434532100000001, d);
}
