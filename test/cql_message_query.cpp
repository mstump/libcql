#include "gtest/gtest.h"
#include "cql.hpp"
#include "internal/cql_defines.hpp"
#include "cql_message_query.hpp"

char TEST_MESSAGE_QUERY[] = {
    0x00, 0x00, 0x00, 0x0b, 0x75, 0x73, 0x65, 0x20,
    0x73, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x3b, 0x00,
    0x05 };

TEST(cql_message_query_cpp, opcode)
{
	cql::cql_message_query_t m;
	EXPECT_EQ(CQL_OPCODE_QUERY, m.opcode());
}

TEST(cql_message_query_cpp, serialization_size)
{
	cql::cql_message_query_t m("use system;", CQL_CONSISTENCY_ALL);
    EXPECT_EQ(sizeof(TEST_MESSAGE_QUERY), m.size());
}

TEST(cql_message_query_cpp, serialization_to_byte)
{
	std::stringstream output;
	cql::cql_message_query_t m("use system;", CQL_CONSISTENCY_ALL);
	m.write(output);
    EXPECT_EQ(sizeof(TEST_MESSAGE_QUERY), m.size());
    EXPECT_EQ(sizeof(TEST_MESSAGE_QUERY), output.str().size());
    EXPECT_TRUE(memcmp(TEST_MESSAGE_QUERY, output.str().data(), sizeof(TEST_MESSAGE_QUERY)) == 0);
}

TEST(cql_message_query_cpp, serialization_from_byte)
{
	std::stringstream input;
	cql::cql_message_query_t m;

    input.write(TEST_MESSAGE_QUERY, sizeof(TEST_MESSAGE_QUERY));
	m.read(input);

	EXPECT_EQ("use system;", m.query());
	EXPECT_EQ(CQL_CONSISTENCY_ALL, m.consistency());
}
