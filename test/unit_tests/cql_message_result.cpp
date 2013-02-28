#include "gtest/gtest.h"
#include "libcql/cql.hpp"
#include "libcql/cql_error.hpp"
#include "libcql/internal/cql_defines.hpp"
#include "libcql/internal/cql_message_result_impl.hpp"

// CREATE TABLE test_cf (
//       ascii ascii,
//       bigint bigint,
//       blob blob,
//       boolean boolean,
//       decimal decimal,
//       double double,
//       float float,
//       int int,
//       text text,
//       timestamp timestamp,
//       uuid uuid,
//       timeuuid timeuuid,
//       varchar varchar,
//       varint varint,
//       a_set set<int>,
//       a_list list<boolean>,
//       a_map map<text, int>,
//       PRIMARY KEY (ascii));

// INSERT INTO test_cf (ascii, bigint, blob, boolean, decimal, double, float, int, text, timestamp, uuid, timeuuid, varchar, varint, a_set, a_list, a_map) VALUES ('ascii', 9223372036854775807, 'DEADBEEF', true, 3.14345321, 3.14345321, 3.14, 314, 'text', 1240003134, '21c9b031-a3dc-4556-b42f-12c2867c7d4a', 'afbfe1e0-80ff-11e2-9e96-0800200c9a66', 'varchar', 170141183460469231731687303715884105727, {1, 2, 3}, [true, true, false], {'a' : 1, 'b' : 2});


char TEST_MESSAGE_RESULT[] = { 0x00, 0x00, 0x00, 0x02, // result_type(int=2)
                               0x00, 0x00, 0x00, 0x01, // metadata flags(int=1)
                               0x00, 0x00, 0x00, 0x11, // metadata column_count(int=17)
                               0x00, 0x02, 0x6b, 0x6d, // metadata global_table_spec(string=km)
                               0x00, 0x07, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x63, 0x66, // metadata global_table_spec(string=test_cf)
                               0x00, 0x05, 0x61, 0x73, 0x63, 0x69, 0x69, 0x00, 0x01, // 1 'ascii' type=ascii
                               0x00, 0x06, 0x61, 0x5f, 0x6c, 0x69, 0x73, 0x74, 0x00, 0x20, 0x00, 0x04, // 2 'a_list' type=list<boolean>
                               0x00, 0x05, 0x61, 0x5f, 0x6d, 0x61, 0x70, 0x00, 0x21, 0x00, 0x0d, 0x00, 0x09, // 3 'a_map' type=map<varchar, int>
                               0x00, 0x05, 0x61, 0x5f, 0x73, 0x65, 0x74, 0x00, 0x22, 0x00, 0x09, // 4 'a_set' type=set<boolean>
                               0x00, 0x06, 0x62, 0x69, 0x67, 0x69, 0x6e, 0x74, 0x00, 0x02, // 5 'bigint' type=bigint
                               0x00, 0x04, 0x62, 0x6c, 0x6f, 0x62, 0x00, 0x03, // 6 'blob' type=blob
                               0x00, 0x07, 0x62, 0x6f, 0x6f, 0x6c, 0x65, 0x61, 0x6e, 0x00, 0x04, // 7 'boolean' type=boolean
                               0x00, 0x07, 0x64, 0x65, 0x63, 0x69, 0x6d, 0x61, 0x6c, 0x00, 0x06, // 8 'decimal' type=decimal
                               0x00, 0x06, 0x64, 0x6f, 0x75, 0x62, 0x6c, 0x65, 0x00, 0x07, // 9 'double' type=double
                               0x00, 0x05, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x00, 0x08, // 10 'float' type=float
                               0x00, 0x03, 0x69, 0x6e, 0x74, 0x00, 0x09, // 11 'int' type=int
                               0x00, 0x04, 0x74, 0x65, 0x78, 0x74, 0x00, 0x0d, // 12 'text' type=varchar
                               0x00, 0x09, 0x74, 0x69, 0x6d, 0x65, 0x73, 0x74, 0x61, 0x6d, 0x70, 0x00, 0x0b, // 13 'timestamp' type=timestamp
                               0x00, 0x08, 0x74, 0x69, 0x6d, 0x65, 0x75, 0x75, 0x69, 0x64, 0x00, 0x0f, // 14 'timeuuid' type=timeuuid
                               0x00, 0x04, 0x75, 0x75, 0x69, 0x64, 0x00, 0x0c, // 15 'uuid' type=uuid
                               0x00, 0x07, 0x76, 0x61, 0x72, 0x63, 0x68, 0x61, 0x72, 0x00, 0x0d, // 16 'varchar' type=varchar
                               0x00, 0x06, 0x76, 0x61, 0x72, 0x69, 0x6e, 0x74, 0x00, 0x0e, // 17 'varint' type=varint
                               0x00, 0x00, 0x00, 0x01, // row_count(int=1)
                               0x00, 0x00, 0x00, 0x05, 0x61, 0x73, 0x63, 0x69, 0x69, // 1 ascii(ascii)
                               0x00, 0x00, 0x00, 0x0b, // 2 list(true, true, false)
                               0x00, 0x03, // list size of 3
                               0x00, 0x01, 0x01, // list item true
                               0x00, 0x01, 0x01, // list item true
                               0x00, 0x01, 0x00, // list item false
                               0x00, 0x00, 0x00, 0x14, // 3 map{'a' : 1, 'b' : 2}
                               0x00, 0x02, // map size of 2
                               0x00, 0x01, 0x61, // map key a
                               0x00, 0x04, 0x00, 0x00, 0x00, 0x01, // map val 1
                               0x00, 0x01, 0x62, // map key b
                               0x00, 0x04, 0x00, 0x00, 0x00, 0x02, // map val 2
                               0x00, 0x00, 0x00, 0x14, // 4 set{1, 2, 3}
                               0x00, 0x03, // set size of 3
                               0x00, 0x04, 0x00, 0x00, 0x00, 0x01, // set item 1
                               0x00, 0x04, 0x00, 0x00, 0x00, 0x02, // set item 2
                               0x00, 0x04, 0x00, 0x00, 0x00, 0x03, // set item 3
                               0x00, 0x00, 0x00, 0x08, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 5 bigint(9223372036854775807)
                               0x00, 0x00, 0x00, 0x04, 0xde, 0xad, 0xbe, 0xef,  // 6 blob(0xDEADBEEF)
                               0x00, 0x00, 0x00, 0x01, 0x01, // 7 bool(true)
                               0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x12, 0xbc, 0x87, 0x69, // 8 decimal(3.14345321)
                               0x00, 0x00, 0x00, 0x08, 0x40, 0x09, 0x25, 0xca, 0xcb, 0xeb, 0xa6, 0x57, // 9 double(3.14345321)
                               0x00, 0x00, 0x00, 0x04, 0x40, 0x48, 0xf5, 0xc3, // 10 float(3.14)
                               0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x3a, // 11 int(314)
                               0x00, 0x00, 0x00, 0x04, 0x74, 0x65, 0x78, 0x74, // 12 varchar(text)
                               0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x49, 0xe8, 0xf2, 0x3e, // 13 timestamp(1240003134)
                               0x00, 0x00, 0x00, 0x10, 0xaf, 0xbf, 0xe1, 0xe0, 0x80, 0xff, 0x11, 0xe2, 0x9e, 0x96, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66, // 14 timeuuid(afbfe1e0-80ff-11e2-9e96-0800200c9a66)
                               0x00, 0x00, 0x00, 0x10, 0x21, 0xc9, 0xb0, 0x31, 0xa3, 0xdc, 0x45, 0x56, 0xb4, 0x2f, 0x12, 0xc2, 0x86, 0x7c, 0x7d, 0x4a, // 15 uuid(21c9b031-a3dc-4556-b42f-12c2867c7d4a)
                               0x00, 0x00, 0x00, 0x07, 0x76, 0x61, 0x72, 0x63, 0x68, 0x61, 0x72, // 16 varchar('varchar')
                               0x00, 0x00, 0x00, 0x10, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // varint(170141183460469231731687303715884105727)

TEST(cql_message_result_cpp, opcode)
{
	cql::cql_message_result_impl_t m;
	EXPECT_EQ(cql::CQL_OPCODE_RESULT, m.opcode());
}

TEST(cql_message_result_cpp, serialization_from_byte_size)
{
	cql::cql_message_result_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_RESULT, TEST_MESSAGE_RESULT + sizeof(TEST_MESSAGE_RESULT));
    cql::cql_error_t err;
    m.consume(&err);

    EXPECT_EQ(sizeof(TEST_MESSAGE_RESULT), m.buffer()->size());
}

