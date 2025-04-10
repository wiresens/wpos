/***************************************************************************
                          genericbuttongroup.cpp  -  description
                             -------------------
    begin                : vie may 7 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "genericbuttongroup.h"
#include "genericbutton.h"
#include "menubutton.h"
#include "salesscreen.h"

#include "database/auxdb.h"
#include "prodselect/product.h"

#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include <wposcore/productextrainfo.h>
#include <libbslxml/xmlconfig.h>

#include <QWidget>
#include <QFrame>
#include <QLayout>
#include <QString>
#include <QColor>
#include <QAbstractButton>

static const int BUTTON_SIZE{80};

GenericButtonGroup::GenericButtonGroup(
    ProductScreenStack *stack,
    QWidget *parent,
    const QString& name )
    :QObject(parent),
    m_product_screen_stack{stack},
    m_parent{parent}
{
    setObjectName(name);
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::REREAD_CONFIG, this);
}

void GenericButtonGroup::clear(){
    m_buttons.clear();
}

bool GenericButtonGroup::readConfigFrom(
    const QString& section,
    const QString& xmlFile)
{
    XmlConfig xml (xmlFile);
    if ( !QFile(xmlFile).exists() || !xml.wellFormed() )
        return false;
    return readConfigFrom(section, xml);
}

bool GenericButtonGroup::readConfigFrom(
    const QString& section,
    XmlConfig &xml)
{
    xml.pushDomain();
    bool section_exist  {false};

    int i{0};
    for ( i=0 ; i < xml.howManyTags("widget") ; i++){
        auto cur_section = xml.readString("widget["+QString::number(i)+"].code");
        if ( cur_section == section){
            section_exist = true;
            break;
        }
    }

    if ( ! section_exist ) {
        xml.pushDomain();
        return false;
    }

    xml.setDomain("widget["+QString::number(i)+"]");
    auto color_str = xml.readString("background_color");
    if (  !color_str.isEmpty() && QColor::isValidColor( color_str ) ){
        m_parent->setBackgroundRole(QPalette::Window);
        m_parent->setPalette(QPalette(QColor(color_str)));
    }

    QLayout *layout{};
    if ( (layout = m_parent->layout()) ) delete layout;

    xml.setDomain("layout");
    auto layout_type = xml.readString("type");
    if (layout_type.isEmpty()) layout_type = "hbox";

    int row, col;
    QGridLayout *gridLayout{};
    QHBoxLayout *hLayout{};
    QVBoxLayout *vLayout {};
    if (layout_type == "hbox"){
        hLayout = new QHBoxLayout(m_parent);
        layout = qobject_cast<QLayout *>( hLayout);
    }
    else if (layout_type == "vbox"){
        vLayout = new QVBoxLayout(m_parent);
        layout = qobject_cast<QLayout *>( vLayout );
    }
    else/* if (layout_type == "grid")*/{
        row = xml.readInt("rows");
        col = xml.readInt("cols");
        gridLayout = new QGridLayout(m_parent);
        layout = qobject_cast<QLayout *>( gridLayout);
        uint strecth_factor = 0;
        if ( xml.readString("stretch_rows") == "true"){
            for ( i=0; i < row; i++)
                gridLayout->setRowStretch(i, strecth_factor);
        }
        if (xml.readString("stretch_cols") == "true"){
            for ( i=0; i < col; i++)
                gridLayout->setColumnStretch(i, strecth_factor);
        }
    }

    auto spacing = 0;
    auto margin  = 0;
    if ( xml.howManyTags("spacing") ) spacing = xml.readInt("spacing");
    if ( xml.howManyTags("margin") )  margin = xml.readInt("margin");

    layout->setSpacing(spacing);
    layout->setContentsMargins(margin,margin,margin,margin);
    xml.releaseDomain("layout");

    for ( i=0; i < xml.howManyTags("item"); i++){
        xml.setDomain("item["+QString::number(i)+"]");
        QPushButton* button = createButton(xml, m_parent);
        if (layout_type == "hbox") hLayout->addWidget(button);
        else if (layout_type == "vbox") vLayout->addWidget(button);
        else if (layout_type == "grid"){
            row = xml.readInt("row");
            col = xml.readInt("col");
            gridLayout->addWidget( button, row, col);
        }
        xml.releaseDomain("item");
    }

    xml.popDomain();
    return true;
}

