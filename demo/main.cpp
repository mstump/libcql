#include "cql.h"
#include "../src/cql_message_result.hpp"
#include "../src/cql_client.hpp"

void
errback(cql::cql_client_t& client,
		int8_t stream,
		const cql_error_t& err)
{

}

void
select_callback(cql::cql_client_t& client,
				int8_t stream,
				const cql::cql_message_result_t& result)
{
	std::cout << result.str();
}

void
use_callback(cql::cql_client_t& client,
			 int8_t stream,
			 const cql::cql_message_result_t& result)
{
	client.query("SELECT * from schema_keyspaces;",
				 CQL_CONSISTENCY_ALL,
				 &select_callback,
				 &errback);
	std::cout << result.str();
}

void
connect_callback(cql::cql_client_t& client)
{
	client.query("use system;",
				 CQL_CONSISTENCY_ALL,
				 &use_callback,
				 &errback);
}

int
main(int argc,
	 char* argv[])
{
	try
	{
		boost::asio::io_service io_service;
		cql::cql_client_t c(io_service);
		c.connect("localhost", 9042, &connect_callback);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;
}
