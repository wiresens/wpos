project(wposrpclient VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposrpclient)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
    ../libqutexr
    ..
)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

set(HEADER_FILES
    clientimpl.h
)

set(SOURCES_FILES
    clientimpl.cpp
    main.cpp
)

set(UIC_FILES
    client.ui
)

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

add_executable(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}
    ${UIC_FILES}
)

target_include_directories(${MAIN_TARGET}  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    wposrpc
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

# install(TARGETS ${MAIN_TARGET}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

#qt_generate_deploy_app_script( // >= Qt 6.3
#    TARGET ${MAIN_TARGET}
#    OUTPUT_SCRIPT deploy_script
#    NO_UNSUPPORTED_PLATFORM_ERROR
#)

#install(SCRIPT ${deploy_script})

