cmake_minimum_required(VERSION 3.11)
project(hvr_xml)

if(MSVC)
  set(DLLIMPORT "__declspec(dllimport)")
  set(DLLEXPORT "__declspec(dllexport)")
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/XMLNode.cpp
${PROJECT_SOURCE_DIR}/src/XMLParser.cpp
${PROJECT_SOURCE_DIR}/src/XMLWriter.cpp
)

set(PROJECT_INCS
${PROJECT_SOURCE_DIR}/include/Hvr/XML/XMLNode.h
${PROJECT_SOURCE_DIR}/include/Hvr/XML/XMLParser.h
${PROJECT_SOURCE_DIR}/include/Hvr/XML/XMLWriter.h
)

set(PROJECT_INFO
${PROJECT_SOURCE_DIR}/info.dox
)

# Separate include and src folder on Visual Studio
source_group("include" FILES ${PROJECT_INCS})
source_group("src"     FILES ${PROJECT_SRCS})

# Library for parsing Dash media presentation description
find_package(tinyxml2 REQUIRED)
# Library for lock-free queue
# find_package( Boost COMPONENTS system REQUIRED )
# Library for logging
# find_package( glog REQUIRED )

add_library(${PROJECT_NAME} SHARED ${PROJECT_SRCS}
                                    ${PROJECT_INCS}
                                    ${PROJECT_INFO}
                                    )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

target_compile_definitions(${PROJECT_NAME} INTERFACE
                          HVR_XML_DLL=${DLLIMPORT})

target_compile_definitions(${PROJECT_NAME} PRIVATE
                          HVR_XML_DLL=${DLLEXPORT})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC)

target_link_libraries(${PROJECT_NAME}
                      # glog::glog
                      tinyxml2
                      )

set_property(TARGET ${PROJECT_NAME} PROPERTY
            FOLDER ${CMAKE_PROJECT_NAME}/modules)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib COMPONENT install-app
        LIBRARY DESTINATION lib COMPONENT install-app
        RUNTIME DESTINATION bin COMPONENT install-app)

install(FILES
  ${PROJECT_SOURCE_DIR}/include/Hvr/XML/XMLNode.h
  ${PROJECT_SOURCE_DIR}/include/Hvr/XML/XMLParser.h
  ${PROJECT_SOURCE_DIR}/include/Hvr/XML/XMLWriter.h
  DESTINATION include/Hvr/XML)

export(TARGETS
        ${PROJECT_NAME}
        APPEND FILE "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake")

file(APPEND ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    "find_dependency(tinyxml2 REQUIRED)\n"
    # "find_dependency( Boost COMPONENTS system REQUIRED )\n"
    # "find_dependency( glog REQUIRED )\n"
    )

