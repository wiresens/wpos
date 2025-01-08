project(wpos VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET prism)

if(NOT MSVC)
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--copy-dt-needed-entries")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(WS_SERVICE com.wiresens)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Network
    Sql
    DBus
    WebEngineWidgets
)

qt_standard_project_setup()

include_directories(
    core/ui
    modules/stock/ui
    modules/product/ui
    modules/health/ui
    ../libbslxml
    ../wposcore
    ../wposgui
    ..
    .
)

set(HEADER_FILES
    core/ui/views/prism_main_window.h
    core/ui/views/prism_application.h

    modules/product/ui/views/product_creation_form.h
    modules/product/ui/views/product_creation_navigator.h

    modules/stock/ui/views/inventory/stock_inventory_form.h
    modules/stock/ui/views/stock_creation_navigator.h
    modules/stock/ui/views/stock_entry_form.h
    modules/stock/ui/views/stock_entry_command_widget.h
    modules/stock/ui/views/stock_item.h
    modules/stock/ui/views/stock_item_form.h
    modules/stock/ui/views/stock_item_command_widget.h
)

set(SOURCES_FILES
    core/ui/views/prism_main_window.cpp
    core/ui/views/prism_application.cpp

    main.cpp

    modules/product/ui/views/product_creation_form.cpp
    modules/product/ui/views/product_creation_navigator.cpp

    modules/stock/ui/views/inventory/stock_inventory_form.cpp
    modules/stock/ui/views/stock_creation_navigator.cpp
    modules/stock/ui/views/stock_entry_form.cpp
    modules/stock/ui/views/stock_entry_command_widget.cpp
    modules/stock/ui/views/stock_item.cpp
    modules/stock/ui/views/stock_item_form.cpp
    modules/stock/ui/views/stock_item_command_widget.cpp
)

set(UIC_FILES

    core/ui/views/stock/stock_inventory_form.ui

    modules/health/ui/views/health/consultation_book.ui
    modules/health/ui/views/health/consultation_details.ui
    modules/health/ui/views/health/consultation_report.ui
    modules/health/ui/views/health/patient_consultation.ui
    modules/health/ui/views/health/patient_identification.ui
    modules/health/ui/views/health/patient_kanban_view.ui
    modules/health/ui/views/health/patient_parameters.ui
    modules/health/ui/views/health/patient_registration.ui

    modules/product/ui/views/family_creation_form.ui
    modules/product/ui/views/grouped_product_creation_form.ui
    modules/product/ui/views/product_creation_form.ui
    modules/product/ui/views/product_creation_form_copy.ui
    modules/product/ui/views/product_creation_enventory_form.ui

    modules/stock/ui/views/inventory/session_opening_form.ui
    modules/stock/ui/views/inventory/stock_inventory_form.ui
    modules/stock/ui/views/stock_entry_form.ui
    modules/stock/ui/views/stock_item_form.ui
    modules/stock/ui/views/stock_item.ui
)

set(QRC_FILES
    core/ui/views.qrc
    modules/health/ui/views.qrc
    modules/product/ui/views.qrc
    modules/stock/ui/views.qrc
)

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../lib)

add_executable(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}
    ${DBUS_XML2CPP_FILES}
    ${UIC_FILES}
    ${QRC_FILES}
)

target_compile_definitions(${MAIN_TARGET}
    PRIVATE HAVE_LIBFREETYPE
)

target_include_directories(${MAIN_TARGET}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${MAIN_TARGET}
    PRIVATE
   wposmodel
#    wposxml
#    wposcore
#    wposrpc
    wposgui
#    xml2
#    crypt
#    jpeg
#    png
#    gdc
#    gd
#    z
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    Qt::Widgets
    Qt::Network
    Qt::Sql
    Qt::DBus
    Qt::WebEngineWidgets
)

install(TARGETS ${MAIN_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# qt_generate_deploy_app_script(
#    TARGET ${MAIN_TARGET}
#    OUTPUT_SCRIPT deploy_script
#    NO_UNSUPPORTED_PLATFORM_ERROR
# )

# install(SCRIPT ${deploy_script})
