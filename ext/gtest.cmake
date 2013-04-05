#
# Install gtest from source
#

if(NOT gtest_NAME)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

include(ExternalProject)

set(ABBREV "gtest")
set(${ABBREV}_NAME         ${ABBREV})
set(${ABBREV}_INCLUDE_DIRS ${EXT_PREFIX}/include)
set(${ABBREV}_SRC_DIR ${EXT_PREFIX}/src/${ABBREV})

set(APP_DEPENDENCIES ${APP_DEPENDENCIES} ${ABBREV})

message("Installing ${gtest_NAME} into ext build area: ${EXT_PREFIX} ...")

ExternalProject_Add(gtest
  PREFIX ${EXT_PREFIX}
  SVN_REPOSITORY http://googletest.googlecode.com/svn/tags/release-1.6.0
  CMAKE_ARGS -DBUILD_SHARED_LIBS=ON

  CONFIGURE_COMMAND ${CMAKE_COMMAND} ${gtest_SRC_DIR} -DCMAKE_INSTALL_PREFIX=${EXT_PREFIX}
  BUILD_COMMAND make
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  BUILD_IN_SOURCE 1
)

ExternalProject_add_step(${gtest_NAME} install_includes
    DEPENDEES   build
    COMMAND     ${CMAKE_COMMAND} -E copy_directory ${gtest_SRC_DIR}/include/gtest ${EXT_PREFIX}/include/gtest
    COMMENT     "Placed gtest include files in ${EXT_PREFIX}include"
)

ExternalProject_add_step(${gtest_NAME} install_library
    DEPENDEES   install_includes
    COMMAND     ${CMAKE_COMMAND} -E copy ${gtest_SRC_DIR}/libgtest.a ${EXT_PREFIX}/lib
    COMMENT     "Placed libgtest.a in ${EXT_PREFIX}/lib"
)

ExternalProject_add_step(${gtest_NAME} install_library2
    DEPENDEES   install_includes
    COMMAND     ${CMAKE_COMMAND} -E copy ${gtest_SRC_DIR}/libgtest_main.a ${EXT_PREFIX}/lib
    COMMENT     "Placed libgtest_main.a in ${EXT_PREFIX}/lib"
)

set(gtest_STATIC_LIBRARIES ${EXT_PREFIX}/lib/libgtest.a ${EXT_PREFIX}/lib/libgtest_main.a)

set_target_properties(${gtest_NAME} PROPERTIES EXCLUDE_FROM_ALL ON)

endif(NOT gtest_NAME)
