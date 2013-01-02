#include "gtest/gtest.h"
#include "cql.h"
#include "../src/cql_message_error.hpp"

TEST(cql_message_error_cpp, serialization_round_trip)
{
	std::stringstream output;
	cql::cql_message_error_t m1;
	cql::cql_message_error_t m2;
	m1.code(1);
	m1.message("foo");
	m1.write(output);
	m2.read(output);
	EXPECT_EQ(m1.message(), m2.message());
	EXPECT_EQ(m1.code(), m2.code());
	EXPECT_EQ(m1.opcode(), m2.opcode());
	EXPECT_EQ(m1.size(), m2.size());
}
