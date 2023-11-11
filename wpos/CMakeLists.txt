cmake_minimum_required(VERSION 3.15)
project(wpos VERSION 2.0.0 LANGUAGES CXX C)

# Set compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-copy  -Wno-narrowing")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic-errors") // Warning Free
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") #Inibit all warning message

#find_package(Qt5 REQUIRED COMPONENTS Core)
#qt_standard_project_setup() // >= Qt6.3

#add_subdirectory(domainmodel)
add_subdirectory(wposmodel)
add_subdirectory(libbslxml)
add_subdirectory(wposcore)
add_subdirectory(libqutexr)
add_subdirectory(xmlmanage)
add_subdirectory(dbusprinter)
add_subdirectory(dbusreceipt)
add_subdirectory(wposwidget)
add_subdirectory(wpos)
add_subdirectory(wposbo)
#add_subdirectory(qtest)
