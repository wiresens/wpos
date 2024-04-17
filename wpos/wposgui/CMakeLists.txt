project(wposgui VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET   wposgui)  #wposwidget.so.${VERSION}
set(TESTING_TARGET test_wposgui)
set(EXEC_TARGET   wposviewer)
set(PLUGIN_TARGET wposviewer)  #wposviewer.so

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(WS_SERVICE com.wiresens)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Core5Compat
    Widgets
    DBus
    Pdf
    PdfWidgets
)

qt_standard_project_setup()

# We need to fecth the corresponding .xml files from their directories
# were their respectives target generated them.
# Which are in their target CMAKE_CURRENT_BINARY_DIR
# Since all the target have the same CMAKE_BINARY_DIR we go CMAKE_BINARY_DIR/target
# to fetch the files

set(DBUS_HEADER_FILE dbusprinter)
set(DBUS_DEPENDENCY_DIR "${CMAKE_BINARY_DIR}/dbusprinter")

# Invocke qdbusxml2cpp to generate *_interface.{h,cpp} from .xml specs
qt_add_dbus_interface(
    DBUS_XML2CPP_FILES
    "${DBUS_DEPENDENCY_DIR}/${WS_SERVICE}.${DBUS_HEADER_FILE}.xml"
    ${DBUS_HEADER_FILE}_interface
)

# find_package(Threads)
# find_package(Git QUIET)

# # Define all testing related content here
# enable_testing()

# # Bring in GoogleTest
# include(FetchContent)
# FetchContent_Declare(googletest
#     GIT_REPOSITORY https://github.com/google/googletest.git
#     GIT_TAG 703bd9caab50b139428cea1aaff9974ebee5742e  # release-1.10.0
# )

# FetchContent_GetProperties(googletest)
# if(NOT googletest_POPULATED)
#     FetchContent_Populate(googletest)
#     add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
# endif()
# include_directories(${googletest_SOURCE_DIR}/googletest/include)

include_directories(
    ../wposmodel/include
    ../wposmodel/src
    ../wposmodel/src/product
    ../wposmodel/src/core
    ../wposmodel/src/order
    ..
)

list (APPEND  ODB_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/../wposmodel/include
    ${PROJECT_SOURCE_DIR}/../wposmodel/src
    ${PROJECT_SOURCE_DIR}/../wposmodel/src/product
    ${PROJECT_SOURCE_DIR}/../wposmodel/src/core
    ${PROJECT_SOURCE_DIR}/../wposmodel/src/order
)

list ( REMOVE_DUPLICATES ODB_INCLUDE_DIRS)

set(HEADER_FILES
    application/basic_application.h
    authentication/credential_manager.h
    authentication/greeter_dialog.h
    authentication/password_change_dialog.h

    common/dragobjects.h
    common/effects.h
    common/global.h
    #common/iwidgetconfig.h
    common/iwidgetconfighelper.h
    common/poslabel.h
    common/toolkit.h
    common/treeview.h

    keyboard/floatkeyboard.h
    keyboard/numkeyboard.h
    keyboard/numpad.h
    keyboard/popupkeyboard.h #not used, investigate deletion
    keyboard/spkeyboard.h    #not used

    menu/abstract_menu_tree.h

    order/ordercontentview.h
    order/ordercontrols.h
    order/orderfooterview.h
    order/orderheaderview.h
    #order/ordermanagerconfig.h
    order/orderpickerview.h
    order/orderview.h

    # viewer/page_selector.h
    viewer/pdf_viewer.h
    viewer/zoom_selector.h

    views/page_navigator.h
    views/catalog_browser.h
    views/content_lister_navigator.h
    views/wabstractcatalogmanager.h
    views/wabstractmodeview.h
    visualproduct.h

    widget/caption_widget.h
    widget/command_widget.h
    widget/content_lister.h
    widget/context_menu_bar.h
    widget/date_edit.h
    widget/header_command.h
    widget/hpanel.h
    widget/listing_form.h
    widget/period_edit.h
    widget/period_specifiyer.h
    widget/menu_bar_widget.h
    widget/mode_selector.h
    widget/navigation_buttons.h
    widget/tool_bar.h

    windows/application_display.h
    windows/application_menu.h
    windows/basic_main_window.h
    windows/window_factory.h
    windows/main_window.h
    windows/menu_item_widget.h
    windows/menupage.h
    windows/menusmanager.h
    windows/menustack.h
)

