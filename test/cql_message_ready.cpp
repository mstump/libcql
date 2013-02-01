#include "gtest/gtest.h"
#include "cql.h"
#include "internal/cql_message_ready.hpp"


TEST(cql_message_ready_cpp, zero_size)
{
	cql::cql_message_ready_t m;
	EXPECT_EQ(0, m.size());
}

TEST(cql_message_ready_cpp, opcode)
{
	cql::cql_message_ready_t m;
	EXPECT_EQ(CQL_OPCODE_READY, m.opcode());
}

TEST(cql_message_ready_cpp, zero_length_serialization)
{
	std::stringstream output;
	cql::cql_message_ready_t m;
	m.write(output);
    EXPECT_EQ(0, output.str().length());
}
