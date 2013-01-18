#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "cql.h"
#include "../src/cql_error.hpp"
#include "../src/cql_client.hpp"
#include "../src/cql_message_execute.hpp"
#include "../src/cql_message_prepare.hpp"
#include "../src/cql_message_result.hpp"
#include "../src/cql_row.hpp"

void
message_errback(cql::cql_client_t& client,
                int8_t stream,
                const cql::cql_error_t& err)
{
    std::cerr << "ERROR " << err.message() << std::endl;
}

void
connection_errback(cql::cql_client_t& client,
                   const cql::cql_error_t& err)
{
    std::cerr << "ERROR " << boost::lexical_cast<std::string>(err.application_error()) << " " << err.message() << std::endl;
}

void
select_callback(cql::cql_client_t& client,
                int8_t stream,
                const cql::cql_message_result_t& result)
{
    BOOST_FOREACH(const cql::cql_row_t& row, result)
    {
        std::cout << row.str() << std::endl;
    }
    client.close();
}

void
execute_callback(cql::cql_client_t& client,
                 int8_t stream,
                 const cql::cql_message_result_t& result)
{
    BOOST_FOREACH(const cql::cql_row_t& row, result)
    {
        std::cout << row.str() << std::endl;
    }

    client.query("SELECT * from schema_keyspaces;",
                 CQL_CONSISTENCY_ALL,
                 &select_callback,
                 &message_errback);
}

void
prepare_callback(cql::cql_client_t& client,
                 int8_t stream,
                 const cql::cql_message_result_t& result)
{
    cql::cql_message_execute_t m(result.query_id(), CQL_CONSISTENCY_ALL);
    client.write(m,
                 &execute_callback,
                 &message_errback);
}

void
use_callback(cql::cql_client_t& client,
             int8_t stream,
             const cql::cql_message_result_t& result)
{
    cql::cql_message_prepare_t m("SELECT * from schema_keyspaces;");
    client.write(m,
                 &prepare_callback,
                 &message_errback);
}

void
connect_callback(cql::cql_client_t& client)
{
    client.query("USE system;",
                 CQL_CONSISTENCY_ALL,
                 &use_callback,
                 &message_errback);
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
        c.connect("localhost", 9042, &connect_callback, &connection_errback);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
