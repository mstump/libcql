#include "gtest/gtest.h"
#include "libcql/cql.hpp"
#include "libcql/cql_error.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_message_options_impl.hpp"

TEST(cql_message_options_cpp, opcode)
{
	cql::cql_message_options_impl_t m;
	EXPECT_EQ(cql::CQL_OPCODE_OPTIONS, m.opcode());
}

TEST(cql_message_options_cpp, zero_length_serialization)
{
	cql::cql_message_options_impl_t m;
    cql::cql_error_t err;
    m.prepare(&err);
    EXPECT_EQ(0, m.size());
}
