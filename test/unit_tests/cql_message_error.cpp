#include "gtest/gtest.h"
#include "libcql/cql.hpp"
#include "libcql/cql_error.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_message_error_impl.hpp"


TEST(cql_message_error_cpp, setters_and_getters)
{
	cql::cql_message_error_impl_t m;
	m.code(1);
	m.message("foo");
	EXPECT_EQ(1, m.code());
	EXPECT_EQ("foo", m.message());
}

TEST(cql_message_error_cpp, constructor)
{
	cql::cql_message_error_impl_t m(1, "foo");
	EXPECT_EQ(1, m.code());
	EXPECT_EQ("foo", m.message());
}

TEST(cql_message_error_cpp, opcode)
{
	cql::cql_message_error_impl_t m;
	EXPECT_EQ(cql::CQL_OPCODE_ERROR, m.opcode());
}

TEST(cql_message_error_cpp, serialization_round_trip)
{
	std::stringstream output;
	cql::cql_message_error_impl_t m(1, "foo");
    cql::cql_error_t err;
    m.prepare(&err);
    m.consume(&err);
	EXPECT_EQ("foo", m.message());
	EXPECT_EQ(1, m.code());
}
