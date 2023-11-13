cmake_minimum_required(VERSION 3.15)
project(wposmodel)

set(MAIN_TARGET wposmodel)
set(TESTING_TARGET testing)

find_package(Threads)
find_package(Git QUIET)

# Set compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -pedantic-errors")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") # Inibit all warning message

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
    ./include
    ./product
    ./core
    .
    ..
)

# Define the source files and dependencies for the executable
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

set(SOURCE_FILES
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

set (ODB_HEADER_FILES
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

set(TESTING_SOURCE_FILES
    tests/main.cpp
    tests/simpleArrayTest.cpp
    tests/moreTests.cpp
)

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_library(${MAIN_TARGET}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${ODB_HEADER_FILES}
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
