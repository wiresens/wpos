project(wposbo VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposbo)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(WS_SERVICE com.wiresens)

find_package(Qt5 REQUIRED COMPONENTS
    Widgets
    Network
    Sql
    DBus
    WebEngineWidgets
    Xml
)

# This is just a hack. We had to capitalized file names according
# to class names to make qt_add_dbus_adaptor() work as it require
# parent class name as it argument.
set(DBUS_ADAPTOR_FILES
    databasemodule/DatabaseModule.h
    menusystem/offers_options/OptionCreationWidget.h
    menusystem/offers_options/OptionDeletionWidget.h
    menusystem/offers_options/OptionUpdateWidget.h
    menusystem/reports/NProfitReportWidget.h
#    menusystem/system/FX2000ConfigWidget.h
#    menusystem/system/MemStickWidget.h
    productsmodule/offersmodule/ProductOfferModule.h
    productsmodule/optionsmodule/ProductOptionModule.h
    productsmodule/ProductModule.h
    usersmodule/UserModule.h
)

foreach( filePathUpper IN LISTS DBUS_ADAPTOR_FILES)
    string(TOLOWER ${filePathUpper} filePathLower)
    get_filename_component(fileBase ${filePathLower} NAME_WE)

    #Invoke qdbuscpp2xml to generate .xml spec files form .h
    qt5_generate_dbus_interface(
        ${filePathLower}
        "${WS_SERVICE}.${fileBase}.xml"
        OPTIONS -M -s  #export all Public Slot, all Scriptable Signals
    )

    message("filePathLower  : ${filePathLower}")
    message("fileBase       : ${fileBase}  ")
    get_filename_component(parentClass ${filePathUpper} NAME_WE)

    #Invoke qdbusxml2cpp to generate *_adaptor.{h,cpp} from .xml spec files
    qt_add_dbus_adaptor(
        DBUS_XML2CPP_FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${WS_SERVICE}.${fileBase}.xml"
        ${filePathLower}
        ${parentClass}
        "${fileBase}_adaptor"
    )
    message("parentClass    : ${parentClass}")
endforeach()

include_directories(
    /usr/include
    /usr/include/libxml2
    /usr/include/gdchart
    ../libbslxml
    ../wposcore
    ../libqutexr
    ../wposwidget
    ..
    .
)

set(HEADER_FILES
    database/productsmoduledb.h
    database/usersmoduledb.h
    database/productoptionsmoduledb.h
    database/productoffersmoduledb.h
    database/historicdb.h
    databasemodule/databasemoduleinterface.h
    databasemodule/databasemodule.h
    mainscreen.h
    menusystem/database/databaseconfigwidget.h
    menusystem/design/addinvitationscreenwidget.h
    menusystem/design/deleteinvitationscreenwidget.h
    menusystem/design/deleteofferwidget.h
    menusystem/design/invitationscreendesignwidget.h
    menusystem/design/posdesignselectorwidget.h
    menusystem/design/productscreendesignwidget.h
    menusystem/design/submenuinfoframe.h
    menusystem/menus/genericscreenwidget.h
    menusystem/menus/instantiator.h
    menusystem/menus/mainscreenwidget.h
    menusystem/utils.h
    menusystem/design/shortcutbuttonwidget.h
    menusystem/design/nticketdesignwidget.h
    menusystem/products/productinsertionwidget.h
    menusystem/products/advancedproductinsertionwidget.h
    menusystem/products/advancedproducteditionwidget.h
    menusystem/products/advancedproductdeletionwidget.h
    menusystem/products/advancedproductupdatewidget.h
    menusystem/products/productxmfileupdatewidget.h
    menusystem/products/productoffercreationwidget.h
    menusystem/products/productoptioncreationwidget.h
    menusystem/products/basicproductcreationwidget.h
    menusystem/products/basicproductupdatewidget.h
    menusystem/products/basicproducteditionwidget.h
    menusystem/offers_options/offereditionwidget.h
    menusystem/offers_options/offercreationwidget.h
    menusystem/offers_options/offerupdatewidget.h
    menusystem/offers_options/offerdeletionwidget.h
    menusystem/offers_options/optioneditionwidget.h
    menusystem/offers_options/optioncreationwidget.h
    menusystem/offers_options/optionupdatewidget.h
    menusystem/offers_options/optiondeletionwidget.h
    productsmodule/optionsmodule/productoptionmoduleinterface.h
    productsmodule/optionsmodule/productoptionmodule.h
    productsmodule/productmoduleinterface.h
    productsmodule/offersmodule/productoffermoduleinterface.h
    productsmodule/offersmodule/productoffermodule.h
    productsmodule/productmodule.h
    usersmodule/usermodule.h
    menusystem/menus/supportwidget.h
    menusystem/menus/techwidget.h
    menusystem/reports/perszwidget.h
    menusystem/reports/persticketwidget.h
    menusystem/reports/nprofitreportwidget.h
    menusystem/reports/reportinterface.h
    #menusystem/reports/nprodandofferreportwidget.h
    #menusystem/system/fx2000configwidget.h
    #menusystem/system/memstickwidget.h
    menusystem/system/fx2000enrollwidget.h
    menusystem/system/memstickinterface.h
    menusystem/system/devicesconfigwidget.h
    menusystem/users/usereditionwidget.h
    menusystem/users/usercreatewidget.h
    menusystem/users/userupdatewidget.h
    menusystem/users/userselectwidget.h
    usersmodule/usermoduleinterface.h
)

