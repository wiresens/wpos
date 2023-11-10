project(wposrpcd    LANGUAGES   CXX)

set(HEADER_FILES
    kxmlrpcdaemon.h
    kxmlrpcparser.h
    kxmlrpcserver.h
    kxmlrpcutil.h
)

set(SOURCES_FILES
    kxmlrpcdaemon.cpp
    kxmlrpcparser.cpp
    kxmlrpcserver.cpp
    kxmlrpcutil.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCES_FILES}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION  ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION  ${CMAKE_INSTALL_BINDIR}
)
