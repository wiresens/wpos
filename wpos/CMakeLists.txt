cmake_minimum_required(VERSION 3.15)
project(wpos VERSION 2.0.0 LANGUAGES CXX C)

# Set compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-copy -Wno-narrowing")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic-errors") # Warning Free
else()
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address") #Sanitizing address
    add_compile_definitions(__PRETTY_FUNCTION__ __FUNCSIG__)
endif()

set(BUILD_SHARED_LIBS Yes)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") #Inibit all warning message

# Add subdirectories if PROJECT_IS_TOP_LEVEL
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_subdirectory(wposmodel)
    add_subdirectory(libbslxml)
    add_subdirectory(wposcore)
    add_subdirectory(libqutexr)
    add_subdirectory(wposrpcd)
    add_subdirectory(wposrpclient)

    if(NOT MSVC)
        add_subdirectory(xmlmanage)
    endif()

    add_subdirectory(dbusprinter)    
    add_subdirectory(wposgui)
    add_subdirectory(prism)
    add_subdirectory(dbusreceipt)
    add_subdirectory(wpos)
    add_subdirectory(wposbo)
    # add_subdirectory(pdfviewer)
    # add_subdirectory(qtest)
    # add_subdirectory(gtest)
endif()
