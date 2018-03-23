cmake_minimum_required(VERSION 3.8)
project(XMLSample)

if(MSVC)
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/XMLSample.cpp
)

source_group("src" FILES ${PROJECT_SRCS})

find_package(Boost COMPONENTS filesystem REQUIRED)

add_executable(${PROJECT_NAME} ${PROJECT_SRCS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

target_link_libraries(${PROJECT_NAME} hvr_xmlparser Boost::filesystem)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${CMAKE_PROJECT_NAME}/apps)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        DESTINATION bin COMPONENT install-app)

if(MSVC)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy
              ${PROJECT_SOURCE_DIR}/sample.xml
              ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/data)

elseif((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR CMAKE_COMPILER_IS_GNUCC
    OR CMAKE_COMPILER_IS_GNUCXX)

  file(GLOB XMLPARSER_DATA ${PROJECT_SOURCE_DIR}/sample.xml)

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/xmlparserdata.cmake
    "file(COPY ${XMLPARSER_DATA} DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/data
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_WRITE GROUP_READ WORLD_READ)")

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
                    ${CMAKE_COMMAND}
                  -P ${CMAKE_CURRENT_BINARY_DIR}/xmlparserdata.cmake)
endif()
