#include "gtest/gtest.h"
#include "libcql/cql.hpp"
#include "libcql/cql_error.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_message_prepare_impl.hpp"

char TEST_MESSAGE_PREPARE[] = {
0x00, 0x00, 0x00, 0x1f, 0x53, 0x45, 0x4c, 0x45,
0x43, 0x54, 0x20, 0x2a, 0x20, 0x66, 0x72, 0x6f,
0x6d, 0x20, 0x73, 0x63, 0x68, 0x65, 0x6d, 0x61,
0x5f, 0x6b, 0x65, 0x79, 0x73, 0x70, 0x61, 0x63,
0x65, 0x73, 0x3b };

TEST(cql_message_prepare_cpp, opcode)
{
	cql::cql_message_prepare_impl_t m;
	EXPECT_EQ(cql::CQL_OPCODE_PREPARE, m.opcode());
}

TEST(cql_message_prepare_cpp, serialization_to_byte)
{
	cql::cql_message_prepare_impl_t m("SELECT * from schema_keyspaces;");
    cql::cql_error_t err;
    m.prepare(&err);
    EXPECT_EQ(sizeof(TEST_MESSAGE_PREPARE), m.size());
    EXPECT_TRUE(memcmp(TEST_MESSAGE_PREPARE, &(*m.buffer())[0], sizeof(TEST_MESSAGE_PREPARE)) == 0);
}

TEST(cql_message_prepare_cpp, serialization_from_byte)
{
	cql::cql_message_prepare_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_PREPARE, TEST_MESSAGE_PREPARE + sizeof(TEST_MESSAGE_PREPARE));
    cql::cql_error_t err;
    m.consume(&err);
	EXPECT_EQ("SELECT * from schema_keyspaces;", m.query());
}