set(SOURCES_FILES
    database/productoffersmoduledb.cpp
    database/productsmoduledb.cpp
    database/usersmoduledb.cpp
    database/productoptionsmoduledb.cpp
    database/historicdb.cpp
    databasemodule/databasemodule.cpp
    mainscreen.cpp
    menusystem/database/databaseconfigwidget.cpp
    menusystem/design/addinvitationscreenwidget.cpp
    menusystem/design/deleteinvitationscreenwidget.cpp
    menusystem/design/deleteofferwidget.cpp
    menusystem/design/invitationscreendesignwidget.cpp
    menusystem/design/posdesignselectorwidget.cpp
    menusystem/design/productscreendesignwidget.cpp
    menusystem/design/submenuinfoframe.cpp
    menusystem/menus/genericscreenwidget.cpp
    menusystem/menus/instantiator.cpp
    menusystem/menus/mainscreenwidget.cpp
    menusystem/utils.cpp
    menusystem/design/shortcutbuttonwidget.cpp
    menusystem/design/nticketdesignwidget.cpp
    menusystem/products/productinsertionwidget.cpp
    menusystem/products/advancedproductinsertionwidget.cpp
    menusystem/products/advancedproducteditionwidget.cpp
    menusystem/products/advancedproductdeletionwidget.cpp
    menusystem/products/advancedproductupdatewidget.cpp
    menusystem/products/productxmfileupdatewidget.cpp
    menusystem/products/productoffercreationwidget.cpp
    menusystem/products/productoptioncreationwidget.cpp
    menusystem/products/basicproductcreationwidget.cpp
    menusystem/products/basicproductupdatewidget.cpp
    menusystem/products/basicproducteditionwidget.cpp
    menusystem/offers_options/offereditionwidget.cpp
    menusystem/offers_options/offercreationwidget.cpp
    menusystem/offers_options/offerupdatewidget.cpp
    menusystem/offers_options/offerdeletionwidget.cpp
    menusystem/offers_options/optioneditionwidget.cpp
    menusystem/offers_options/optioncreationwidget.cpp
    menusystem/offers_options/optionupdatewidget.cpp
    menusystem/offers_options/optiondeletionwidget.cpp
    productsmodule/productmodule.cpp
    productsmodule/optionsmodule/productoptionmodule.cpp
    productsmodule/offersmodule/productoffermodule.cpp
    usersmodule/usermodule.cpp
    main.cpp
    menusystem/menus/supportwidget.cpp
    menusystem/menus/techwidget.cpp
    menusystem/reports/perszwidget.cpp
    menusystem/reports/persticketwidget.cpp
    menusystem/reports/nprofitreportwidget.cpp
    #menusystem/reports/nprodandofferreportwidget.cpp
    #menusystem/system/fx2000configwidget.cpp
    #menusystem/system/memstickwidget.cpp
    menusystem/system/fx2000enrollwidget.cpp
    menusystem/system/devicesconfigwidget.cpp
    menusystem/users/usereditionwidget.cpp
    menusystem/users/usercreatewidget.cpp
    menusystem/users/userupdatewidget.cpp
    menusystem/users/userselectwidget.cpp
)

set(UIC_FILES
    menusystem/database/ddbbwidgetbase.ui
    menusystem/design/deleteofferproductinterface.ui
    menusystem/design/deletescreeninvitationsinterface.ui
    menusystem/design/newscreeninvitationsinterface.ui
    menusystem/design/posdesignselectorinterface.ui
    menusystem/design/screeninvitationsdesigninterface.ui
    menusystem/design/screenproductsdesigninterface.ui
    menusystem/design/shortcutbuttoninterface.ui
    menusystem/design/ticketdesigninterface.ui
    menusystem/menus/supportwidgetbase.ui
    menusystem/menus/techwidgetbase.ui
    menusystem/offers_options/offerdeletionwidgetbase.ui
    menusystem/offers_options/offereditionwidgetbase.ui
    menusystem/offers_options/optioneditionwidgetbase.ui
    menusystem/products/advancedproducteditionwidgetbase.ui
    menusystem/products/basicproductcreationwidgetbase.ui
    menusystem/products/basicproducteditionwidgetbase.ui
    menusystem/products/productinsertionwidgetbase.ui
    menusystem/products/productoffercreationwidgetbase.ui
    menusystem/products/productoptioncreationwidgetbase.ui
    menusystem/products/productxmfileupdatewidgetbase.ui
    menusystem/reports/persticketswidgetbase.ui
    menusystem/reports/perszwidgetbase.ui
    menusystem/reports/prodandofferreportwidgetbase.ui
    menusystem/reports/profitreportwidgetbase.ui
    menusystem/system/devicesconfigwidgetbase.ui
    menusystem/system/fx2000configwidgetbase.ui
    menusystem/system/fx2000enrollwidgetbase.ui
    menusystem/system/memstickwidgetbase.ui
    menusystem/users/usercreatewidgetbase.ui
    menusystem/users/userselectwidgetbase.ui
)

add_executable(
    ${MAIN_TARGET}
    ${HEADER_FILES}
    ${SOURCES_FILES}
    ${DBUS_XML2CPP_FILES}
    ${UIC_FILES}
)

target_compile_definitions(${MAIN_TARGET}
    PRIVATE HAVE_LIBFREETYPE
)

target_include_directories(${MAIN_TARGET}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${MAIN_TARGET} PRIVATE
#    wposmodel
    wposxml
    wposcore
    wposwidget
    xml2
    jpeg
    png
    gdc
    gd
    z
    m
)

target_link_libraries(${MAIN_TARGET} PRIVATE
    Qt5::Widgets
    Qt5::Network
    Qt5::Sql
    Qt5::DBus
    Qt5::WebEngineWidgets
    Qt5::Xml
)

install(TARGETS ${MAIN_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
