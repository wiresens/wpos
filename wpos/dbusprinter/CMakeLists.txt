project(wposprinter VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposprinter)

set(CMAKE_AUTOMOC ON)
set(WS_SERVICE com.wiresens)

include_directories(
    /usr/include/libxml2
    ..
)

find_package(Qt5 REQUIRED COMPONENTS Core DBus Sql)

set(DBUS_HEADER_FILE
    dbusprinter
)

#Invoke qdbuscpp2xml to generate .xml spec files form .h
qt5_generate_dbus_interface(
    "${DBUS_HEADER_FILE}.h"
    "${WS_SERVICE}.${DBUS_HEADER_FILE}.xml"
    OPTIONS -M  #export all Public Slot
)

#Invoke qdbusxml2cpp to generate *_adaptor.{h,cpp} from .xml spec files
qt_add_dbus_adaptor(DBUS_XML2CPP_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${WS_SERVICE}.${DBUS_HEADER_FILE}.xml"
    "${DBUS_HEADER_FILE}.h"
    DBusPrinter
    "${DBUS_HEADER_FILE}_adaptor"
)

set(HEADER_FILES
    dbusprinter.h
    dbusprinterinterface.h
    main.h
    printer.h
    printermanager.h
    printersamsungsrp350.h
    printerticket.h
    xmlrpcprinter.h
)

set(SOURCES_FILES
    dbusprinter.cpp
    main.cpp
    printer.cpp
    printermanager.cpp
    printersamsungsrp350.cpp
    printerticket.cpp
    xmlrpcprinter.cpp
)

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

add_executable(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}
    ${DBUS_XML2CPP_FILES}
)

target_include_directories(${MAIN_TARGET}  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    wposxml wposcore wposrpc cups
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    Qt5::Core Qt5::DBus Qt5::Sql
)

install(TARGETS ${MAIN_TARGET}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#qt_generate_deploy_app_script( // >= Qt 6.3
#    TARGET ${MAIN_TARGET}
#    OUTPUT_SCRIPT deploy_script
#    NO_UNSUPPORTED_PLATFORM_ERROR
#)

#install(SCRIPT ${deploy_script})

