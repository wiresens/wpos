project(wposbo VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(MAIN_TARGET wposbo)

if(NOT MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--copy-dt-needed-entries")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(WS_SERVICE com.wiresens)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Network
    Sql
    DBus
    WebEngineWidgets
    Xml
)

qt_standard_project_setup()

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
    menusystem/system/MemStickWidget.h
    productsmodule/offersmodule/ProductOfferModule.h
    productsmodule/optionsmodule/ProductOptionModule.h
    productsmodule/ProductModule.h
    usersmodule/UserModule.h
)

foreach( filePathUpper IN LISTS DBUS_ADAPTOR_FILES)
    string(TOLOWER ${filePathUpper} filePathLower)
    get_filename_component(fileBase ${filePathLower} NAME_WE)

    #Invoke qdbuscpp2xml to generate .xml spec files form .h
    #exporting all Scriptable Slot, all Scriptable Signals
    qt_generate_dbus_interface(
        ${filePathLower}
        "${WS_SERVICE}.${fileBase}.xml"
        OPTIONS -m -s
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
    ../libbslxml
    ../wposcore
    ../libqutexr
    ../wposwidget
    ..
    .
)

set(HEADER_FILES
    database/historicdb.h
    database/productoffersmoduledb.h
    database/productoptionsmoduledb.h
    database/productsmoduledb.h
    database/usersmoduledb.h
    databasemodule/databasemodule.h
    databasemodule/databasemoduleinterface.h
    mainscreen.h
    menusystem/database/databaseconfigwidget.h
    menusystem/design/addinvitationscreenwidget.h
    menusystem/design/deleteinvitationscreenwidget.h
    menusystem/design/deleteofferwidget.h
    menusystem/design/invitationscreendesignwidget.h
    menusystem/design/nticketdesignwidget.h
    menusystem/design/posdesignselectorwidget.h
    menusystem/design/productscreendesignwidget.h
    menusystem/design/shortcutbuttonwidget.h
    menusystem/design/submenuinfoframe.h
    menusystem/menus/genericscreen.h
    menusystem/menus/instantiator.h
    menusystem/menus/mainscreenmenu.h
    menusystem/menus/supportwidget.h
    menusystem/menus/techwidget.h
    menusystem/offers_options/offercreationwidget.h
    menusystem/offers_options/offerdeletionwidget.h
    menusystem/offers_options/offereditionwidget.h
    menusystem/offers_options/offerupdatewidget.h
    menusystem/offers_options/optioncreationwidget.h
    menusystem/offers_options/optiondeletionwidget.h
    menusystem/offers_options/optioneditionwidget.h
    menusystem/offers_options/optionupdatewidget.h
    menusystem/products/advancedproductdeletionwidget.h
    menusystem/products/advancedproducteditionwidget.h
    menusystem/products/advancedproductinsertionwidget.h
    menusystem/products/advancedproductupdatewidget.h
    menusystem/products/basicproductcreationwidget.h
    menusystem/products/basicproducteditionwidget.h
    menusystem/products/basicproductupdatewidget.h
    menusystem/products/productinsertionwidget.h
    menusystem/products/productoffercreationwidget.h
    menusystem/products/productoptioncreationwidget.h
    menusystem/products/productxmfileupdatewidget.h
    #menusystem/reports/nprodandofferreportwidget.h
    menusystem/reports/nprofitreportwidget.h
    menusystem/reports/persticketwidget.h
    menusystem/reports/perszwidget.h
    menusystem/reports/reportinterface.h
    menusystem/system/devicesconfigwidget.h
    #menusystem/system/fx2000configwidget.h
    menusystem/system/fx2000enrollwidget.h
    menusystem/system/memstickinterface.h
    menusystem/system/memstickwidget.h
    menusystem/users/usercreatewidget.h
    menusystem/users/usereditionwidget.h
    menusystem/users/userselectwidget.h
    menusystem/users/userupdatewidget.h
    menusystem/utils.h
    productsmodule/offersmodule/productoffermodule.h
    productsmodule/offersmodule/productoffermoduleinterface.h
    productsmodule/optionsmodule/productoptionmodule.h
    productsmodule/optionsmodule/productoptionmoduleinterface.h
    productsmodule/productmodule.h
    productsmodule/productmoduleinterface.h
    usersmodule/usermodule.h
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
    menusystem/menus/genericscreen.cpp
    menusystem/menus/instantiator.cpp
    menusystem/menus/mainscreenmenu.cpp
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
    menusystem/system/memstickwidget.cpp
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
    PRIVATE HAVE_LIBFREETYPE
)

target_include_directories(${MAIN_TARGET}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${MAIN_TARGET}  PRIVATE
#    wposmodel
    wposxml
    wposcore
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
        libgdc.a
        libgd.a
        libz.a
        # libm.a commented out because, does'nt compile with clang, but with gcc
    )
endif()

target_link_libraries(${MAIN_TARGET} PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::Network
    Qt::Sql
    Qt::DBus
    Qt::WebEngineWidgets
    Qt::Xml
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
