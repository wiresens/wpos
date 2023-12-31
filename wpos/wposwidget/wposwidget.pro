# File generated by kdevelop's qmake manager.
# -------------------------------------------
# Subdirectory relative to the main directory of the project: ./ntpvlibwidget
# The target is a library: ntpvlibwidget

QT              += widgets dbus pdf pdfwidgets

CONFIG          += debug -Wall warn_on qt dll
#CONFIG         += release warn_off qt dll
TARGET          = wposwidget
TEMPLATE        = lib
VERSION         = 0.4

QMAKE_CXXFLAGS  += -std=c++20
QMAKE_CXXFLAGS  += -Wno-deprecated-copy  -Wno-narrowing

INSTALLS        += target
target.path     = /usr/lib

TARGETDEPS     += \
    ../wposcore/libwposcore.so \
   ../libbslxml/libbslxml.so

LIBS  += \
    -lwposcore \
    -lbslxml

QMAKE_LIBDIR = \
    ../wposcore \
    ../libbslxml

MOC_DIR      = moc
UI_DIR       = ui
OBJECTS_DIR  = obj

INCLUDEPATH += \
    /usr/include/libxml2 \
    ../wposcore \
    ../libbslxml \
    ..\
    .

# Input of qdbusxml2cpp to generate *_interface.{h,cpp},
# qdbusxml2cpp is automatically invoqued by qmake
DBUS_INTERFACES   += ../dbusprinter/com.wiresens.dbusprinter.xml

HEADERS += \
    basickeyboardbox.h \
    dragobjects.h \
    effects.h \
    floatkeyboardbox.h \
    global.h \
    #iwidgetconfig.h \
    iwidgetconfighelper.h \
    menusmanager.h \
    menupage.h \
    menustack.h \
    numkeyboardbox.h \
    ordercontentview.h \
    #ordermanagerconfig.h \
    ordercontrols.h \
    orderfooterview.h \
    orderheaderview.h \
    orderpickerview.h \
    orderview.h \
    pdfviewer/pageselector.h \
    pdfviewer/pdfviewer.h \
    pdfviewer/zoomselector.h \
    popupkeyboard.h \
    spkeyboard.h \
    treeview.h \
    visualproduct.h \
    toolkit.h \
    poslabel.h

SOURCES += \
    basickeyboardbox.cpp \
    dragobjects.cpp \
    effects.cpp \
    floatkeyboardbox.cpp \
    global.cpp \
    #iwidgetconfig.cpp \
    iwidgetconfighelper.cpp \
    menusmanager.cpp \
    menupage.cpp \
    menustack.cpp \
    numkeyboardbox.cpp \
    ordercontentview.cpp \
    #ordermanagerconfig.cpp \
    ordercontrols.cpp \
    orderfooterview.cpp \
    orderheaderview.cpp \
    orderpickerview.cpp \
    orderview.cpp \
    pdfviewer/main.cpp \
    pdfviewer/pageselector.cpp \
    pdfviewer/pdfviewer.cpp \
    pdfviewer/zoomselector.cpp \
    popupkeyboard.cpp \
    spkeyboard.cpp \
    treeview.cpp \
    visualproduct.cpp \
    toolkit.cpp \
    poslabel.cpp

FORMS += \
    keyboardwidget.ui \
    loungewidget.ui \
    menusmanager.ui \
    numkeyboard.ui \
    pdfviewer/pdfviewer.ui \
    spkeyboardwidget.ui

RESOURCES += \
    pdfviewer/pdfviewer.qrc

DISTFILES += \
    pdfviewer/images/busy.png \
    pdfviewer/images/fileopen.png \
    pdfviewer/images/go-next-24.png \
    pdfviewer/images/go-previous-24.png \
    pdfviewer/images/zoom-in-24.png \
    pdfviewer/images/zoom-in-32.png \
    pdfviewer/images/zoom-out-24.png \
    pdfviewer/images/zoom-out-32.png