QPushButton* GenericButtonGroup::createButton(
    XmlConfig &xml,
    QWidget *parent,
    bool append)
{
    auto item_type = xml.readString("type");
    XmlConfig tmp_xml;
    tmp_xml.createElement("type", "Botoneras");
    tmp_xml.createElement("first_id", item_type);
    emit genericDataSignal(GDATASIGNAL::REREAD_CONFIG, &tmp_xml);

    QPushButton* button{};
    if (item_type == "product")
        button = createProductButton(xml, parent);
    else if (item_type == "button")
        button = createGenericButton(xml, parent);
    else if (item_type == "menu")
        button = createMenuButton(xml,parent);
    else if (item_type == "screens")
        button = createScreensButton(xml,parent);
    else if (item_type == "options")
        button = createOptionsButton(xml,parent);
    else if (item_type == "defaultscreen")
        button = createDefaultScreenButton(xml,parent);

    if (button){
        if (append) m_buttons.append(button, button->objectName());
        setSizeAndColorPolicy(button, xml);
        button->show();
    }
    return button;
}

QPushButton*
GenericButtonGroup::createProductButton(
    XmlConfig &xml,
    QWidget *parent)
{
    xml.pushDomain();
    auto product_code = xml.readString("product_code");
    auto product = new Product(product_code, cfg::xmlFileByKey(cfg::XMLKey::Bar), parent);
    product->setDefaultFontSize(9);
    product->setTextInPixmap(true);

    connect(product, &Product::productClicked, m_product_screen_stack, &ProductScreenStack::productClickedSlot);
    connect(m_product_screen_stack, &ProductScreenStack::defaultValue, product, &Product::defaultValues);
    xml.popDomain();
    return ((QPushButton *) product);
}

QPushButton*
GenericButtonGroup::createGenericButton(
    XmlConfig &xml,
    QWidget *parent)
{
    xml.pushDomain();
    auto text = xml.readString("text");
    auto name_id = xml.readString("name");
    if (name_id.isEmpty())
        name_id = text;
    text.replace(" ","\n");
    text.replace("_","\n");

    GenericButton* button {};
    button = new GenericButton(text, parent, name_id );

    bool is_toggle{false};
    if ( xml.setDomain("relationships") ){
        for (auto j=0; j < xml.howManyTags("relation"); j++){
            GenericButton::EventType event_type;
            xml.setDomain("relation["+QString::number(j)+"]");
            auto signal = xml.readString("signal");
            event_type = ( GenericButton::EventType ) xml.readInt("event");
            if ( event_type == GenericButton::ToggleOn || event_type == GenericButton::ToggleOff)
                is_toggle = true;
            button->createRelationship(signal, event_type);
            xml.releaseDomain("relation", false);
        }
        xml.releaseDomain("relationships");
    }

    if (xml.setDomain("data_relationships")){
        for (auto j=0; j < xml.howManyTags("relation"); j++){
            GenericButton::EventType num;
            QString signal,domain,value;
            xml.setDomain("relation["+QString::number(j)+"]");
            signal = xml.readString("signal");
            num = (GenericButton::EventType) xml.readInt("event");
            if ( num == GenericButton::ToggleOn || num == GenericButton::ToggleOff)
                is_toggle = true;
            button->createDataRelationship(signal, num);

            for(auto k=0; k < xml.howManyTags("content"); k++){
                xml.setDomain("content["+QString::number(k)+"]");
                domain = xml.readString("domain");
                value = xml.readString("value");
                button->addContent(domain, value, num);
                xml.releaseDomain("content",false);
            }
            xml.releaseDomain("relation",false);
        }
        xml.releaseDomain("data_relationships");
    }

    if (xml.setDomain("look_feels")){
        for (auto j=0; j < xml.howManyTags("look_feel"); j++){
            GenericButton::EventType event;
            QString text,pixmap, set_icon;
            event = ( GenericButton::EventType ) xml.readInt("look_feel["+QString::number(j)+"].event");
            if ( event == GenericButton::ToggleOn || event == GenericButton::ToggleOff )
                is_toggle = true;
            text = xml.readString("look_feel["+QString::number(j)+"].text");
            pixmap = xml.readString("look_feel["+QString::number(j)+"].pixmap");
            set_icon = xml.readString("look_feel["+QString::number(j)+"].iconset");
            if (set_icon == "true")
                button->changeAspect(event, text, pixmap, true);
            else
                button->changeAspect(event, text, pixmap, false);
        }
        xml.releaseDomain("look_feels");
    }

    auto iconset = xml.readString("iconset");
    if (!iconset.isEmpty())
        button->setIcon(QPixmap(iconset));

    auto pixmap = xml.readString("pixmap");
    if (!pixmap.isEmpty())
        button->setIcon(QPixmap(pixmap));

    auto toggle = xml.readString("toggle");
    if ( !toggle.isEmpty() && toggle == "true" )
        button->toggle();

    if (is_toggle)  button->toggle();

    xml.popDomain();
    return button;
}

