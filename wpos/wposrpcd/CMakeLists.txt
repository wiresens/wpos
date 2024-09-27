project(wposrpcd VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposrpcd)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
    ../libqutexr
)

find_package(Qt6 REQUIRED COMPONENTS
    Network
    Xml
)

set(HEADER_FILES
    validationserver.h
)

set(SOURCES_FILES
    validationserver.cpp
    main.cpp
)

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

add_executable(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}
)

target_include_directories(${MAIN_TARGET}  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    wposrpc
    Qt::Core
    Qt::Network
    Qt::Xml
)

install(TARGETS ${MAIN_TARGET}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
   TARGET ${MAIN_TARGET}
   OUTPUT_SCRIPT deploy_script
   NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

