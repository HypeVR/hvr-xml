cmake_minimum_required(VERSION 3.11)

project(XMLTest)

set(XML_INCS
${PROJECT_SOURCE_DIR}/include/XMLTest.h
)

set(XML_SRCS
${PROJECT_SOURCE_DIR}/src/XMLTest.cpp
${PROJECT_SOURCE_DIR}/src/main.cpp
)

find_package(GTest CONFIG REQUIRED)
find_package(Boost COMPONENTS filesystem thread REQUIRED)

add_executable(XMLTest ${XML_SRCS} ${XML_INCS})

target_include_directories(${PROJECT_NAME} PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

target_link_libraries(XMLTest
                      hvr_xml
                      GTest::gtest_main
                      Boost::filesystem)

set_property(TARGET XMLTest PROPERTY FOLDER
            ${CMAKE_PROJECT_NAME}/unit-test)

if(MSVC)
  add_custom_command(TARGET XMLTest POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_directory
              ${PROJECT_SOURCE_DIR}/data/
              ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/data)

elseif((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR CMAKE_COMPILER_IS_GNUCC
    OR CMAKE_COMPILER_IS_GNUCXX)

  file(GLOB XML_DATA ${PROJECT_SOURCE_DIR}/data/*)

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/xmldata.cmake
    "file(COPY ${XML_DATA} DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/data
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_WRITE GROUP_READ WORLD_READ)")

  add_custom_command(TARGET XMLTest POST_BUILD COMMAND
                    ${CMAKE_COMMAND}
                  -P ${CMAKE_CURRENT_BINARY_DIR}/xmldata.cmake)
endif()
