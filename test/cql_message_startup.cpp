#include "gtest/gtest.h"
#include "cql.h"
#include "internal/cql_message_startup.hpp"

char TEST_MESSAGE_STARTUP[] = {
    0x00, 0x01, 0x00, 0x0b, 0x43, 0x51, 0x4c, 0x5f,
    0x56, 0x45, 0x52, 0x53, 0x49, 0x4f, 0x4e, 0x00,
    0x05, 0x33, 0x2e, 0x30, 0x2e, 0x30 };


TEST(cql_message_startup_cpp, opcode)
{
	cql::cql_message_startup_t m;
	EXPECT_EQ(CQL_OPCODE_STARTUP, m.opcode());
}

TEST(cql_message_startup_cpp, serialization_size)
{
	cql::cql_message_startup_t m;
    m.version(CQL_VERSION_IMPL);
    EXPECT_EQ(sizeof(TEST_MESSAGE_STARTUP), m.size());
}

TEST(cql_message_startup_cpp, serialization_to_byte)
{
	std::stringstream output;
	cql::cql_message_startup_t m;
    m.version(CQL_VERSION_IMPL);
	m.write(output);
    EXPECT_TRUE(memcmp(TEST_MESSAGE_STARTUP, output.str().data(), sizeof(TEST_MESSAGE_STARTUP)) == 0);
}

TEST(cql_message_startup_cpp, serialization_from_byte)
{
	std::stringstream input;
	cql::cql_message_startup_t m;

    input.write(TEST_MESSAGE_STARTUP, sizeof(TEST_MESSAGE_STARTUP));
	m.read(input);

	EXPECT_EQ(CQL_VERSION_IMPL, m.version());
	EXPECT_EQ("", m.compression());
}
