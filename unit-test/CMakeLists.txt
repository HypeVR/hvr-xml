cmake_minimum_required(VERSION 3.11)
project(unit-test)

if((NOT TARGET test) AND (NOT TARGET RUN_UNIT_TEST))
  if(MSVC)
    add_custom_target(RUN_UNIT_TEST DEPENDS unit-test-cmd)

    add_custom_command(
      OUTPUT unit-test-cmd
      COMMAND ${CMAKE_BINARY_DIR}//bin//$<CONFIGURATION>//XMLTest.exe)
  elseif((CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
         OR CMAKE_COMPILER_IS_GNUCC
         OR CMAKE_COMPILER_IS_GNUCXX)
    add_custom_target(test DEPENDS unit-test-cmd)

    add_custom_command(OUTPUT unit-test-cmd
                       COMMAND ${CMAKE_CURRENT_BINARY_DIR}/XML/XMLTest)
  endif()
endif()

add_subdirectory(XML)
