project(wposrpc VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposrpc)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Network Xml WebSockets)

include_directories(
    /usr/include/libxml2
)

set(HEADER_FILES
    xrbase64.h
    xrclient.h
    xrfaultcodes.h
    xrmethodcall.h
    xrmethodresponse.h
    xrvariant.h
    rpcserver.h
    rpcserverimp.h
    xmlrpcserver.h
    rpcsocket.h
    tcprpcserver.h
    rpcencoder.h
)

set(SOURCES_FILES
    xrbase64.cpp
    xrclient.cpp
    xrmethodcall.cpp
    xrmethodresponse.cpp
    xrvariant.cpp
    rpcserver.cpp
    rpcserverimp.cpp
    xmlrpcserver.cpp
    rpcsocket.cpp
    tcprpcserver.cpp
    rpcencoder.cpp
)

# Make the project root directory the working directory when we run
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../lib)

add_library(
    ${MAIN_TARGET} SHARED
    ${HEADER_FILES}
    ${SOURCES_FILES}
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    wposxml
    Qt5::Network
    Qt5::Xml
    Qt5::WebSockets
)

target_include_directories(${MAIN_TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties (${MAIN_TARGET} PROPERTIES
    VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_PATCH}
)

install(TARGETS ${MAIN_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
