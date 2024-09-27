project(wposcore VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposcore)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Sql
)

find_package( LibXml2 REQUIRED)

if(MSVC)
    set(XML2_RELEASE_ROOT "C:/Users/user/Workspace/projets/libxml2/install/release")
    include_directories(
        ${XML2_RELEASE_ROOT}/include
        ${XML2_RELEASE_ROOT}/include/libxml2
    )
else()
    include_directories(
        ${LIBXML2_INCLUDE_DIR}
    )
endif()

set(HEADER_FILES
    basicdatabase.h
    config.h
    genericsignalmanager.h
    hlist.h
    productextrainfo.h
    signals.h
    utility.h
    authentication/credential.h
    authentication/crypto_hasher.h
)

set(SOURCES_FILES
    basicdatabase.cpp
    config.cpp
    genericsignalmanager.cpp
    productextrainfo.cpp
    signals.cpp
    utility.cpp
    authentication/credential.cpp
    authentication/crypto_hasher.cpp
)

# Make the project root directory the working directory when we run
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../lib)

add_library(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}

)
target_link_libraries(${MAIN_TARGET} PRIVATE
    Qt::Sql
    wposxml
)

target_include_directories(${MAIN_TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties (${MAIN_TARGET} PROPERTIES
    VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_PATCH}
)

install(TARGETS ${MAIN_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
