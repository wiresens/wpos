project(xmlmanage VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposxmlmanage)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Core5Compat
)

qt_standard_project_setup()

include_directories(
    ../libbslxml
    ..
    .
)

set(HEADER_FILES
    comon.h
    fileutils.h
    helpclass.h
    xmldelete.h
    xmllist.h
    xmlread.h
    xmlwrite.h
)

set(SOURCES_FILES
    fileutils.cpp
    helpclass.cpp
    main.cpp
    xmldelete.cpp
    xmllist.cpp
    xmlread.cpp
    xmlwrite.cpp
)

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

add_executable(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    wposxml
    Qt::Core5Compat
    Qt::Core
)

# install(TARGETS ${MAIN_TARGET}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

qt_generate_deploy_app_script(
   TARGET ${MAIN_TARGET}
   OUTPUT_SCRIPT deploy_script
   NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
