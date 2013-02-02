#include "gtest/gtest.h"
#include "cql.hpp"
#include "internal/cql_defines.hpp"
#include "internal/cql_message_error.hpp"


TEST(cql_message_error_cpp, setters_and_getters)
{
	cql::cql_message_error_t m;
	m.code(1);
	m.message("foo");
	EXPECT_EQ(1, m.code());
	EXPECT_EQ("foo", m.message());
}

TEST(cql_message_error_cpp, constructor)
{
	cql::cql_message_error_t m(1, "foo");
	EXPECT_EQ(1, m.code());
	EXPECT_EQ("foo", m.message());
}

TEST(cql_message_error_cpp, opcode)
{
	cql::cql_message_error_t m;
	EXPECT_EQ(CQL_OPCODE_ERROR, m.opcode());
}

TEST(cql_message_error_cpp, serialization_round_trip)
{
	std::stringstream output;
	cql::cql_message_error_t m1(1, "foo");
	cql::cql_message_error_t m2;
	m1.write(output);
	m2.read(output);
	EXPECT_EQ(m1.message(), m2.message());
	EXPECT_EQ(m1.code(), m2.code());
	EXPECT_EQ(m1.opcode(), m2.opcode());
	EXPECT_EQ(m1.size(), m2.size());
}