TEST(cql_message_result_cpp, serialization_from_byte_result_type)
{
	cql::cql_message_result_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_RESULT, TEST_MESSAGE_RESULT + sizeof(TEST_MESSAGE_RESULT));
    cql::cql_error_t err;
    m.consume(&err);

    EXPECT_EQ(cql::CQL_RESULT_ROWS, m.result_type());
}

TEST(cql_message_result_cpp, serialization_from_byte_flags)
{
	cql::cql_message_result_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_RESULT, TEST_MESSAGE_RESULT + sizeof(TEST_MESSAGE_RESULT));
    cql::cql_error_t err;
    m.consume(&err);

    EXPECT_EQ(1, m.get_metadata().flags());
}

TEST(cql_message_result_cpp, serialization_from_byte_global_keyspace_and_table)
{
	cql::cql_message_result_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_RESULT, TEST_MESSAGE_RESULT + sizeof(TEST_MESSAGE_RESULT));
    cql::cql_error_t err;
    m.consume(&err);

    EXPECT_EQ(m.get_metadata().has_global_keyspace(), true);
    EXPECT_EQ(m.get_metadata().has_global_table(), true);
    EXPECT_EQ(m.get_metadata().global_keyspace(), "km");
    EXPECT_EQ(m.get_metadata().global_table(), "test_cf");
}

TEST(cql_message_result_cpp, serialization_from_byte_row_count)
{
	cql::cql_message_result_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_RESULT, TEST_MESSAGE_RESULT + sizeof(TEST_MESSAGE_RESULT));
    cql::cql_error_t err;
    m.consume(&err);
    EXPECT_EQ(1, m.row_count());
}

TEST(cql_message_result_cpp, serialization_from_byte_column_count)
{
	cql::cql_message_result_impl_t m;
    m.buffer()->assign(TEST_MESSAGE_RESULT, TEST_MESSAGE_RESULT + sizeof(TEST_MESSAGE_RESULT));
    cql::cql_error_t err;
    m.consume(&err);
    EXPECT_EQ(17, m.column_count());
}
