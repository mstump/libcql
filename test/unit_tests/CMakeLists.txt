
#-------------------
# Test
#-------------------
enable_testing()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${INCLUDES} ${PROJECT_SOURCE_DIR}/src/)

file(GLOB TEST_SRC_FILES *.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} gtest)
target_link_libraries(${PROJECT_TEST_NAME} ${gtest_STATIC_LIBRARIES} ${LIBS} ${PROJECT_LIB_NAME})
add_test(${PROJECT_TEST_NAME} ${PROJECT_TEST_NAME})
