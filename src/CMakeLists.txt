set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.49.0 COMPONENTS system)

add_executable(cql cql.cpp)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(cql ${Boost_LIBRARIES})
