cmake_minimum_required(VERSION 3.8)
project(unit-test)

if((NOT TARGET test) AND (NOT TARGET RUN_UNIT_TEST))
  # for visual studio
  set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll")
  string(REPLACE "/w44668" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REPLACE "/w44365" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REPLACE "/w44571" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REPLACE "/w44640" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

  set(CMAKE_CXX_STANDARD 14)

  # Download and unpack gtest at configure time
  configure_file(gtest.txt gtest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gtest-download)
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gtest-download)

  # Add gtest directly to our build
  add_subdirectory(${CMAKE_BINARY_DIR}/gtest-src
                  ${CMAKE_BINARY_DIR}/gtest-build
                  EXCLUDE_FROM_ALL)

  if(MSVC)
    add_custom_target(RUN_UNIT_TEST
        COMMAND ${CMAKE_BINARY_DIR}//bin//$<CONFIGURATION>//XMLTest.exe)
  elseif((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR CMAKE_COMPILER_IS_GNUCC
    OR CMAKE_COMPILER_IS_GNUCXX)
      add_custom_target(test
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/XML/XMLTest)
  endif()
endif()

add_subdirectory(XML)