set(SOURCES_FILES
    application/basic_application.cpp
    authentication/credential_manager.cpp
    authentication/greeter_dialog.cpp
    authentication/password_change_dialog.cpp

    common/dragobjects.cpp
    common/effects.cpp
    common/global.cpp
    #common/iwidgetconfig.cpp
    common/iwidgetconfighelper.cpp
    common/poslabel.cpp
    common/toolkit.cpp
    common/treeview.cpp

    keyboard/floatkeyboard.cpp
    keyboard/numkeyboard.cpp
    keyboard/numpad.cpp
    keyboard/popupkeyboard.cpp #not used
    keyboard/spkeyboard.cpp    #not used

    menu/abstract_menu_tree.cpp

    order/ordercontentview.cpp
    order/ordercontrols.cpp
    order/orderfooterview.cpp
    order/orderheaderview.cpp
    #order/ordermanagerconfig.cpp
    order/orderpickerview.cpp
    order/orderview.cpp

    viewer/main.cpp
    # viewer/page_selector.cpp
    viewer/pdf_viewer.cpp
    viewer/zoom_selector.cpp

    views/page_navigator.cpp
    views/catalog_browser.cpp
    views/content_lister_navigator.cpp
    views/wabstractcatalogmanager.cpp
    views/wabstractmodeview.cpp


    widget/caption_widget.cpp
    widget/command_widget.cpp
    widget/content_lister.cpp
    widget/context_menu_bar.cpp
    widget/date_edit.cpp
    widget/listing_form.cpp
    widget/header_command.cpp
    widget/hpanel.cpp
    widget/menu_bar_widget.cpp
    widget/mode_selector.cpp
    widget/navigation_buttons.cpp
    widget/period_edit.cpp
    widget/period_specifiyer.cpp
    widget/tool_bar.cpp

    windows/application_display.cpp
    windows/application_menu.cpp
    windows/basic_main_window.cpp
    windows/window_factory.cpp
    windows/main_window.cpp
    windows/menu_item_widget.cpp
    windows/menupage.cpp
    windows/menusmanager.cpp
    windows/menustack.cpp
)

set(TESTING_SOURCE_FILES
    # tests/main.cpp
#    tests/uom_test.cpp
#    tests/produt_test.cpp
#    tests/party_test.cpp
#    tests/user_test.cpp
#    tests/greeterdialog_test.cpp
)

set(UIC_FILES
    authentication/authenticator.ui
    authentication/authenticator_dialog.ui
    authentication/greeter_dialog.ui
    authentication/password_change_dialog.ui

    cash/cashdesk_closure.ui

    keyboard/keyboardwidget.ui
    keyboard/loungewidget.ui
    keyboard/numpad.ui
    keyboard/spkeyboardwidget.ui

    menu/abstract_menu_tree.ui

    viewer/pdf_viewer.ui

    session/session_opening_inventory_form.ui

    stock/advanced_sales_dialog.ui
    stock/family_creation_form.ui
    stock/grouped_product_creation_form.ui
    stock/inventorydialog.ui
    stock/stock_periodic_move_recap.ui
    stock/product_creation_form.ui

    stock/reporting_dialog.ui
    stock/sales_dialog.ui
    stock/session_manager.ui

    stock/stock_move_form.ui
    stock/stock_move_validation_form_.ui
    stock/stock_request_form.ui
    stock/tab_dialog.ui

    views/basic_view.ui
    views/catalog_browser.ui
    views/page_navigator.ui

    widget/caption_widget.ui
    widget/content_lister.ui
    widget/context_menu_bar.ui
    widget/date_edit.ui
    widget/listing_form.ui  #widget/listing_form.ui
    widget/menu_bar_widget.ui
    widget/navigation_buttons.ui
    widget/hpanel.ui
    widget/catalog_navigator.ui
    widget/command_widget.ui
    widget/header_command.ui
    widget/mode_selector.ui
    widget/period_specifiyer.ui
    widget/period_edit.ui
    widget/tool_bar.ui

    windows/application_display.ui
    windows/application_menu.ui
    windows/basic_main_window.ui
    windows/docked_main_window.ui
    windows/main_window.ui
    windows/menusmanager.ui
    windows/stacked_main_window.ui
)

set(QRC_FILES
    viewer/pdf_viewer.qrc
    wposgui.qrc
)

# Make the project root directory the working directory when we run
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../lib)

#Consider adding target to build pdfviewer as a librarypluging and a standalone executable
add_library(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}
    ${DBUS_XML2CPP_FILES}
    ${UIC_FILES}
    ${QRC_FILES}
)

target_include_directories(${MAIN_TARGET}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
    #include-search paths as usage-requirements for consumers
    INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/../wposmodel_autogen/include
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    wposxml wposcore
)

add_dependencies(${MAIN_TARGET} wposprinter)

target_link_libraries(${MAIN_TARGET} PRIVATE
    Qt::Widgets
    Qt::DBus
    Qt::Pdf
    Qt::PdfWidgets
    Qt::Core5Compat
    wposmodel
)

# add_executable(${TESTING_TARGET}
#     ${TESTING_SOURCE_FILES}
# )

# add_dependencies(${TESTING_TARGET} gtest)
# target_link_libraries(${TESTING_TARGET} PRIVATE
#     ${MAIN_TARGET}
#     Qt::Widgets
#     gtest
#     wposmodel
#     ${CMAKE_THREAD_LIBS_INIT}
# )

# target_include_directories(${TESTING_TARGET}
#     PRIVATE
#     ${ODB_INCLUDE_DIRS}
#     ${ODB_TAGERT_INCLUDE_DIRS}
# )

set_target_properties (${MAIN_TARGET} PROPERTIES
    VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_PATCH}
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
