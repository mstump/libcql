#include "gtest/gtest.h"
#include "libcql/cql.hpp"
#include "libcql/cql_error.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_message_event_impl.hpp"


char TEST_MESSAGE_EVENT[] = {
    // 0x81, 0x00, 0xff, 0x0c, 0x00, 0x00, 0x00, 0x25, // header
    0x00, 0x0d, 0x53, 0x43, 0x48, 0x45, 0x4d, 0x41,
    0x5f, 0x43, 0x48, 0x41, 0x4e, 0x47, 0x45, 0x00,
    0x07, 0x44, 0x52, 0x4f, 0x50, 0x50, 0x45, 0x44,
    0x00, 0x02, 0x6b, 0x6d, 0x00, 0x07, 0x74, 0x65,
    0x73, 0x74, 0x5f, 0x63, 0x66 };

TEST(cql_message_event_cpp, opcode)
{
	cql::cql_message_event_impl_t m;
	EXPECT_EQ(cql::CQL_OPCODE_EVENT, m.opcode());
}

TEST(cql_message_event_cpp, serialization_from_byte_size)
{
	cql::cql_message_event_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_EVENT, TEST_MESSAGE_EVENT + sizeof(TEST_MESSAGE_EVENT));
    cql::cql_error_t err;
    m.consume(&err);

    EXPECT_EQ(sizeof(TEST_MESSAGE_EVENT), m.buffer()->size());
}

TEST(cql_message_event_cpp, serialization_from_byte_event_type)
{
	cql::cql_message_event_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_EVENT, TEST_MESSAGE_EVENT + sizeof(TEST_MESSAGE_EVENT));
    cql::cql_error_t err;
    m.consume(&err);

    EXPECT_EQ(cql::CQL_EVENT_TYPE_SCHEMA, m.event_type());
}

TEST(cql_message_event_cpp, serialization_from_byte_keyspace)
{
	cql::cql_message_event_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_EVENT, TEST_MESSAGE_EVENT + sizeof(TEST_MESSAGE_EVENT));
    cql::cql_error_t err;
    m.consume(&err);

    EXPECT_EQ("km", m.keyspace());
}

TEST(cql_message_event_cpp, serialization_from_byte_column_family)
{
	cql::cql_message_event_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_EVENT, TEST_MESSAGE_EVENT + sizeof(TEST_MESSAGE_EVENT));
    cql::cql_error_t err;
    m.consume(&err);

    EXPECT_EQ("test_cf", m.column_family());
}

TEST(cql_message_event_cpp, serialization_from_byte_schema_change)
{
	cql::cql_message_event_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_EVENT, TEST_MESSAGE_EVENT + sizeof(TEST_MESSAGE_EVENT));
    cql::cql_error_t err;
    m.consume(&err);

    EXPECT_EQ(cql::CQL_EVENT_SCHEMA_DROPPED, m.schema_change());
}
