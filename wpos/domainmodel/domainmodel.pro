CONFIG  -= qt
CONFIG  += c++20

TARGET   = wposmodel
TEMPLATE = lib
#VERSION  = 0.1  with no version, qmake automatically assign version 1.0.0
DEFINES += DOMAINMODEL_LIBRARY

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Suppress unknown pragmas GCC warnings needed by odb compiler
QMAKE_CXXFLAGS_WARN_ON += -Wall -pedantic -Wno-unknown-pragmas -Wno-deprecated-declarations
#QMAKE_CXXFLAGS_WARN_ON += -Werror -pedantic-errors

INCLUDEPATH += \
    ./product \
    ./core \
    . \
    ..\

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    daterange.cpp \
    core/group.cpp \
    core/party.cpp \
    core/user.cpp \
    monetary.cpp \
    persistable.cpp \
    product/pricelog.cpp \
    product/product.cpp \
    product/producttemplate.cpp \
    product/uom.cpp

HEADERS += \
    daterange.h \
    core/group.h \
    core/party.h \
    core/user.h \
    domainmodel_global.h \
    monetary.h \
    persistable.h \
    product/pricelog.h \
    product/product.h \
    product/producttemplate.h \
    product/uom.h \
    default_ptr.hxx \
    version.hxx

ODB_FILES += \
    core/party.hxx \
    core/user.hxx \
    daterange.hxx \
    monetary.hxx \
    persistable.hxx \
    product/pricelog.hxx \
    product/uom.hxx \
    product/producttemplate.hxx \
    product/product.hxx \

HEADERS += $$ODB_FILES

# ODB compiler flags.
ODB_FLAGS =\
    -x -fPIC -x --std=c++20 \
    --database pgsql \
    --sql-name-case lower \
    --profile boost/date-time/posix-time \
    --default-pointer std::shared_ptr \
#    --hxx-prologue-file "../default_ptr.hxx" \
    --generate-query \
    --generate-session \
    --generate-schema

# Link to the ODB runtime libraries.
#LIBS += -lodb-pgsql
#LIBS += -lodb-qt
#LIBS += -lodb

# ODB compilation rules. Normally you don't need to change anything here.
#

# Add the Qt headers directory to the ODB include directory list.
ODB_FLAGS += -I$$[QT_INSTALL_HEADERS]

# Newer versions of QtCreator do builds in a separate directory. As a
# result, we need to append the source directory to ODB files.
for(file, ODB_FILES) {
  ODB_PWD_FILES += $$PWD/$${file}
}

#for(file, ODB_FILES) {
#  ODB_PWD_FILES += $${file}
#}

odb.name                = odb ${QMAKE_FILE_IN}
odb.input               = ODB_PWD_FILES
odb.output              = ${QMAKE_FILE_BASE}-odb.cxx
odb.commands            = odb $$ODB_FLAGS ${QMAKE_FILE_IN}
odb.depends             = $$ODB_PWD_FILES
odb.variable_out        = SOURCES
odb.clean               = ${QMAKE_FILE_BASE}-odb.cxx ${QMAKE_FILE_BASE}-odb.hxx ${QMAKE_FILE_BASE}-odb.ixx ${QMAKE_FILE_BASE}.sql ${QMAKE_FILE_BASE}.xml
QMAKE_EXTRA_COMPILERS   += odb

odbh.name               = odb ${QMAKE_FILE_IN}
odbh.input              = ODB_PWD_FILES
odbh.output             = ${QMAKE_FILE_BASE}-odb.hxx
odbh.commands           = @true
odbh.CONFIG             = no_link
odbh.depends            = ${QMAKE_FILE_BASE}-odb.cxx
QMAKE_EXTRA_COMPILERS   += odbh

odbi.name               = odb ${QMAKE_FILE_IN}
odbi.input              = ODB_PWD_FILES
odbi.output             = ${QMAKE_FILE_BASE}-odb.ixx
odbi.commands           = @true
odbi.CONFIG             = no_link
odbi.depends            = ${QMAKE_FILE_BASE}-odb.cxx
QMAKE_EXTRA_COMPILERS   += odbi
