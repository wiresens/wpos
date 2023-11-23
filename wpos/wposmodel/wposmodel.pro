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
    include \
    src \
    src/product \
    src/core \
    . \


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/SimpleArray.h \
    include/AllocationTracker.h \
    src/daterange.h \
    src/core/group.h \
    src/core/party.h \
    src/core/user.h \
    src/domainmodel_global.h \
    src/monetary.h \
    src/persistable.h \
    src/product/pricelog.h \
    src/product/product.h \
    src/product/producttemplate.h \
    src/product/uom.h \
    src/default_ptr.hxx \
    src/version.hxx

SOURCES += \
    src/SimpleArray.cpp \
    src/AllocationTracker.cpp \
    src/daterange.cpp \
    src/core/group.cpp \
    src/core/party.cpp \
    src/core/user.cpp \
    src/monetary.cpp \
    src/persistable.cpp \
    src/product/pricelog.cpp \
    src/product/product.cpp \
    src/product/producttemplate.cpp \
    src/product/uom.cpp

ODB_FILES += \
    src/core/party.hxx \
    src/core/user.hxx \
    src/daterange.hxx \
    src/monetary.hxx \
    src/persistable.hxx \
    src/product/pricelog.hxx \
    src/product/uom.hxx \
    src/product/producttemplate.hxx \
    src/product/product.hxx \

HEADERS += $$ODB_FILES

# ODB compiler flags.
ODB_FLAGS =\
    -x -fPIC -x --std=c++20 \
    --database pgsql \
    --sql-name-case lower \
    --profile boost/date-time/posix-time \
    --default-pointer std::shared_ptr \
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

for(dir, INCLUDEPATH){
    ODB_FLAGS += -I$$PWD/$${dir}
}

message("ODB_FLAGS = $$ODB_FLAGS")
#--output-dir $QMAKEPATH

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
