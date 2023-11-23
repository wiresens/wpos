cmake_minimum_required(VERSION 3.15)
project(wposmodel VERSION ${PROJECT_VERSION})

# Set compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -pedantic-errors")
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
    --default-pointer std::shared_ptr
    --profile boost/date-time/posix-time
)

find_package(Threads)
find_package(Git QUIET)

# Define all testing related content here
enable_testing()

# Bring in GoogleTest
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 703bd9caab50b139428cea1aaff9974ebee5742e  # release-1.10.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()
include_directories(${googletest_SOURCE_DIR}/googletest/include)

# Include project headers
include_directories(
    include
    src
    src/product
    src/core
)

list (APPEND  ODB_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/product
    ${PROJECT_SOURCE_DIR}/src/core
)

list ( REMOVE_DUPLICATES ODB_INCLUDE_DIRS)

# Define the source files and dependencies for the executable
set(HEADER_FILES
    include/connector.h
    include/database.h
    include/SimpleArray.h
    include/AllocationTracker.h
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
)

set(SOURCE_FILES
#    src/SimpleArray.cpp
#    src/AllocationTracker.cpp
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
)

set(TESTING_SOURCE_FILES
    tests/main.cpp
#    tests/simpleArrayTest.cpp
#    tests/moreTests.cpp
    tests/uom_test.cpp
    tests/produt_test.cpp
    tests/party_test.cpp
)

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

message("ODB_INCLUDE_DIRS : ${ODB_INCLUDE_DIRS}")
message("ODB_COMPILE_OUTPUT_DIR : ${ODB_COMPILE_OUTPUT_DIR}")
message("ODB_TAGERT_INCLUDE_DIRS : ${ODB_TAGERT_INCLUDE_DIRS}")

# Make the project root directory the working directory when we run
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../lib)

add_library(${MAIN_TARGET} SHARED
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

target_link_libraries(${MAIN_TARGET}
    ${ODB_LIBRARIES}
)

add_executable(${TESTING_TARGET}
    ${TESTING_SOURCE_FILES}   
)

add_dependencies(${TESTING_TARGET} gtest)
target_link_libraries(${TESTING_TARGET} PRIVATE
    ${MAIN_TARGET}
    gtest
    ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(${TESTING_TARGET}
    PRIVATE
    ${ODB_INCLUDE_DIRS}
    ${ODB_TAGERT_INCLUDE_DIRS}
)
