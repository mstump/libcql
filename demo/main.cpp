#include <boost/foreach.hpp>

#include "cql.h"
#include "../src/cql_message_result.hpp"
#include "../src/cql_client.hpp"
#include "../src/cql_row.hpp"

void
errback(cql::cql_client_t& client,
        int8_t stream,
        const cql_error_t& err)
{
    std::cerr << "ERROR " << err.message << std::endl;
}

void
select_callback(cql::cql_client_t& client,
                int8_t stream,
                const cql::cql_message_result_t& result)
{
    std::cout << "select callback" << std::endl;
    BOOST_FOREACH(const cql::cql_row_t& row, result)
    {
        std::cout << row.str() << std::endl;
    }
}

void
prepare_callback(cql::cql_client_t& client,
                 int8_t stream,
                 const cql::cql_message_result_t& result)
{
    client.query("SELECT * from schema_keyspaces;",
                 CQL_CONSISTENCY_ALL,
                 &select_callback,
                 &errback);
}

void
use_callback(cql::cql_client_t& client,
             int8_t stream,
             const cql::cql_message_result_t& result)
{
    client.prepare("SELECT * from schema_keyspaces;",
                   &prepare_callback,
                   &errback);
}

void
connect_callback(cql::cql_client_t& client)
{
    client.query("USE system;",
                 CQL_CONSISTENCY_ALL,
                 &use_callback,
                 &errback);
}

void
log_callback(cql_short_t level,
             const std::string& message)
{
    std::cout << "LOG: " << message << std::endl;
}

int
main(int argc,
     char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        cql::cql_client_t c(io_service, &log_callback);
        c.connect("localhost", 9042, &connect_callback);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
