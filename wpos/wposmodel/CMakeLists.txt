project(wposmodel VERSION ${PROJECT_VERSION})

# Set compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /Qspectre")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -pedantic-errors")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas") #to enable ODB #pragma
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar") #permit definition of multichar €, ₦, £
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overflow")
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") # Inibit all warning message

set(MAIN_TARGET wposmodel)
set(TESTING_TARGET test_wposmodel)

# Adding custom odb CMake modules to CMake module search path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(ODB
    REQUIRED COMPONENTS qt boost sqlite
    OPTIONAL_COMPONENTS mysql pgsql
)

include(${ODB_USE_FILE})
list (APPEND ODB_ARGS
    -x --std=c++20   
    --profile boost/date-time/posix-time
    --default-pointer std::shared_ptr
)

find_package(Threads)
find_package(Git QUIET)

if(NOT MSVC)
    # Define all testing related content here
    enable_testing()

    # Bring in GoogleTest
    include(FetchContent)
    FetchContent_Declare(googletest
        # GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_REPOSITORY file:///home/benes/Workspace/study/googletest
        # GIT_TAG 703bd9caab50b139428cea1aaff9974ebee5742e  # release-1.10.0
        GIT_TAG main
    )

    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()
    include_directories(${googletest_SOURCE_DIR}/googletest/include)
endif()

# Include project headers

list (APPEND  ODB_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/product
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/order
)

list ( REMOVE_DUPLICATES ODB_INCLUDE_DIRS)

# Define the source files and dependencies for the executable
set(HEADER_FILES
    include/connector.h
    include/database.h
    src/core/group.h
    src/core/party.h
    src/core/user.h
    src/daterange.h
    src/default_ptr.hxx
    src/domainmodel_global.h
    src/monetary.h
    src/persistable.h
    src/product/pricelog.h
    src/product/product.h
    src/product/producttemplate.h
    src/product/uom.h
    src/version.hxx
    src/exception.h
    src/exceptions.h
    src/order/orderline.h
    src/order/order.h
    src/core/session_manager.h
)

set(SOURCE_FILES
    src/daterange.cpp
    src/core/group.cpp
    src/core/party.cpp
    src/core/user.cpp
    src/monetary.cpp
    src/persistable.cpp
    src/product/pricelog.cpp
    src/product/product.cpp
    src/product/producttemplate.cpp
    src/product/uom.cpp
    src/database.cpp
    src/exceptions.cpp
    src/order/orderline.cpp
    src/order/order.cpp
    src/core/session_manager.cpp
)

set (ODB_HEADER_FILES
    src/core/party.hxx
    src/core/user.hxx
    src/daterange.hxx
    src/monetary.hxx
    src/persistable.hxx
    src/product/pricelog.hxx
    src/product/producttemplate.hxx
    src/product/product.hxx
    src/product/uom.hxx
    src/order/orderline.hxx
    src/order/order.hxx
)

if(NOT MSVC)
    set(TESTING_SOURCE_FILES
        tests/main.cpp
        tests/uom_test.cpp
        tests/produt_test.cpp
        tests/party_test.cpp
        tests/user_test.cpp
        tests/order_test.cpp
    )
endif()

set(ODB_TAGERT_INCLUDE_DIRS)
odb_target_includes(ODB_TAGERT_INCLUDE_DIRS
    ${ODB_HEADER_FILES}
)

odb_compile(SOURCE_FILES FILES ${ODB_HEADER_FILES}
    DB sqlite
    GENERATE_QUERY
    GENERATE_SESSION
    GENERATE_SCHEMA
)

# message("ODB_INCLUDE_DIRS : ${ODB_INCLUDE_DIRS}")
# message("ODB_COMPILE_OUTPUT_DIR : ${ODB_COMPILE_OUTPUT_DIR}")
# message("ODB_TAGERT_INCLUDE_DIRS : ${ODB_TAGERT_INCLUDE_DIRS}")

# Make the project root directory the working directory when we run
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../lib)

add_library(${MAIN_TARGET}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${ODB_HEADER_FILES}
)

set_target_properties (${MAIN_TARGET} PROPERTIES
    VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_PATCH}
)

target_compile_definitions(${MAIN_TARGET}
    PRIVATE   DATABASE_SQLITE
)

target_include_directories(${MAIN_TARGET}
    PRIVATE
    ${ODB_INCLUDE_DIRS}
    ${ODB_COMPILE_OUTPUT_DIR}
    ${ODB_TAGERT_INCLUDE_DIRS}
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    ${ODB_LIBRARIES}
)

if(MSVC)
    target_link_libraries(${MAIN_TARGET} PRIVATE
        sqlite3
        pq
        mysqlclient
        crypto
        ssl
        z
    )
endif()

if(NOT MSVC)
    add_executable(${TESTING_TARGET}
        ${TESTING_SOURCE_FILES}
    )

    add_dependencies(${TESTING_TARGET} gtest)
    target_link_libraries(${TESTING_TARGET} PRIVATE
        ${MAIN_TARGET}
        ${ODB_LIBRARIES}
        gtest
        ${CMAKE_THREAD_LIBS_INIT}
    )

    target_include_directories(${TESTING_TARGET}
        PRIVATE
        ${ODB_INCLUDE_DIRS}
        ${ODB_TAGERT_INCLUDE_DIRS}
    )
endif()

# qt_generate_deploy_app_script(
#    TARGET ${MAIN_TARGET}
#    OUTPUT_SCRIPT deploy_script
#    NO_UNSUPPORTED_PLATFORM_ERROR
# )

# install(SCRIPT ${deploy_script})
