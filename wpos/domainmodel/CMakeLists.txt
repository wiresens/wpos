project(wposmodel LANGUAGES CXX)

set(HEADER_FILES
    daterange.h
    core/group.h
    core/party.h
    core/user.h
    domainmodel_global.h
    monetary.h
    persistable.h
    product/pricelog.h
    product/product.h
    product/producttemplate.h
    product/uom.h
    default_ptr.hxx
    version.hxx
)

set(SOURCES_FILES
    daterange.cpp
    core/group.cpp
    core/party.cpp
    core/user.cpp
    monetary.cpp
    persistable.cpp
    product/pricelog.cpp
    product/product.cpp
    product/producttemplate.cpp
    product/uom.cpp
)

set(ODB_FILES
    core/party.hxx
    core/user.hxx
    daterange.hxx
    monetary.hxx
    persistable.hxx
    product/pricelog.hxx
    product/uom.hxx
    product/producttemplate.hxx
    product/product.hxx
)

add_library(
    ${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCES_FILES}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
