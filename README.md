libcql
======

A high performance implementation of the [Cassandra binary protocol](https://github.com/apache/cassandra/blob/trunk/doc/native_protocol.spec) in C++

### Current Functionality and Design
- Completely asynchronous
- An emphasis on avoiding copying of memory when possible
- Designed to be easily wrapped from C or other languages
- Exception safe
- SSL support
- Ad-hoc queries
- Prepared statements
- Authentication
- Connection pool with auto-reconnect
- Cassandra 1.2 native collections
- Event registration and notification

### TODO
- More docs
- Packaging for Linux and OSX
- Integration tests
- Query tracing


## Examples
In addition to the sample code below there is a fully functional [demo](https://github.com/mstump/libcql/blob/master/demo/main.cpp) which exploits most of the functionality of the library.


### Instantiate a client and connect to Cassandra
```
#include <boost/asio.hpp>
#include <libcql/cql.hpp>
#include <libcql/cql_error.hpp>
#include <libcql/cql_client.hpp>
#include <libcql/cql_client_factory.hpp>
#include <libcql/cql_result.hpp>

int
main(int argc,
     char* argv[])
{
    // setup Boost ASIO
    boost::asio::io_service io_service;

    // instantiate a client
    std::auto_ptr<cql::cql_client_t> client(create_cql_client_t(io_service));

    // Connect to localhost
    client->connect("localhost", 9042, &connect_callback, NULL);

    // Start the event loop
    io_service.run();
    return 0;
}

void
connect_callback(cql::cql_client_t& client)
{
    // Called after a successfull connection, or
    // if using SSL it's called after a successfull handshake.
    // Lets perform an ad-hoc query.

    // perform a query
    client.query("use system;",
                 cql::CQL_CONSISTENCY_ALL,
                 &query_callback,
                 &message_errback);
}

void
query_callback(cql::cql_client_t& client,
               int8_t stream,
               const cql::cql_result_t* result)
{
    // close the connection
    client.close();
}

void
message_errback(cql::cql_client_t& client,
                int8_t stream,
                const cql::cql_error_t& err)
{
    std::cerr << "ERROR " << err.message << std::endl;
}


```


### Connect to the Cassandra cluster using a connection pool
```
#include <boost/asio.hpp>
#include <libcql/cql.hpp>
#include <libcql/cql_error.hpp>
#include <libcql/cql_client_pool.hpp>
#include <libcql/cql_client_pool_factory.hpp>
#include <libcql/cql_result.hpp>

void
message_errback(cql::cql_client_t& client,
                int8_t stream,
                const cql::cql_error_t& err)
{
    std::cerr << "ERROR " << err.message << std::endl;
}

void
use_query_callback(cql::cql_client_t& client,
                   int8_t stream,
                   const cql::cql_result_t* result)
{
    // close the connection
    client.close();
}

void
connect_callback(cql::cql_client_pool_t* pool)
{
    // Called after a successfull connection, or
    // if using SSL it's called after a successfull handshake.
    // Lets perform an ad-hoc query.
    pool->query("USE system;",
                cql::CQL_CONSISTENCY_ALL,
                &use_query_callback,
                &message_errback);
}

/**
  A factory functor which instantiates clients.
  For SSL connections this is how you set OpenSSL cert validation and algo options
*/
struct client_functor_t
{

public:

    client_functor_t(boost::asio::io_service&              service,
                     cql::cql_client_t::cql_log_callback_t log_callback) :
        _io_service(service),
        _log_callback(log_callback)
    {}

    cql::cql_client_t*
    operator()()
    {
        return cql::cql_client_factory_t::create_cql_client_t(_io_service, _log_callback);
    }

private:
    boost::asio::io_service&              _io_service;
    cql::cql_client_t::cql_log_callback_t _log_callback;
};

int
main(int argc,
     char* argv[])
{
    // setup Boost ASIO
    boost::asio::io_service io_service;

    // Create a client factory for the pool
    cql::cql_client_pool_t::cql_client_callback_t client_factory = client_functor_t(io_service, &log_callback);

    // Instantiate the pool
    std::auto_ptr<cql::cql_client_pool_t> pool(cql::cql_client_pool_factory_t::create_client_pool_t(client_factory, &connect_callback, NULL));

    // Connect to localhost
    pool->add_client("localhost", 9042);

    // Start the event loop
    io_service.run();
    return 0;
}

```