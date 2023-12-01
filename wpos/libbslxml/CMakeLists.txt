project(wposxml VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposxml)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Xml)

include_directories(
    /usr/include/libxml2
    /usr/include/libxml2/libxml    
)

set(HEADER_FILES
    comp.h
    utils.h
    xmlconfig.h
    xmlconfigprivate.h
)

set(SOURCES_FILES
    utils.cpp
    xmlconfig.cpp
    xmlconfigprivate.cpp
)

# Make the project root directory the working directory when we run
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../lib)

add_library(
    ${MAIN_TARGET} SHARED
    ${HEADER_FILES}
    ${SOURCES_FILES}
)
target_link_libraries(${MAIN_TARGET} PRIVATE Qt5::Xml xml2)

set_target_properties (${MAIN_TARGET} PROPERTIES
    VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_PATCH}
)

target_include_directories(${MAIN_TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS ${MAIN_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
