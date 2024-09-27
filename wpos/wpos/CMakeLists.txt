project(wpos VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wpos)

if(NOT MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--copy-dt-needed-entries")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(WS_SERVICE com.wiresens)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Network
    Sql
    DBus
    WebEngineWidgets
    Core5Compat
)

#QtDbus stuff : not yet robbust, because depending on hardcoded directories and
#file name. Any refactoring that changes those names will breack build process
#We should find a way to make it more robbust
#Concerned folders : dbusprinter dbusreceipt
#Concerned files and classes:
    # barcore/FileAlterationWatcher.h
    # menusystem/menus/ReceiptRecoverWidget.h
    # menusystem/menus/NumCodeWidget.h
    # LoginScreen.h

set(DBUS_XML_INTERFACE_DIR dbusprinter dbusreceipt)
set(DBUS_INTERFACE_BASE1 dbusprinter)
set(DBUS_INTERFACE_BASE2 dbusreceiptprimitive dbusreceiptmediator)

# We need to fecth the corresponding .xml files from their directories
# were their respectives target generated them.
# Which are in their target CMAKE_CURRENT_BINARY_DIR
# Since all the target have the same CMAKE_BINARY_DIR we go CMAKE_BINARY_DIR/target
# to fetch the files

set(i 0)
foreach(dirItem IN LISTS DBUS_XML_INTERFACE_DIR)
    set(DBUS_DEPENDENCY_DIR "${CMAKE_BINARY_DIR}/${dirItem}")
    math(EXPR i "(${i} + 1)")
    foreach(dbus_spec IN LISTS DBUS_INTERFACE_BASE${i})

        # Invocke qdbusxml2cpp to generate *_interface.{h,cpp} from .xml specs
        qt_add_dbus_interface(
            DBUS_XML2CPP_FILES
            "${DBUS_DEPENDENCY_DIR}/${WS_SERVICE}.${dbus_spec}.xml"
            ${dbus_spec}_interface
        )
    endforeach()
endforeach()

# This is just a hack. We had to capitalized file names according
# to class names to make qt_add_dbus_adaptor() work as it require
# parent class name as it argument.
set(DBUS_ADAPTOR_FILES
    barcore/FileAlterationWatcher.h
    menusystem/menus/ReceiptRecoverWidget.h
    menusystem/menus/PinCodeWidget.h
    Greeter.h
)

foreach( filePathUpper IN LISTS DBUS_ADAPTOR_FILES)
    string(TOLOWER ${filePathUpper} filePathLower)
    get_filename_component(fileBase ${filePathLower} NAME_WE)

    # Invoke qdbuscpp2xml to generate .xml spec files form .h
    qt6_generate_dbus_interface(
        ${filePathLower}
        "${WS_SERVICE}.${fileBase}.xml"
        OPTIONS -m  # export all Scriptable Public Slot
    )
    get_filename_component(parentClass ${filePathUpper} NAME_WE)

    # Invoke qdbusxml2cpp to generate *_adaptor.{h,cpp} from .xml spec files
    qt_add_dbus_adaptor(
        DBUS_XML2CPP_FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${WS_SERVICE}.${fileBase}.xml"
        ${filePathLower}
        ${parentClass}
        "${fileBase}_adaptor"
    )
endforeach()

include_directories(
    ../libbslxml
    ../wposcore
    ../libqutexr
    ../wposwidget
    ..
    .
)

set(HEADER_FILES
    auth/authcore.h
    barcore/cashdevice.h
    barcore/dbusreceiptquerier.h
    barcore/extracore.h
    barcore/filemanager.h
    barcore/filealterationwatcher.h
    barcore/filealterationwatcherinterface.h
    barcore/eventlogcore.h
    barcore/barcore.h
    customize/buttonsetwidget.h
    customize/menubutton.h
    customize/nkeyboard.h
    customize/npopupkeyboard.h
    customize/genericmenubutton.h
    customize/genericbuttongroup.h
    customize/genericbutton.h
    database/barcoredb.h
    database/changemoneywidgetdb.h
    database/filemanagerdb.h
    database/authcoredb.h
    database/killticketsdb.h
    database/ticketcoredb.h
    database/auxdb.h
    database/eventlogcoredb.h
    menusystem/menus/adminwidget.h
    menusystem/menus/cashwidget.h
    menusystem/menus/changemoneywidget.h
    menusystem/menus/exitactionswidget.h
    menusystem/menus/freepricewidget.h
    menusystem/menus/genericproduct.h
    menusystem/menus/invitationallocatorwidget.h
    menusystem/menus/killticketswidget.h
    menusystem/menus/pincodewidget.h
    menusystem/menus/receiptinterface.h
    menusystem/menus/receiptrecoverwidget.h
    menusystem/menus/xzwidget.h
    menusystem/menus/paymodeswidget.h
    menusystem/menus/ticketshandlingwidget.h
    printer/printerticket.h
    printer/printermanager.h
    printer/printersamsungsrp350.h
    printer/printer.h
    prodselect/product.h
    prodselect/productscreen.h
    prodselect/productscreenstack.h
    prodselect/relatedproductgroup.h
    ticket/ticketcore.h
    greeter.h
    greeterinterface.h
    main.h
    mainscreen.h
    salesscreen.h
    readconfigscreen.h
)

