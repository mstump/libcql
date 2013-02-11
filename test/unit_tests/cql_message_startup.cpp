#include "gtest/gtest.h"
#include "libcql/cql.hpp"
#include "libcql/cql_error.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_message_startup_impl.hpp"

char TEST_MESSAGE_STARTUP[] = {
    0x00, 0x01, 0x00, 0x0b, 0x43, 0x51, 0x4c, 0x5f,
    0x56, 0x45, 0x52, 0x53, 0x49, 0x4f, 0x4e, 0x00,
    0x05, 0x33, 0x2e, 0x30, 0x2e, 0x30 };


TEST(cql_message_startup_cpp, opcode)
{
	cql::cql_message_startup_impl_t m;
	EXPECT_EQ(cql::CQL_OPCODE_STARTUP, m.opcode());
}

TEST(cql_message_startup_cpp, serialization_to_byte)
{
	cql::cql_message_startup_impl_t m;
    m.version(CQL_VERSION_IMPL);
    cql::cql_error_t err;
    m.prepare(&err);
    EXPECT_EQ(sizeof(TEST_MESSAGE_STARTUP), m.size());
    EXPECT_TRUE(memcmp(TEST_MESSAGE_STARTUP, &(*m.buffer())[0], sizeof(TEST_MESSAGE_STARTUP)) == 0);
}

TEST(cql_message_startup_cpp, serialization_from_byte)
{
	cql::cql_message_startup_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_STARTUP, TEST_MESSAGE_STARTUP + sizeof(TEST_MESSAGE_STARTUP));
    cql::cql_error_t err;
    m.consume(&err);

	EXPECT_EQ(CQL_VERSION_IMPL, m.version());
	EXPECT_EQ("", m.compression());
}
