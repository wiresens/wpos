project(wposreceipt VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposreceipt)
set(CMAKE_AUTOMOC ON)
set(WS_SERVICE com.wiresens)

include_directories(
    /usr/include/libxml2
    ..
)

find_package(Qt5 REQUIRED COMPONENTS Core DBus Sql)

#Invoke qdbuscpp2xml to generate .xml spec files form .h
qt5_generate_dbus_interface(
    dbusreceiptprimitive.h
    "${WS_SERVICE}.dbusreceiptprimitive.xml"
    OPTIONS -M  #export all Public Slot
)

#Invoke qdbuscpp2xml to generate .xml spec files form .h
qt5_generate_dbus_interface(
    dbusreceiptmediator.h
    "${WS_SERVICE}.dbusreceiptmediator.xml"
    OPTIONS -M  #export all Public Slot
)

#Invoke qdbusxml2cpp to generate *_adaptor.{h,cpp} from .xml spec files
qt_add_dbus_adaptor(DBUS_XML2CPP_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${WS_SERVICE}.dbusreceiptprimitive.xml"
    dbusreceiptprimitive.h
    DBusReceiptPrimitive
    dbusreceiptprimitive_adaptor
)

#Invoke qdbusxml2cpp to generate *_adaptor.{h,cpp} from .xml spec files
qt_add_dbus_adaptor(DBUS_XML2CPP_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${WS_SERVICE}.dbusreceiptmediator.xml"
    dbusreceiptmediator.h
    DBusReceiptMediator
    dbusreceiptmediator_adaptor
)

set(HEADER_FILES
    dbusreceiptmanager.h
    dbusreceiptmediator.h
    dbusreceiptprimitive.h
    receiptchangenotifiyer.h
    receiptclient.h
    receiptclientmediator.h
    receiptdb.h
    receiptmediatorinterface.h
    receiptprimitiveinterface.h
)

set(SOURCES_FILES
    dbusreceiptmanager.cpp
    dbusreceiptmediator.cpp
    dbusreceiptprimitive.cpp
    receiptchangenotifiyer.cpp
    receiptclient.cpp
    receiptclientmediator.cpp
    receiptdb.cpp
    main.cpp
)

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
    wposxml wposcore wposrpc
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    Qt5::Core Qt5::DBus Qt5::Sql
)

install(TARGETS ${MAIN_TARGET}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
