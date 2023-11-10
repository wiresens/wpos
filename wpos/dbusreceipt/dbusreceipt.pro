# Subdirectory relative to the main directory of the project: dbusreceipt
# Destination is an application: dbusreceipt

QT += core dbus sql
QT -= gui
CONFIG += c++17
TARGET = dbusreceipt
CONFIG += console
CONFIG -= app_bundle
CONFIG += debug -Wall warn_on qt
#CONFIG += release warn_off qt
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++20
QMAKE_CXXFLAGS += -Wno-deprecated-copy  -Wno-narrowing

INSTALLS += etc target

target.path = /usr/bin
etc.files += etc/*
etc.path = /etc/ntpv

TARGETDEPS += \
    ../libqutexr/libqutexr.so \
    ../wposcore/libwposcore.so \
    ../libbslxml/libbslxml.so

LIBS += \
    -lqutexr \
    -lwposcore \
    -lbslxml

INCLUDEPATH += \
    ../libqutexr \
    ../wposcore \
    ../libbslxml \
    /usr/include/libxml2 \
    ..

QMAKE_LIBDIR = \
    ../libqutexr \
    ../wposcore \
    ../libbslxml

DISTFILES += \
    etc/dcopreceipt_config.xml \
    etc/dtds/dcopreceipt_config.dtd \
    AUTHORS \
    COPYING \
    README

HEADERS += \
    dbusreceiptmanager.h \
    dbusreceiptmediator.h \
    dbusreceiptprimitive.h \
    receiptchangenotifiyer.h \
    receiptclient.h \
    receiptclientmediator.h \
    receiptdb.h \
    receiptmediatorinterface.h \
    receiptprimitiveinterface.h

SOURCES += \
    dbusreceiptmanager.cpp \
    dbusreceiptmediator.cpp \
    dbusreceiptprimitive.cpp \
    receiptchangenotifiyer.cpp \
    receiptclient.cpp \
    receiptclientmediator.cpp \
    receiptdb.cpp \
    main.cpp

DBUS_ADAPTORS += com.wiresens.dbusreceiptprimitive.xml com.wiresens.dbusreceiptmediator.xml
DBUS_HEADER_FILES += dbusreceiptprimitive.h dbusreceiptmediator.h

#DBUS compiler flags.
#export all the signals and public slots
DBUS_FLAGS = -M #export all public slots
DBUS_OUT_FLAGS = -o

# Newer versions of QtCreator do builds in a separate directory. As a
# result, we need to append the source directory to DBUS files.
#
for(file, DBUS_HEADER_FILES) {
  DBUS_PWD_FILES += $$PWD/$${file}
}

DBUS_SRV += com.wiresens

qdbuscpp2xml.name = qdbuscpp2xml ${QMAKE_FILE_IN}
qdbuscpp2xml.input = DBUS_PWD_FILES
qdbuscpp2xml.output = com.wiresens.${QMAKE_FILE_BASE}.xml
qdbuscpp2xml.commands = qdbuscpp2xml $$DBUS_FLAGS ${QMAKE_FILE_IN} $$DBUS_OUT_FLAGS com.wiresens.${QMAKE_FILE_BASE}.xml
qdbuscpp2xml.CONFIG = no_link
qdbuscpp2xml.depends = $$DBUS_HEADER_FILES
qdbuscpp2xml.clean = com.wiresens.${QMAKE_FILE_BASE}.xml
QMAKE_EXTRA_COMPILERS += qdbuscpp2xml