QPushButton*
GenericButtonGroup::createMenuButton(
    XmlConfig &xml,
     QWidget *parent)
{
    xml.pushDomain();
    auto orientation = xml.readString("orientation");
    auto text = xml.readString("text");
    auto name_id = xml.readString("name");
    text.replace(" ","\n");
    text.replace("_","\n");

    if (name_id.isEmpty())  name_id = text;
    if (orientation.isEmpty()) orientation = "hmenu";

    MenuButton *menu_button {};
    if (orientation == "hmenu")
        menu_button = new MenuButton(Qt::Horizontal, parent, name_id);
    else/* if (orientation == "vmenu")*/
        menu_button = new MenuButton(Qt::Vertical, parent, name_id);

    for ( auto i = 0; i < xml.howManyTags("item"); i++ ){
        xml.setDomain("item["+QString::number(i)+"]");
        createButton(xml, menu_button, false);
        xml.releaseDomain("item",false);
    }

    auto iconset = xml.readString("iconset");
    if ( !iconset.isEmpty() )
        menu_button->setIcon(QPixmap(iconset));

    auto pixmap = xml.readString("pixmap");
    if ( !pixmap.isEmpty() )
        menu_button->setIcon(QPixmap(pixmap));

    xml.popDomain();
    return menu_button;
}

QPushButton*
GenericButtonGroup::createScreensButton(
    XmlConfig &xml,
    QWidget *parent)
{
    xml.pushDomain();

    auto tmp_str = xml.readString("orientation");
    if ( tmp_str.isEmpty() ) tmp_str = "hmenu";

    MenuButton *menu_button {};
    if (tmp_str == "hmenu")
        menu_button = new MenuButton(Qt::Horizontal, parent, "Screens");
    else /*if (tmp_str == "vmenu")*/
        menu_button = new MenuButton(Qt::Vertical, parent, "Screens");

    auto iconset = xml.readString("iconset");
    if (!iconset.isEmpty()) menu_button->setIcon(QPixmap(iconset));

    auto pixmap = xml.readString("pixmap");
    if (!pixmap.isEmpty()) menu_button->setIcon(QPixmap(pixmap));

    xml.popDomain();

    XmlConfig tmp_xml(cfg::xmlFileByKey(cfg::XMLKey::Bar));
    if (!QFile(cfg::xmlFileByKey(cfg::XMLKey::Bar)).exists() || !tmp_xml.setDomain("screens"))
        return qobject_cast<QPushButton*>( menu_button );

    GenericButton *generic_button {};

    for( auto i=0; i < tmp_xml.howManyTags("screen"); i++){
        tmp_str = tmp_xml.readString("screen["+QString::number(i)+"].name");

        generic_button =  new GenericButton(tmp_str, menu_button, tmp_str.toLatin1() );
        generic_button->setMinimumSize(80, 80);

        generic_button->createDataRelationship( GDATASIGNAL::MAINSTACK_SET_PAGE );
        generic_button->addContent("name", SalesScreen::PRODUCT_MENU);

        generic_button->createDataRelationship(GDATASIGNAL::PRODUCTS_SETPAGE);
        generic_button->addContent("name", tmp_str);
    }

    return qobject_cast<QPushButton*>( menu_button );
}

QPushButton*
GenericButtonGroup::createDefaultScreenButton(
    XmlConfig &xml,
    QWidget *parent)
{
    GenericButton *button {};
    XmlConfig tmp_xml(cfg::xmlFileByKey(cfg::XMLKey::Bar)) ;
    xml.pushDomain();
    if ( !QFile(cfg::xmlFileByKey(cfg::XMLKey::Bar)).exists() || !tmp_xml.setDomain("screens") )
        return qobject_cast<QPushButton *>( button );

    auto aux = tmp_xml.readString("defaultscreen");
    if ( !aux.isEmpty() )
        button =  new GenericButton("Principal", parent, "defaultscreen");
    else{
        aux = tmp_xml.readString("screen.name");
        button =  new GenericButton(aux, parent, "defaultscreen");
    }
    button->createDataRelationship(GDATASIGNAL::MAINSTACK_SET_PAGE);
    button->addContent("name", SalesScreen::PRODUCT_MENU);
    button->createDataRelationship(GDATASIGNAL::PRODUCTS_SETPAGE);
    button->addContent("name", aux);

    auto iconset = xml.readString("iconset");
    if (!iconset.isEmpty())
        button->setIcon(QPixmap(iconset));

    auto pixmap = xml.readString("pixmap");
    if (!pixmap.isEmpty())
        button->setIcon(QPixmap(pixmap));

    xml.popDomain();
    return qobject_cast<QPushButton *>( button );
}

