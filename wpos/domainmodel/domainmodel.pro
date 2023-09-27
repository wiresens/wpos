CONFIG -= qt

TEMPLATE = lib
DEFINES += DOMAINMODEL_LIBRARY

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/group.cpp \
    core/party.cpp \
    core/user.cpp \
    persistable.cpp \
    product/product.cpp \
    product/producttemplate.cpp

HEADERS += \
    core/group.h \
    core/party.h \
    core/user.h \
    domainmodel_global.h \
    persistable.h \
    product/product.h \
    product/producttemplate.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
