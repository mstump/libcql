CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)
set(PROJECT_NAME_STR cql)
project(${PROJECT_NAME_STR} C CXX)

set(EXT_PREFIX ${PROJECT_SOURCE_DIR}/ext)
set(CMAKE_MODULE_PATH ${EXT_PREFIX})

#-------------------
# setup deps
#-------------------

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include(openssl)
else()
  find_package(openssl)
endif()

include(gtest)

# Find Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.49.0 COMPONENTS system)

add_custom_target (AppDependencies ALL
  DEPENDS ${APP_DEPENDENCIES}
  COMMENT "Installing any missing dependencies.")

# Build up the lib and include pathes
set(LIBS ${LIBS} ${Boost_LIBRARIES} )
set(LIBS ${LIBS} ${zlib_STATIC_LIBRARIES} )
set(LIBS ${LIBS} ${openssl_STATIC_LIBRARIES} )

set(INCLUDES ${INCLUDES} ${openssl_INCLUDE_DIRS} )
set(INCLUDES ${INCLUDES} ${zlib_INCLUDE_DIRS} )
set(INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIRS} )

#-------------------
# libcql static and shared
#-------------------

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-Wall -ansi -Wno-deprecated -pthread)
endif()

set(PROJECT_LIB_NAME ${PROJECT_NAME_STR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build/bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDES} ${PROJECT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/libcql/*.cpp ${PROJECT_SOURCE_DIR}/src/libcql/internal/*.cpp)

add_library(${PROJECT_LIB_NAME} SHARED ${SRC_FILES})
add_library(${PROJECT_LIB_NAME}-static STATIC ${SRC_FILES})
target_link_libraries(${PROJECT_LIB_NAME} ${LIBS})

install(DIRECTORY ${PROJECT_INCLUDE_DIR}/libcql
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.hpp")

INSTALL(TARGETS ${PROJECT_LIB_NAME}-static ${PROJECT_LIB_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)


enable_testing()
add_subdirectory(demo)
add_subdirectory(test/unit_tests)