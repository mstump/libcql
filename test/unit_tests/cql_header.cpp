#include "gtest/gtest.h"
#include "cql.hpp"
#include "internal/cql_defines.hpp"
#include "internal/cql_header.hpp"

char TEST_HEADER[] = { 0x01, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x05 };

TEST(cql_header_cpp, getters_and_constructor)
{
    std::stringstream output;
    cql::internal::cql_header_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, 1, CQL_OPCODE_READY, 5);
    EXPECT_EQ(1, header.version());
    EXPECT_EQ(0, header.flags());
    EXPECT_EQ(1, header.stream());
    EXPECT_EQ(2, header.opcode());
    EXPECT_EQ(5, header.length());
}

TEST(cql_header_cpp, setters)
{
    std::stringstream output;
    cql::internal::cql_header_t header;

    header.version(CQL_VERSION_1_REQUEST);
    header.flags(CQL_FLAG_NOFLAG);
    header.stream(1);
    header.opcode(CQL_OPCODE_READY);
    header.length(5);

    EXPECT_EQ(1, header.version());
    EXPECT_EQ(0, header.flags());
    EXPECT_EQ(1, header.stream());
    EXPECT_EQ(2, header.opcode());
    EXPECT_EQ(5, header.length());
}

TEST(cql_header_cpp, serialization_size)
{
    std::stringstream output;
    cql::internal::cql_header_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, 0, CQL_OPCODE_READY, 5);
    header.write(output);
    EXPECT_EQ(header.size(), output.str().size());
}

TEST(cql_header_cpp, serialization_round_trip)
{
    std::stringstream output;
    cql::internal::cql_header_t header1(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, 1, CQL_OPCODE_READY, 5);
    cql::internal::cql_header_t header2;
    header1.write(output);
    header2.read(output);
    EXPECT_EQ(header1.version(), header2.version());
    EXPECT_EQ(header1.flags(), header2.flags());
    EXPECT_EQ(header1.stream(), header2.stream());
    EXPECT_EQ(header1.opcode(), header2.opcode());
    EXPECT_EQ(header1.size(), header2.size());
}

TEST(cql_header_cpp, serialization_to_byte)
{
    std::stringstream output;
    cql::internal::cql_header_t header(CQL_VERSION_1_REQUEST, CQL_FLAG_NOFLAG, 1, CQL_OPCODE_READY, 5);
    header.write(output);
    EXPECT_TRUE(memcmp(TEST_HEADER, output.str().data(), sizeof(TEST_HEADER)) == 0);
}

TEST(cql_header_cpp, serialization_from_byte)
{
    std::stringstream input;
    cql::internal::cql_header_t header;

    input.write(TEST_HEADER, sizeof(TEST_HEADER));
    header.read(input);

    EXPECT_EQ(1, header.version());
    EXPECT_EQ(0, header.flags());
    EXPECT_EQ(1, header.stream());
    EXPECT_EQ(2, header.opcode());
    EXPECT_EQ(5, header.length());
}
