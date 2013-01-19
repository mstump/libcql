CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)
set(PROJECT_NAME_STR cql)
project(${PROJECT_NAME_STR} C CXX)

include (ExternalProject)

set(EXT_PREFIX ${PROJECT_SOURCE_DIR}/ext)

ExternalProject_Add(zlib
  PREFIX ${EXT_PREFIX}
  URL http://zlib.net/zlib-1.2.7.tar.gz
  URL_MD5 "60df6a37c56e7c1366cca812414f7b85"
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ./configure -p=${EXT_PREFIX} --static --64
  BUILD_COMMAND make
  INSTALL_COMMAND make install prefix=${EXT_PREFIX}
  BUILD_IN_SOURCE 1
  )

ExternalProject_Add(openssl
  DEPENDS zlib
  PREFIX ${EXT_PREFIX}
  URL http://openssl.org/source/openssl-1.0.0c.tar.gz
  URL_MD5 "ff8fb85610aef328315a9decbb2712e4"
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ./Configure darwin64-x86_64-cc zlib no-shared no-zlib-dynamic --prefix=${EXT_PREFIX}
  BUILD_IN_SOURCE 1
  )

ExternalProject_Add(googletest
  PREFIX ${EXT_PREFIX}
  SVN_REPOSITORY http://googletest.googlecode.com/svn/tags/release-1.6.0
  CMAKE_ARGS -DBUILD_SHARED_LIBS=ON

  CONFIGURE_COMMAND ${CMAKE_COMMAND} ${gtest_SRC_DIR} -DCMAKE_INSTALL_PREFIX=${EXT_PREFIX} -DCMAKE_PREFIX_PATH=${EXT_PREFIX}
  BUILD_COMMAND make
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  BUILD_IN_SOURCE 1
)

#-------------------
# find boost
#-------------------
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.49.0 COMPONENTS system)

find_path(OPENSSL_INCLUDE_DIR
  openssl/ssl.h
  PATHS ${EXT_PREFIX}/include/
  PATH_SUFFIXES openssl
  NO_DEFAULT_PATH
  NO_CMAKE_ENVIRONMENT_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_PATH
  NO_CMAKE_SYSTEM_PATH
  NO_CMAKE_FIND_ROOT_PATH)

find_library(OPENSSL_LIBRARY
  NAMES libssl.a
  PATHS ${EXT_PREFIX}/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenSSL DEFAULT_MSG OPENSSL_LIBRARY OPENSSL_INCLUDE_DIR)
mark_as_advanced(OPENSSL_INCLUDE_DIR OPENSSL_LIBRARY)


set(LIBS ${source_dir}/include)
set(LIBS ${LIBS} ${Boost_LIBRARIES})
set(LIBS ${LIBS} ${EXT_PREFIX}/lib/libz.a)
set(LIBS ${LIBS} ${EXT_PREFIX}/lib/libcrypto.a)
set(LIBS ${LIBS} ${EXT_PREFIX}/lib/libssl.a)

#-------------------
# Module source
#-------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build/bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

set(PROJECT_LIB_NAME ${PROJECT_NAME_STR})
include_directories(${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${source_dir}/include)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)


add_library(${PROJECT_LIB_NAME} SHARED ${SRC_FILES})
add_library(${PROJECT_LIB_NAME}-static STATIC ${SRC_FILES})


target_link_libraries(${PROJECT_LIB_NAME} ${LIBS})

SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ext)


# CMAKE_MODULE_PATH

# # Specify include dir
# ExternalProject_Get_Property(googletest source_dir)
# set(GTEST_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)

# # Specify MainTest's link libraries
# ExternalProject_Get_Property(googletest binary_dir)
# set(GTEST_LIBS_DIR ${binary_dir} PARENT_SCOPE)




# #find_package(openssl 1.0.0 COMPONENTS system)




#   # # look for dylibs on OSX
#   # IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#   #   # Mac OS X specific code
#   #   SET(OperatingSystem "Mac OS X")
#   #   SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
#   # ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


# if(CMAKE_COMPILER_IS_GNUCXX)
#   add_definitions(-Wall -ansi -Wno-deprecated -pthread)
# endif()



  # #-------------------
  # # Demo
  # #-------------------
  # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "demo")
  # set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_demo)
  # include_directories(${COMMON_INCLUDES})

  # file(GLOB DEMO_SRC_FILES ${PROJECT_SOURCE_DIR}/demo/*.cpp)
  # add_executable(${PROJECT_TEST_NAME} ${DEMO_SRC_FILES})
  # target_link_libraries(${PROJECT_TEST_NAME} ${Boost_LIBRARIES})
  # target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_LIB_NAME})

  #-------------------
  # Test
  #-------------------
  # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "test")
  # enable_testing()
  # set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
  # include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})

  # file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
  # add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
  # add_dependencies(${PROJECT_TEST_NAME} googletest)

  # if(CMAKE_COMPILER_IS_GNUCXX)
  #   target_link_libraries(${PROJECT_TEST_NAME} pthread)
  #   target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a)
  # else()
  #   target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_LIBS_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES})
  #   target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_LIBS_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES})
  # endif()

  # target_link_libraries(${PROJECT_TEST_NAME} ${Boost_LIBRARIES})
  # target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_LIB_NAME})

  # add_test(test1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_TEST_NAME})


  ############################################################################
#endif()
