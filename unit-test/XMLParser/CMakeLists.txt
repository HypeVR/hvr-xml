cmake_minimum_required(VERSION 3.8)

project(XMLParserTest)

set(XMLPARSER_INCS
${PROJECT_SOURCE_DIR}/include/XMLParserTest.h
)

set(XMLPARSER_SRCS
${PROJECT_SOURCE_DIR}/src/XMLParserTest.cpp
${PROJECT_SOURCE_DIR}/src/main.cpp
)

find_package(Boost COMPONENTS filesystem thread REQUIRED)
find_package(tinyxml2 REQUIRED)

add_executable(XMLParserTest ${XMLPARSER_SRCS} ${XMLPARSER_INCS})

target_include_directories(${PROJECT_NAME} PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

target_link_libraries(XMLParserTest
                      hvr_xmlparser
                      gtest_main
                      Boost::filesystem)

set_property(TARGET XMLParserTest PROPERTY FOLDER
            ${CMAKE_PROJECT_NAME}/unit-test)

if(MSVC)
  add_custom_command(TARGET XMLParserTest POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_directory
              ${PROJECT_SOURCE_DIR}/data/
              ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/data)

elseif((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR CMAKE_COMPILER_IS_GNUCC
    OR CMAKE_COMPILER_IS_GNUCXX)

  file(GLOB XMLPARSER_DATA ${PROJECT_SOURCE_DIR}/data/*)

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/xmlparserdata.cmake
    "file(COPY ${XMLPARSER_DATA} DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/data
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_WRITE GROUP_READ WORLD_READ)")

  add_custom_command(TARGET XMLParserTest POST_BUILD COMMAND
                    ${CMAKE_COMMAND}
                  -P ${CMAKE_CURRENT_BINARY_DIR}/xmlparserdata.cmake)
endif()