QPushButton*
GenericButtonGroup::createOptionsButton(
    XmlConfig &xml,
    QWidget *parent)
{
    AuxDB db("AuxDBConnection", cfg::xmlFileByKey(cfg::XMLKey::Database));
    db.connect();
    QMap<QString,ProductExtraInfo> extra = db.productExtra();
    db.disConnect();

    if ( extra.isEmpty() ) return nullptr;

    MenuButton *menu{};
    MenuButton *menuValue{};
    GenericButton *genericButton{};
    if ( extra.size() == 1 ){
        ProductExtraInfo &option = extra.first();
        QString type = option.getOptionType().replace("_","\n").replace(" ","\n");
        menu = new MenuButton( Qt::Horizontal, parent, type );
        for( QString& item :  option.getValues()){
            auto value = item.replace("_", "\n").replace(" ", "\n");
            genericButton =  new GenericButton(value, menu, value);
            genericButton->createDataRelationship(GDATASIGNAL::XCORE_SET_PRODUCT_OPTION);
            genericButton->addContent("type", option.getOptionType());
            genericButton->addContent("value", item);
            genericButton->resize(BUTTON_SIZE,BUTTON_SIZE);
        }
    }
    else{
        menu = new MenuButton(Qt::Horizontal, parent, tr("Options"));
        for( auto &option : extra){
            QString type = option.getOptionType().replace("_","\n").replace(" ","\n");
            menuValue = new MenuButton(Qt::Vertical, menu, type);
            menuValue->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
            menuValue->resize(BUTTON_SIZE,BUTTON_SIZE);
            for( QString& item : option.getValues()){
                auto value = item.replace("_","\n").replace(" ","\n");
                genericButton =  new GenericButton(value,menuValue, value);
                genericButton->createDataRelationship(GDATASIGNAL::XCORE_SET_PRODUCT_OPTION);
                genericButton->addContent("type", option.getOptionType());
                genericButton->addContent("value", item);
                genericButton->resize(BUTTON_SIZE,BUTTON_SIZE);
            }
        }
    }
    auto iconFile = xml.readString("iconset");
    if (!iconFile.isEmpty())
        menu->setIcon(QPixmap(iconFile));

    auto pixmapFile = xml.readString("pixmap");
    if (!pixmapFile.isEmpty())
        menu->setIcon(QPixmap(pixmapFile));

    return ((QPushButton *) menu);
}

void GenericButtonGroup::setSizeAndColorPolicy(QPushButton *button, XmlConfig &xml){
    xml.pushDomain();
    auto xpolicy = xml.readString("size_policy.x").toLower();
    auto ypolicy = xml.readString("size_policy.y").toLower();

    QMap<QString, QSizePolicy::Policy> size_policy{
        {"fixed", QSizePolicy::Fixed},
        {"minimum", QSizePolicy::Minimum},
        {"maximum", QSizePolicy::Maximum},
        {"preferred", QSizePolicy::Preferred},
        {"expanding", QSizePolicy::Expanding},
        {"minimumexpanding", QSizePolicy::MinimumExpanding},
        {"ignored", QSizePolicy::Ignored}
    };

    if ( !xpolicy.isEmpty() && !ypolicy.isEmpty())
        button->setSizePolicy( QSizePolicy( size_policy[xpolicy], size_policy[ypolicy]));

    auto bg_color = xml.readString("background_color");
    if ( !bg_color.isEmpty() && QColor::isValidColor(bg_color)){
        button->setPalette(QPalette(QColor(bg_color)));
        button->setBackgroundRole(QPalette::Button);
    }

    //    //These are commented out so that the shortcut_buttons are resizable
    //        int width, height;
    //        if (xml.howManyTags("width")){
    //            width = xml.readInt("width");
    //            button->setMinimumWidth(width);
    //        }
    //        if (xml.howManyTags("height")){
    //            height = xml.readInt("height");
    //            button->setMinimumHeight(height);
    //        }
    //        if (xml.howManyTags("maxwidth")){
    //            width = xml.readInt("width");
    //            button->setMaximumWidth(width);
    //        }
    //        if (xml.howManyTags("maxheight")){
    //            height = xml.readInt("height");
    //            button->setMaximumHeight(height);
    //        }
    //        if (xml.howManyTags("fixedwidth")){
    //            width = xml.readInt("fixedwidth");
    //            button->setFixedWidth(width);
    //        }
    //        if (xml.howManyTags("fixedheight")){
    //            height = xml.readInt("fixedheight");
    //            button->setFixedHeight(height);
    //        }
    xml.popDomain();
}

QPushButton*
GenericButtonGroup::buttonByName(const QString& name){
    return m_buttons.find(name);
}
