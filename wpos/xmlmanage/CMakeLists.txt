project(xmlmanage VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET xmlmanage)
set(CMAKE_AUTOMOC ON)

#find_package(Qt5 REQUIRED COMPONENTS Core)
#qt_standard_project_setup() // >= Qt6.3
find_package(Qt5 REQUIRED COMPONENTS Core)

include_directories(
    /usr/include/libxml2
)

set(HEADER_FILES
    comon.h
    fileutils.h
    helpclass.h
    xmldelete.h
    xmllist.h
    xmlread.h
    xmlwrite.h
)

set(SOURCES_FILES
    fileutils.cpp
    helpclass.cpp
    main.cpp
    xmldelete.cpp
    xmllist.cpp
    xmlread.cpp
    xmlwrite.cpp
)

add_executable(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}
)

target_link_libraries(${MAIN_TARGET} PRIVATE wposxml Qt5::Core)

install(TARGETS ${MAIN_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
