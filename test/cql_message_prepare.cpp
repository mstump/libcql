#include "gtest/gtest.h"
#include "cql.h"
#include "cql_message_prepare.hpp"

char TEST_MESSAGE_PREPARE[] = {
0x00, 0x00, 0x00, 0x1f, 0x53, 0x45, 0x4c, 0x45,
0x43, 0x54, 0x20, 0x2a, 0x20, 0x66, 0x72, 0x6f,
0x6d, 0x20, 0x73, 0x63, 0x68, 0x65, 0x6d, 0x61,
0x5f, 0x6b, 0x65, 0x79, 0x73, 0x70, 0x61, 0x63,
0x65, 0x73, 0x3b };

TEST(cql_message_prepare_cpp, opcode)
{
	cql::cql_message_prepare_t m;
	EXPECT_EQ(CQL_OPCODE_PREPARE, m.opcode());
}

TEST(cql_message_prepare_cpp, serialization_size)
{
	cql::cql_message_prepare_t m("SELECT * from schema_keyspaces;");
    EXPECT_EQ(sizeof(TEST_MESSAGE_PREPARE), m.size());
}

TEST(cql_message_prepare_cpp, serialization_to_byte)
{
	std::stringstream output;
	cql::cql_message_prepare_t m("SELECT * from schema_keyspaces;");
	m.write(output);
    EXPECT_EQ(sizeof(TEST_MESSAGE_PREPARE), m.size());
    EXPECT_EQ(sizeof(TEST_MESSAGE_PREPARE), output.str().size());
    EXPECT_TRUE(memcmp(TEST_MESSAGE_PREPARE, output.str().data(), sizeof(TEST_MESSAGE_PREPARE)) == 0);
}

TEST(cql_message_prepare_cpp, serialization_from_byte)
{
	std::stringstream input;
	cql::cql_message_prepare_t m;

    input.write(TEST_MESSAGE_PREPARE, sizeof(TEST_MESSAGE_PREPARE));
	m.read(input);

	EXPECT_EQ("SELECT * from schema_keyspaces;", m.query());
}
