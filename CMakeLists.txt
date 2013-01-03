cmake_minimum_required(VERSION 2.8.10)
set(PROJECT_NAME_STR cql)
project(${PROJECT_NAME_STR} C CXX)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-Wall -ansi -Wno-deprecated -pthread)
endif()

# look for dylibs on OSX
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   SET(OperatingSystem "Mac OS X")
   SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

#-------------------
# find boost
#-------------------
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.49.0 COMPONENTS system)

#-------------------
# set common include folder for module
#-------------------
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

#-------------------
# Module source
#-------------------
# set output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build/bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

set(PROJECT_LIB_NAME ${PROJECT_NAME_STR})
include_directories(${COMMON_INCLUDES} ${Boost_INCLUDE_DIRS})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_LIB_NAME} SHARED ${SRC_FILES})
add_library(${PROJECT_LIB_NAME}-static STATIC ${SRC_FILES})
target_link_libraries(${PROJECT_LIB_NAME} ${Boost_LIBRARIES})

SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

#-------------------
# Demo
#-------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "demo")
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_demo)
include_directories(${COMMON_INCLUDES})

file(GLOB DEMO_SRC_FILES ${PROJECT_SOURCE_DIR}/demo/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${DEMO_SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_LIB_NAME})

#-------------------
# Test
#-------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "test")
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(${PROJECT_TEST_NAME} pthread)
  target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a)
else()
  target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_LIBS_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES})
  target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_LIBS_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES})
endif()

target_link_libraries(${PROJECT_TEST_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_LIB_NAME})

add_test(test1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_TEST_NAME})
