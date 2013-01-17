cmake_minimum_required(VERSION 2.8.10)
project(gtest_builder C CXX)
include(ExternalProject)

set(OWN_G_PREFIX "${CMAKE_BINARY_DIR}/ext/gtest")

ExternalProject_Add(googletest
    SVN_REPOSITORY http://googletest.googlecode.com/svn/tags/release-1.6.0
    CMAKE_ARGS -DBUILD_SHARED_LIBS=ON
    INSTALL_COMMAND "" # Disable install step
    UPDATE_COMMAND "" # Disable update step
)

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)

# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest binary_dir)
set(GTEST_LIBS_DIR ${binary_dir} PARENT_SCOPE)
