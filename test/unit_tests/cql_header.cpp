#include "gtest/gtest.h"
#include "libcql/cql.hpp"
#include "libcql/cql_error.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_header_impl.hpp"

char TEST_HEADER[] = { 0x01, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x05 };

TEST(cql_header_cpp, getters_and_constructor)
{
    std::stringstream output;
    cql::cql_header_impl_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, 1, cql::CQL_OPCODE_READY, 5);
    EXPECT_EQ(1, header.version());
    EXPECT_EQ(0, header.flags());
    EXPECT_EQ(1, header.stream());
    EXPECT_EQ(2, header.opcode());
    EXPECT_EQ(5, header.length());
}

TEST(cql_header_cpp, setters)
{
    std::stringstream output;
    cql::cql_header_impl_t header;

    header.version(CQL_VERSION_1_REQUEST);
    header.flags(CQL_FLAG_NOFLAG);
    header.stream(1);
    header.opcode(cql::CQL_OPCODE_READY);
    header.length(5);

    EXPECT_EQ(1, header.version());
    EXPECT_EQ(0, header.flags());
    EXPECT_EQ(1, header.stream());
    EXPECT_EQ(2, header.opcode());
    EXPECT_EQ(5, header.length());
}

TEST(cql_header_cpp, serialization_size)
{
    cql::cql_header_impl_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, 0, cql::CQL_OPCODE_READY, 5);
    cql::cql_error_t err;
    header.prepare(&err);
    EXPECT_EQ(8, header.size());
}

TEST(cql_header_cpp, serialization_round_trip)
{
    cql::cql_header_impl_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, 1, cql::CQL_OPCODE_READY, 5);
    cql::cql_error_t err;
    header.prepare(&err);
    header.consume(&err);
    EXPECT_EQ(CQL_VERSION_1_REQUEST, header.version());
    EXPECT_EQ(CQL_FLAG_NOFLAG, header.flags());
    EXPECT_EQ(1, header.stream());
    EXPECT_EQ(cql::CQL_OPCODE_READY, header.opcode());
    EXPECT_EQ(5, header.length());
}

TEST(cql_header_cpp, serialization_to_byte)
{
    cql::cql_header_impl_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, 1, cql::CQL_OPCODE_READY, 5);
    cql::cql_error_t err;
    header.prepare(&err);
    EXPECT_TRUE(memcmp(TEST_HEADER, &(*header.buffer())[0], sizeof(TEST_HEADER)) == 0);
}

TEST(cql_header_cpp, serialization_from_byte)
{
    cql::cql_header_impl_t header;

    header.buffer()->assign(TEST_HEADER, TEST_HEADER + sizeof(TEST_HEADER));
    cql::cql_error_t err;
    header.consume(&err);
    EXPECT_EQ(1, header.version());
    EXPECT_EQ(0, header.flags());
    EXPECT_EQ(1, header.stream());
    EXPECT_EQ(2, header.opcode());
    EXPECT_EQ(5, header.length());
}
