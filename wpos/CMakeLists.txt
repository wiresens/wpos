cmake_minimum_required(VERSION 3.15)
project(wpos VERSION 2.0.0 LANGUAGES CXX C)

# Set compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-copy -Wno-narrowing")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic-errors") # Warning Free
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") #Inibit all warning message
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

# Add subdirectories if PROJECT_IS_TOP_LEVEL
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_subdirectory(wposmodel)
    add_subdirectory(libbslxml)
    add_subdirectory(wposcore)
    add_subdirectory(libqutexr)
    add_subdirectory(xmlmanage)
    add_subdirectory(dbusprinter)
    add_subdirectory(dbusreceipt)
    # add_subdirectory(wposwidget)
    add_subdirectory(wposgui)
    add_subdirectory(wpos)
    add_subdirectory(prism)
    add_subdirectory(wposbo)
    add_subdirectory(wposrpcd)
    # add_subdirectory(wposrpclient)
    # add_subdirectory(pdfviewer)
#    add_subdirectory(qtest)
#    add_subdirectory(gtest)
endif()