set(SOURCES_FILES
    auth/authcore.cpp
    barcore/barcore.cpp
    barcore/cashdevice.cpp
    barcore/dbusreceiptquerier.cpp
    barcore/filemanager.cpp
    barcore/filealterationwatcher.cpp
    barcore/eventlogcore.cpp
    barcore/extracore.cpp
    customize/buttonsetwidget.cpp
    customize/genericmenubutton.cpp
    customize/genericbuttongroup.cpp
    customize/menubutton.cpp
    customize/nkeyboard.cpp
    customize/genericbutton.cpp
    customize/npopupkeyboard.cpp
    database/changemoneywidgetdb.cpp
    database/killticketsdb.cpp
    database/ticketcoredb.cpp
    database/auxdb.cpp
    database/barcoredb.cpp
    database/filemanagerdb.cpp
    database/eventlogcoredb.cpp
    database/authcoredb.cpp
    menusystem/menus/adminwidget.cpp
    menusystem/menus/cashwidget.cpp
    menusystem/menus/changemoneywidget.cpp
    menusystem/menus/exitactionswidget.cpp
    menusystem/menus/freepricewidget.cpp
    menusystem/menus/genericproduct.cpp
    menusystem/menus/invitationallocatorwidget.cpp
    menusystem/menus/killticketswidget.cpp
    menusystem/menus/pincodewidget.cpp
    menusystem/menus/receiptrecoverwidget.cpp
    menusystem/menus/xzwidget.cpp
    printer/printermanager.cpp
    printer/printer.cpp
    printer/printerticket.cpp
    printer/printersamsungsrp350.cpp
    prodselect/product.cpp
    prodselect/relatedproductgroup.cpp
    ticket/ticketcore.cpp
    main.cpp
    mainscreen.cpp
    salesscreen.cpp
    prodselect/productscreen.cpp
    prodselect/productscreenstack.cpp
    greeter.cpp
    readconfigscreen.cpp
    menusystem/menus/paymodeswidget.cpp
    menusystem/menus/ticketshandlingwidget.cpp
)

set(UIC_FILES
    customize/nkeyboardwidget.ui
    customize/buttonsetwidget.ui
    menusystem/menus/adminwidget.ui
    menusystem/menus/cashwidget.ui
    menusystem/menus/changemoneywidget.ui
    menusystem/menus/exitactionswidget.ui
    menusystem/menus/freepricewidget.ui
    menusystem/menus/invitationallocatorwidget.ui
    menusystem/menus/killticketswidget.ui
    menusystem/menus/pincodewidget.ui
    menusystem/menus/xzwidget.ui
    menusystem/menus/paymodeswidget.ui
    saleswidget.ui
    loginwidget.ui
    readconfigwidget.ui
)

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

add_executable(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}
    ${DBUS_XML2CPP_FILES}
    ${UIC_FILES}
)

target_compile_definitions(${MAIN_TARGET}
    PRIVATE HAVE_LIBFREETYPE #QT_DISABLE_DEPRECATED_UP_TO=0x050F00
)

add_compile_definitions(QT_DISABLE_DEPRECATED_UP_TO=0x050F00)

target_include_directories(${MAIN_TARGET}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(${MAIN_TARGET} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(${MAIN_TARGET}  PRIVATE
#    wposmodel
    wposxml
    wposcore
    wposrpc
    wposgui
)

if(MSVC)
    target_link_libraries(${MAIN_TARGET}  PRIVATE
        jpeg
        libpng16
        zlib
        crypto
        libgd
        # libgdc
    )
else()
    target_link_libraries(${MAIN_TARGET}  PRIVATE
        libjpeg.a
        libpng.a
        libz.a
        libcrypt.a
        libgdc.a
        libgd.a
    )
endif()

target_link_libraries(${MAIN_TARGET} PUBLIC
    Qt::Widgets
    Qt::Network
    Qt::Sql
    Qt::DBus
    Qt::WebEngineWidgets
    Qt::Core5Compat
)

install(TARGETS ${MAIN_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
   TARGET ${MAIN_TARGET}
   OUTPUT_SCRIPT deploy_script
   NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
