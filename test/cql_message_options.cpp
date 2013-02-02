#include "gtest/gtest.h"
#include "cql.hpp"
#include "internal/cql_defines.hpp"
#include "internal/cql_message_options.hpp"


TEST(cql_message_options_cpp, zero_size)
{
	cql::cql_message_options_t m;
	EXPECT_EQ(0, m.size());
}

TEST(cql_message_options_cpp, opcode)
{
	cql::cql_message_options_t m;
	EXPECT_EQ(CQL_OPCODE_OPTIONS, m.opcode());
}

TEST(cql_message_options_cpp, zero_length_serialization)
{
	std::stringstream output;
	cql::cql_message_options_t m;
	m.write(output);
    EXPECT_EQ(0, output.str().length());
}
