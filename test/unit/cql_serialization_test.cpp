#include <boost/test/unit_test.hpp>
#include "libcql/cql.h"
#include "cql_error.hpp"
#include "cql_util.hpp"
#include "cql_defines.hpp"
#include "../src/cql_serialization.cpp"

BOOST_AUTO_TEST_SUITE(serialization)

BOOST_AUTO_TEST_CASE(decode_double_ptr)
{
    double d = 0;
    cql_byte_t d_buffer[] = {0x40, 0x09, 0x25, 0xca, 0xcb, 0xeb, 0xa6, 0x57};
    cql::decode_double(d_buffer, d);
    BOOST_CHECK_CLOSE(3.1434532100000001, d, 0.0000000000000001); // not what you expect due to double limits
}

BOOST_AUTO_TEST_CASE(decode_option)
{
    cql_byte_t  buffer[] = {0x00, 0x01};
    cql_short_t id       = 0;
    std::string custom;

    cql_byte_t* pos = cql::decode_option(buffer, id, custom);
    BOOST_CHECK_EQUAL(1, id);
    BOOST_CHECK_EQUAL("", custom);
    BOOST_CHECK(buffer + sizeof(buffer) == pos);
}

BOOST_AUTO_TEST_SUITE_END()
