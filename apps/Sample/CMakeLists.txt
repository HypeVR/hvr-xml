cmake_minimum_required(VERSION 3.8)
project(SaveStream)

if(MSVC)
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/SaveStreamXML.cpp
)

source_group("src" FILES ${PROJECT_SRCS})

find_package(REDSDK REQUIRED)

find_package(Boost COMPONENTS filesystem REQUIRED)

add_executable(${PROJECT_NAME} ${PROJECT_SRCS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

target_link_libraries(${PROJECT_NAME} hvr_stream
                                      red::REDSDK
                                      Boost::filesystem)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${CMAKE_PROJECT_NAME}/apps)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        DESTINATION bin COMPONENT install-app)
