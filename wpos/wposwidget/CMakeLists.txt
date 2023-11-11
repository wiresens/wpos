project(wposwidget VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET   wposwidget)  #wposwidget.so.${VERSION}
set(EXEC_TARGET   wposviewer)
set(PLUGIN_TARGET wposviewer)  #wposviewer.so

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(WS_SERVICE com.wiresens)

find_package(Qt5 REQUIRED COMPONENTS Widgets DBus Pdf PdfWidgets)

set(DBUS_HEADER_FILE dbusprinter)
set(DBUS_DEPENDENCY_DIR "${CMAKE_CURRENT_BINARY_DIR}/../dbusprinter")

qt_add_dbus_interface(
    DBUS_XML2CPP_FILES
    "${DBUS_DEPENDENCY_DIR}/${WS_SERVICE}.${DBUS_HEADER_FILE}.xml"
    ${DBUS_HEADER_FILE}_interface
)

include_directories(
    /usr/include/libxml2
    ..
)

set(HEADER_FILES
    basickeyboardbox.h
    dragobjects.h
    effects.h
    floatkeyboardbox.h
    global.h
    #iwidgetconfig.h
    iwidgetconfighelper.h
    menusmanager.h
    menupage.h
    menustack.h
    numkeyboardbox.h
    ordercontentview.h
    #ordermanagerconfig.h
    ordercontrols.h
    orderfooterview.h
    orderheaderview.h
    orderpickerview.h
    orderview.h
    pdfviewer/pageselector.h
    pdfviewer/pdfviewer.h
    pdfviewer/zoomselector.h
    popupkeyboard.h
    spkeyboard.h
    treeview.h
    visualproduct.h
    toolkit.h
    poslabel.h
)

set(SOURCES_FILES
    basickeyboardbox.cpp
    dragobjects.cpp
    effects.cpp
    floatkeyboardbox.cpp
    global.cpp
    #iwidgetconfig.cpp
    iwidgetconfighelper.cpp
    menusmanager.cpp
    menupage.cpp
    menustack.cpp
    numkeyboardbox.cpp
    ordercontentview.cpp
    #ordermanagerconfig.cpp
    ordercontrols.cpp
    orderfooterview.cpp
    orderheaderview.cpp
    orderpickerview.cpp
    orderview.cpp
    pdfviewer/main.cpp
    pdfviewer/pageselector.cpp
    pdfviewer/pdfviewer.cpp
    pdfviewer/zoomselector.cpp
    popupkeyboard.cpp
    spkeyboard.cpp
    treeview.cpp
    visualproduct.cpp
    toolkit.cpp
    poslabel.cpp
)

set(UIC_FILES
    keyboardwidget.ui
    loungewidget.ui
    menusmanager.ui
    numkeyboard.ui
    pdfviewer/pdfviewer.ui
    spkeyboardwidget.ui    
)

set(QRC_FILES
    pdfviewer/pdfviewer.qrc
)

#Consider adding target to build pdfviewer as a librarypluging and a standalone executable
add_library(
    ${MAIN_TARGET} SHARED
    ${HEADER_FILES}
    ${SOURCES_FILES}
    ${DBUS_XML2CPP_FILES}
    ${UIC_FILES}
    ${QRC_FILES}
)

target_include_directories(${MAIN_TARGET}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
    #include-search paths as usage-requirements for consumers
    INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/${MAIN_TARGET}_autogen/include
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    wposxml wposcore
)

add_dependencies(${MAIN_TARGET} wposprinter)

target_link_libraries(${MAIN_TARGET} PRIVATE
    Qt5::Widgets Qt5::DBus Qt5::Pdf Qt5::PdfWidgets
)

set_target_properties (${MAIN_TARGET} PROPERTIES
    VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_PATCH}
)

install(TARGETS ${MAIN_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
