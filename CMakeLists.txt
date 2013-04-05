CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)
set(PROJECT_NAME_STR cql)
project(${PROJECT_NAME_STR} C CXX)

#-------------------
# The version number
#-------------------

set (libcql_VERSION_MAJOR 0)
set (libcql_VERSION_MINOR 5)
set (libcql_VERSION_STRING ${libcql_VERSION_MAJOR}.${libcql_VERSION_MINOR})

#-------------------
# setup deps
#-------------------
set(EXT_PREFIX ${PROJECT_SOURCE_DIR}/ext)
set(CMAKE_MODULE_PATH ${EXT_PREFIX})

# OS specific lib stuff
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME ON)

  include(OpenSSL)
  include(ZLib)
  set(LIBS ${LIBS} ${openssl_STATIC_LIBRARIES} )
  set(LIBS ${LIBS} ${zlib_STATIC_LIBRARIES} )
else()
  set(Boost_USE_STATIC_LIBS OFF)
  set(Boost_USE_STATIC_RUNTIME OFF)

  find_package(OpenSSL)
  if (${OPENSSL_FOUND})
    set(LIBS ${LIBS} ${OPENSSL_LIBRARIES} )
  else()
    message(FATAL_ERROR "couldn't find OpenSSL")
  endif()
endif()

# Boost
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.47.0 COMPONENTS system)
set(LIBS ${LIBS} ${Boost_LIBRARIES} )

# Threading
find_package(Threads)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT} )

# google test
include(gtest)

add_custom_target (AppDependencies ALL
  DEPENDS ${APP_DEPENDENCIES}
  COMMENT "Installing any missing dependencies.")

# Build up the include pathes
set(INCLUDES ${INCLUDES} ${openssl_INCLUDE_DIRS} )
set(INCLUDES ${INCLUDES} ${zlib_INCLUDE_DIRS} )
set(INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIRS} )

#-------------------
# libcql static and shared
#-------------------
set(PROJECT_LIB_NAME ${PROJECT_NAME_STR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build/bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDES} ${PROJECT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/libcql/internal/*.cpp ${PROJECT_SOURCE_DIR}/src/libcql/*.cpp)

add_library(${PROJECT_LIB_NAME} SHARED ${SRC_FILES})
add_library(${PROJECT_LIB_NAME}-static STATIC ${SRC_FILES})

set_property(
  TARGET ${PROJECT_LIB_NAME}
  APPEND PROPERTY COMPILE_FLAGS "-Wall -Werror -ansi -Wno-deprecated -pthread")

set_property(
  TARGET ${PROJECT_LIB_NAME}-static
  APPEND PROPERTY COMPILE_FLAGS "-Wall -Werror -ansi -Wno-deprecated -pthread")

target_link_libraries(${PROJECT_LIB_NAME} ${LIBS})

#-------------------
# install target
#-------------------

FILE(GLOB INSTALL_HEADERS "${PROJECT_INCLUDE_DIR}/libcql/*.hpp")
INSTALL(FILES ${INSTALL_HEADERS} DESTINATION include/libcql-${libcql_VERSION_STRING}/libcql)

INSTALL(TARGETS ${PROJECT_LIB_NAME}-static ${PROJECT_LIB_NAME}
  RUNTIME DESTINATION bin/libcql-${libcql_VERSION_STRING}
  LIBRARY DESTINATION lib/libcql-${libcql_VERSION_STRING}
  ARCHIVE DESTINATION lib/libcql-${libcql_VERSION_STRING})


#-------------------
# uninstall target
#-------------------

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)


#-------------------
# unit tests
#-------------------

enable_testing()
add_subdirectory(test/unit_tests)

#-------------------
# the demo program
#-------------------

add_subdirectory(demo)
