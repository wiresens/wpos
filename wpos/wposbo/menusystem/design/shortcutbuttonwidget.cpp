/***************************************************************************
                          shortcutbuttonwidget.cpp  -  description
                             -------------------
    begin                : Sat feb 5 2005
    copyright            : (C) 2005 by Carlos Manzanedo
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

#include "shortcutbuttonwidget.h"
#include "menusystem/utils.h"

#include "productsmodule/productmodule.h"
#include "productsmodule/offersmodule/productoffermodule.h"
#include "productsmodule/optionsmodule/productoptionmodule.h"
#include "database/productsmoduledb.h"

#include <libbslxml/xmlconfig.h>
#include <wposgui/common/dragobjects.h>
#include <wposcore/signals.h>
#include <wposcore/config.h>

#include <QTabWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLayout>
#include <QPixmap>
#include <QHeaderView>
#include <QLabel>
#include <QCheckBox>
#include <QListView>
#include <QComboBox>
#include <QApplication>
#include <QTimer>
#include <QStackedWidget>
#include <QScrollBar>
#include <QProgressBar>
#include <QApplication>
#include <QProgressBar>
#include <QFrame>
#include <QLocale>

#include <iostream>
#include <stdio.h>
using namespace std;

static const QString& PRODUCTS_DTD {"/etc/ntpv_backoffice/dtds/products_productslist.dtd"};
static const QString& OFFERS_DTD {"/etc/ntpv_backoffice/dtds/products_offerslist.dtd"};
static const QString& OPTIONS_DTD {"/etc/ntpv_backoffice/dtds/products_optionslist.dtd"};
static const QString& SHORTCUT_XML {"/etc/ntpv/ntpv_buttons.xml"};
static const QString& ICON_PATH {Files::ProductsDir};
static const QString& OFFERS_ICON_PATH {"controls:offers/"};

static const int SIZE_CONST = 50.00;
static const int TABLE_SIZE_X = 70;
static const int TABLE_SIZE_Y = 70;
static const uint TIME_OUT {10};


ShortcutButtonWidget::ShortcutButtonWidget(QWidget *parent, const QString& name):
    QWidget(parent), button_list{ new HList<ShortcutButtonData>}
{
    setupUi(this);
    setObjectName(name);

    accept_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    reload_button->setIcon(QPixmap("controls48:reload.png"));
    first_button->setIcon(QPixmap("controls32:2uparrow.png"));
    up_button->setIcon(QPixmap("controls32:up.png"));
    down_button->setIcon(QPixmap("controls32:down.png"));
    last_button->setIcon(QPixmap("controls32:2downarrow.png"));

    auto v_layout = new QVBoxLayout(buttons_frame);
    shortcut_button_table = new BslDDTable(buttons_frame,"button_table");
    v_layout->addWidget(shortcut_button_table);

    shortcut_button_table->setAcceptDrops(true);
    shortcut_button_table->setDragEnabled(true);
    shortcut_button_table->setPalette(QPalette(QColor(220,220,220)));
    shortcut_button_table->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    shortcut_button_table->setFont(QFont( "Arial", 8, QFont::Bold ));
    shortcut_button_table->setSelectionMode(QAbstractItemView::NoSelection);

    auto* layout = shortcut_button_table->layout();
    if( layout)
        layout->setContentsMargins(0,0,0,0);
    //@benes    shortcut_button_table->setLeftMargin(0);
    //@benes    shortcut_button_table->setTopMargin(0);
    //@benes    shortcut_button_table->setReadOnly(true);
    //@benes    shortcut_button_table->setRowMovingEnabled(true);
    //@benes    shortcut_button_table->setColumnMovingEnabled(true);
    shortcut_button_table->setRowCount(1);
    shortcut_button_table->setColumnCount(6);
    //@benes    (shortcut_button_table->horizontalHeader())->hide();
    //@benes    (shortcut_button_table->verticalHeader())->hide();

    for(auto i = 0; i < shortcut_button_table->columnCount(); i++)
        shortcut_button_table->setColumnWidth(i, 107);
    for(auto i = 0; i < shortcut_button_table->rowCount(); i++)
        shortcut_button_table->setRowHeight(i, 98);

    QHBoxLayout *h_layout  = new QHBoxLayout(products_frame);
    products_iconview = new BslDDIconView(products_frame, "produts_iconview");
    h_layout->addWidget(products_iconview);
    products_iconview->setFont(QFont( "Arial", 8));
    //@benes    products_iconview->setGridX(TABLE_SIZE_X);
    //@benes    products_iconview->setGridY(TABLE_SIZE_Y);

    h_layout = new QHBoxLayout(offers_frame);
    offers_iconview = new BslDDIconView(offers_frame,"offers_iconview");
    h_layout->addWidget(offers_iconview);
    offers_iconview->setFont(QFont( "Arial", 8));
    //@benes    offers_iconview->setGridX(TABLE_SIZE_X);
    //@benes    offers_iconview->setGridY(TABLE_SIZE_Y);

    h_layout = new QHBoxLayout(options_frame);
    options_iconview = new BslDDIconView(options_frame,"offers_iconview");
    h_layout->addWidget(options_iconview);
    options_iconview->setFont(QFont( "Arial", 8));
    //@benes    options_iconview->setGridX(TABLE_SIZE_X);
    //@benes    options_iconview->setGridY(TABLE_SIZE_Y);


    h_layout = new QHBoxLayout(special_button_frame);
    special_buttons_iconview = new BslDDIconView(special_button_frame,"offers_iconview");
    h_layout->addWidget(special_buttons_iconview);
    special_buttons_iconview->setFont(QFont( "Arial", 8));
    //@benes    special_buttons_iconview->setGridX(TABLE_SIZE_X);
    //@benes    special_buttons_iconview->setGridY(TABLE_SIZE_Y);


    connect(up_button, &QPushButton::released, this, &ShortcutButtonWidget::scrollUpSlot);
    connect(down_button,  &QPushButton::released, this, &ShortcutButtonWidget::scrollDownSlot);
    connect(first_button,  &QPushButton::released, this, &ShortcutButtonWidget::selectFirstSlot);
    connect(last_button,  &QPushButton::released, this, &ShortcutButtonWidget::selectLastSlot);

    connect(reload_button,  &QPushButton::released, this, &ShortcutButtonWidget::reloadSlot);
    connect(accept_button,  &QPushButton::released, this, &ShortcutButtonWidget::acceptSlot);

    connect(this,  &ShortcutButtonWidget::itemReaded, this, &ShortcutButtonWidget::itemReadedSlot);
    connect(shortcut_button_table, QOverload<int, int, const QString&>::of(&BslDDTable::textEnter), this, &ShortcutButtonWidget::draggedText);

}

ShortcutButtonWidget::~ShortcutButtonWidget(){
    delete shortcut_button_table;
    delete products_iconview;
    delete options_iconview;
    delete offers_iconview;
    delete special_buttons_iconview;
    button_list->clear();
    delete button_list;
}

void ShortcutButtonWidget::clear(){
    products_iconview->clear();
    offers_iconview->clear();
    special_buttons_iconview->clear();
    clearTable();
}

void ShortcutButtonWidget::clearTable(){

    for(auto i = 0; i < shortcut_button_table->rowCount(); i++)
        shortcut_button_table->removeRow(i);

    shortcut_button_table->setRowCount(1);
    shortcut_button_table->setColumnCount(6);

    shortcut_button_table->horizontalHeader()->setVisible(false);
    shortcut_button_table->verticalHeader()->setVisible(false);

    for(auto i = 0; i < shortcut_button_table->columnCount(); i++)
        shortcut_button_table->setColumnWidth(i, 107);
    for(auto i = 0; i < shortcut_button_table->rowCount(); i++)
        shortcut_button_table->setRowHeight(i, 98);
}

void ShortcutButtonWidget::acceptSlot(){
    writeConfig();
    accept_button->setEnabled(false);
}

void ShortcutButtonWidget::reloadSlot(){
    clear();
    startShowing();
}

void ShortcutButtonWidget::draggedText(int x, int y, const QString& text){
    int row, col,num;

    QString code;
    ShortcutButtonData *data = 0;
    ProductModule *product_mod = 0;
    ProductOfferModule *offers_module = 0;
    QListWidgetItem *item = 0;

    row = shortcut_button_table->rowAt(y);
    col = shortcut_button_table->columnAt(x);

    if((row < 0)||(col < 0)){
        cerr << "Not valid drop Drop location " << __PRETTY_FUNCTION__ << __LINE__ << endl;
        return;
    }

    data = button_list->at(col);
    if (main_tab->currentWidget() == products_page){
        item = products_iconview->currentItem();
        //@benes        code = item->key();
        code = item->text();
        if (!data){
            data = new ShortcutButtonData;
            button_list->append(data, code);
        }
        data->pixmap ="";
        data->button_type = ShortcutButtonData::Product;
        if (text == "solo"){
            data->code = "solo";
            data->text = "solo";
            data->pixmap = "";
        }
        else{
            product_mod = new ProductModule(this);
            data->state = true;
            data->code = code;
            data->text = product_mod->getProductName(code);
            data->pixmap = product_mod->getLogoFromProductCode(code);
            if (!data->pixmap.isEmpty())
                data->pixmap = ICON_PATH + data->pixmap;
            delete product_mod;
        }
    }
    else if (main_tab->currentWidget() == offers_page){
        item = offers_iconview->currentItem();
        code = item->text();
        if (!data){
            data = new ShortcutButtonData;
            button_list->append(data,code);
        }
        data->pixmap = "";
        data->text = text;
        data->name = text;
        data->type = item->text();
        data->state = true;
        data->button_type = ShortcutButtonData::Offer;
        offers_module = new ProductOfferModule(this);
        data->pixmap = offers_module->getLogo(data->type,data->name);
        if (!data->pixmap.isEmpty())
            data->pixmap = OFFERS_ICON_PATH + data->pixmap;
        delete offers_module;
    }
    else if (main_tab->currentWidget() == options_page){
        item = options_iconview->currentItem();
        code = item->text();
        if (!data){
            data = new ShortcutButtonData;
            button_list->append(data,code);
        }
        data->text = text;
        data->name = text;
        data->pixmap="";
        data->type = item->text();
        data->state = true;
        data->button_type = ShortcutButtonData::Option;
    }
    else if (main_tab->currentWidget() == special_buttons_page){
        item = special_buttons_iconview->currentItem();
        num = (item->text()).toInt();
        if (!data){
            data = new ShortcutButtonData;
            button_list->append(data,code);
        }
        data->button_type = ShortcutButtonData::SpecialButton;
        data->text = text;
        data->num = num;
        data->pixmap="";
        data->state = true;
    }

    showButtonsAtList();
    accept_button->setEnabled(true);
}

//@benes
//void ShortcutButtonWidget::selectFirstSlot(){
//    QListWidgetItem *item = 0;
//    if (main_tab->currentWidget() == products_page){
//        item = products_iconview->firstItem();
//        products_iconview->ensureItemVisible(item);
//    }
//    else if (main_tab->currentWidget() == offers_page){
//        item = offers_iconview->firstItem();
//        offers_iconview->ensureItemVisible(item);
//    }
//    else if (main_tab->currentWidget() == options_page){
//        item = options_iconview->firstItem();
//        options_iconview->ensureItemVisible(item);
//    }
//    else if (main_tab->currentWidget() == special_buttons_page){
//        item = special_buttons_iconview->firstItem();
//        special_buttons_iconview->ensureItemVisible(item);
//    }
//}

void ShortcutButtonWidget::selectFirstSlot(){
    auto page = main_tab->currentWidget();

    QList<QListWidgetItem*> selection;
    if (page == products_page){
        selection = products_iconview->selectedItems();
    }
    else if (page == offers_page){
        selection = offers_iconview->selectedItems();
    }
    else if (page == options_page){
        selection = options_iconview->selectedItems();
    }
    else if (page == special_buttons_page){
        selection = special_buttons_iconview->selectedItems();
    }
    if( !selection.isEmpty()) selection.first()->setHidden(false);
}

void ShortcutButtonWidget::scrollUpSlot(){
    if (main_tab->currentWidget() == products_page)
        products_iconview->scroll(0,-100);
    else if (main_tab->currentWidget() == offers_page)
        offers_iconview->scroll(0,-100);
    else if (main_tab->currentWidget() == options_page)
        options_iconview->scroll(0,-100);
    else if (main_tab->currentWidget() == special_buttons_page)
        special_buttons_iconview->scroll(0,-100);
}

void ShortcutButtonWidget::scrollDownSlot(){
    if (main_tab->currentWidget() == products_page)
        products_iconview->scroll(0,100);
    else if (main_tab->currentWidget() == offers_page)
        offers_iconview->scroll(0,100);
    else if (main_tab->currentWidget() == options_page)
        options_iconview->scroll(0,100);
    else if (main_tab->currentWidget() == special_buttons_page)
        special_buttons_iconview->scroll(0,100);
}

//void ShortcutButtonWidget::selectLastSlot(){
//    QListWidgetItem *item = 0;
//    if (main_tab->currentWidget() == products_page){
//        item = products_iconview->lastItem();
//        products_iconview->ensureItemVisible(item);
//    }
//    else if (main_tab->currentWidget() == offers_page){
//        item = offers_iconview->lastItem();
//        offers_iconview->ensureItemVisible(item);
//    }
//    else if (main_tab->currentWidget() == options_page){
//        item = options_iconview->lastItem();
//        options_iconview->ensureItemVisible(item);
//    }
//    else if (main_tab->currentWidget() == special_buttons_page){
//        item = special_buttons_iconview->lastItem();
//        special_buttons_iconview->ensureItemVisible(item);
//    }
//}

void ShortcutButtonWidget::selectLastSlot(){
    auto page = main_tab->currentWidget();

    QList<QListWidgetItem*> selection;
    if (page == products_page){
        selection = products_iconview->selectedItems();
    }
    else if (page == offers_page){
        selection = offers_iconview->selectedItems();
    }
    else if (page == options_page){
        selection = options_iconview->selectedItems();
    }
    else if (page == special_buttons_page){
        selection = special_buttons_iconview->selectedItems();
    }
    if( !selection.isEmpty()) selection.last()->setHidden(false);
}

void ShortcutButtonWidget::loadShortcutButtonsSlot(){
    readButtonsXml();
    showButtonsAtList();
}

void ShortcutButtonWidget::readButtonsXml(){

    ShortcutButtonData *button = 0;
    ProductModule *product_mod = 0;
    bool found {false};

    button_list->clear();
    XmlConfig xml(SHORTCUT_XML);
    if ( !xml.wellFormed() ) return;

    xml.delDomain();
    for (auto i=0; i < xml.howManyTags("widget"); i++){
        if (xml.readString("widget["+QString::number(i)+"].code") == "shortcut_buttons_frame")
        {
            found = true;
            xml.setDomain("widget["+QString::number(i)+"]");
        }
    }

    if (!found)  return;

    for ( auto i=0; i < xml.howManyTags("item"); i++){
        xml.setDomain("item["+QString::number(i)+"]");
        button = new ShortcutButtonData;
        button->state = false;
        if ( xml.readString("type") == "product" ){
            button->button_type = ShortcutButtonData::Product;
            button->code = xml.readString("product_code");
            product_mod = new ProductModule(this);
            button->text = product_mod->getProductName(button->code);
            button->pixmap = product_mod->getLogoFromProductCode(button->code);
            button->pixmap = QString(ICON_PATH) + button->pixmap;
            delete product_mod;
            if ((button->text.isEmpty())&&(button->code=="solo"))
                button->text = "solo";
        }
        else{
            button->button_type = ShortcutButtonData::ReadedFromXml;
            button->text = xml.readString("text");
            button->pixmap = xml.readString("iconset");
            if (button->pixmap.isEmpty())
                button->pixmap = xml.readString("pixmap");
        }
        xml.releaseDomain("item");
        button_list->append(button,button->text);
    }
    xml.delDomain();
}

void ShortcutButtonWidget::showButtonsAtList(){
    int count;
    count = button_list->count();
    clearTable();
    if ( count >= 6 ) count = 6;

    double size = 30.00;
    int i = 0;
    for(auto button : *button_list){
        auto item = new QTableWidgetItem(button->text);
//@benes        item->setWordWrap(true);
        if (!button->pixmap.isEmpty()){
            item->setIcon(cropedIcon(button->pixmap, size));
        }
        shortcut_button_table->setItem(0, i++, item);
    }
}

void ShortcutButtonWidget::loadProductsSlot(){
    int index, count;
    QString  icon;

    BslDDIconViewItem *item = 0;
    QString product_code;
    QString product_name;

    double size = SIZE_CONST;
    auto product_mod = new ProductModule();
    auto xml_string = product_mod->getUnitaryProducts();
    delete product_mod;

    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(PRODUCTS_DTD)){
        cerr << "The file does not conform a valid xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    xml.delDomain();
    xml.setDomain("products");
    products_iconview->clear();
    count = xml.howManyTags("product");
    //@    progress->setValue(0,count);
    progress->setValue(count);

    first_label->setText(tr("Product"));
    second_label->clear();
    for(index=0; index < count; index++){
        xml.setDomain("product[" + QString::number(index) + "]");
        product_name = xml.readString("name");
        product_code = xml.readString("code");
        emit itemReaded(index, product_name,"");
        if(xml.readString("logo").isEmpty()){
            item = new BslDDIconViewItem(products_iconview, product_name );
            item->setText(product_code);
            xml.releaseDomain("product", false);
            continue;
        }
        icon = ICON_PATH + xml.readString("logo");
        item = new BslDDIconViewItem(products_iconview, product_name, cropedIcon(icon,size));
        item->setText(product_code);
        products_iconview->insertItem(0,item);
        xml.releaseDomain("product", false);

    }

    if ( products_iconview->findItems("solo", Qt::MatchExactly).isEmpty() ){
        item = new BslDDIconViewItem(products_iconview, "solo");
        item->setText(product_code);
        products_iconview->insertItem(0,item);
    }
    xml.releaseDomain("products", true);
    (products_iconview->horizontalScrollBar())->hide();
    (products_iconview->verticalScrollBar())->hide();

}

void ShortcutButtonWidget::loadOffersSlot(){
    int count;
    double size;
    ProductOfferModule *offers_module = 0;
    QString xml_string;
    QString offer_type, offer_name,icon;


    BslDDIconViewItem *item = 0;

    offers_iconview->clear();

    offers_module = new ProductOfferModule(this);
    xml_string = offers_module->getAllOffers();
    delete offers_module;
    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OFFERS_DTD)){
        cerr << "The file does not conform a valid xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    xml.setDomain("offers");
    count = xml.howManyTags("offer");
    //@benes    progress->setProgress(0,count);
    progress->setValue(count);
    first_label->setText(tr("Type of offer"));
    second_label->setText(tr("Offer"));
    size = SIZE_CONST;
    for( auto i=0; i< count ;i++){
        offer_type = xml.readString("offer["+QString::number(i)+"].offer_type");
        offer_name = xml.readString("offer["+QString::number(i)+"].offer_name");
        icon = xml.readString("offer["+QString::number(i)+"].logo");
        item = new BslDDIconViewItem(offers_iconview,offer_name);
        item->setText(offer_type);
        if (!icon.isEmpty()){
            icon = QString(OFFERS_ICON_PATH) + icon;
            item->setIcon(cropedIcon(icon, size));
        }
        emit itemReaded(i, offer_type, offer_name);
        offers_iconview->insertItem(0,item);
    }
}

void ShortcutButtonWidget::loadOptionsSlot(){
    int i,count;
    ProductOptionModule *options_module = 0;
    QString xml_string;
    QString option_type, option_name;
    BslDDIconViewItem *item = 0;

    options_iconview->clear();

    options_module = new ProductOptionModule(this);
    xml_string = options_module->getAllOptions();
    delete options_module;

    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "Cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OPTIONS_DTD)){
        cerr << "The file does not conform a valid xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    xml.setDomain("options");
    count = xml.howManyTags("option");
    //@benes    progress->setProgress(0,count);
    progress->setValue(count);
    first_label->setText(tr("Option Type"));
    second_label->setText(tr("Option"));

    for(auto i=0; i< count ; i++){
        option_type = xml.readString("option["+QString::number(i)+"].option_type");
        option_name = xml.readString("option["+QString::number(i)+"].option_name");
        item = new BslDDIconViewItem(options_iconview, option_name);
        item->setText(option_type);
        options_iconview->insertItem(0, item);
        emit itemReaded(i, option_type, option_name);
    }
}

void ShortcutButtonWidget::loadSpecialButtons(){
    BslDDIconViewItem *item = 0;
    special_buttons_iconview->clear();

    item = new BslDDIconViewItem(special_buttons_iconview, tr("Free Price"));
    item->setText(QString::number(ShortcutButtonWidget::FreePrice));
    special_buttons_iconview->insertItem(0, item);

    item = new BslDDIconViewItem(special_buttons_iconview, tr("Generic Product"));
    item->setText(QString::number(ShortcutButtonWidget::GenericProduct));
    special_buttons_iconview->insertItem(0,item);

    item = new BslDDIconViewItem(special_buttons_iconview, tr("Open CashBox"));
    item->setText(QString::number(ShortcutButtonWidget::OpenCashBox));
    special_buttons_iconview->insertItem(0,item);

    item = new BslDDIconViewItem(special_buttons_iconview, tr("Print in Kitchen"));
    item->setText(QString::number(ShortcutButtonWidget::PrintAtKitchen));
    special_buttons_iconview->insertItem(0,item);


}

void ShortcutButtonWidget::writeConfig(){
    int i, count, j;
    XmlConfig *xml = 0;
    ShortcutButtonData *data = 0;
    bool founded = false;


    xml = new XmlConfig(SHORTCUT_XML);
    xml->delDomain();

    count = xml->howManyTags("widget");
    for (i=0;i<count;i++){
        if (xml->readString("widget["+QString::number(i)+"].code")=="shortcut_buttons_frame"){
            founded = true;
            xml->setDomain("widget["+QString::number(i)+"]");
            j = i;
        }
    }

    if (!founded){
        xml->createElementSetDomain("widget");
        xml->createElement("code","shortcut_buttons_frame");
        xml->createElementSetDomain("layout");
        xml->createElement("type","hbox");
        xml->createElement("spacing","0");
        xml->createElement("margin","0");
        xml->releaseDomain("layout");
    }

    count = button_list->count();

    if (count>=6)
        count=6;

    for (i=0;i<count;i++){
        data = button_list->at(i);
        if (!data->state)
            continue;
        if (xml->setDomain("item["+QString::number(i)+"]")){
            xml->releaseDomain("item");
            xml->deleteElement("item["+QString::number(i)+"]");
        }
        xml->createElementSetDomain("item["+QString::number(i)+"]");
        data->state = false;
        if (data->button_type == ShortcutButtonData::Product){
            xml->createElement("type","product");
            xml->createElementSetDomain("size_policy");
            xml->createElement("x","Expanding");
            xml->createElement("y","Expanding");
            xml->releaseDomain("size_policy");
            xml->createElement("fixedwidth","102");
            xml->createElement("fixedheight","98");
            xml->createElement("product_code",data->code);
        }
        else if (data->button_type == ShortcutButtonData::Offer){
            xml->createElement("type","button");
            xml->createElementSetDomain("size_policy");
            xml->createElement("x","Expanding");
            xml->createElement("y","Expanding");
            xml->releaseDomain("size_policy");
            xml->createElement("fixedwidth","102");
            xml->createElement("fixedheight","98");
            xml->createElement("text",data->text);
            if (!data->pixmap.isEmpty())
                xml->createElement("iconset",data->pixmap);
            xml->createElementSetDomain("data_relationships");
            xml->createElementSetDomain("relation");
            xml->createElement("signal", GDATASIGNAL::XCORE_SET_PRODUCT_OFFER);
            xml->createElement("event","1");
            xml->createElement("content.domain","type");
            xml->createElement("content.value",data->type);
            xml->createElement("content[1].domain","name");
            xml->createElement("content[1].value",data->name);
            xml->releaseDomain("relation");
            xml->createElementSetDomain("relation");
            xml->createElement("signal", GDATASIGNAL::PRODSELECT_COLOR_MODE );
            xml->createElement("event","1");
            xml->createElement("content.domain","mode");
            xml->createElement("content.value","special");
            xml->releaseDomain("relation");
            xml->releaseDomain("data_relationships");
        }
        else if (data->button_type == ShortcutButtonData::Option){
            xml->createElement("type","button");
            xml->createElementSetDomain("size_policy");
            xml->createElement("x","Expanding");
            xml->createElement("y","Expanding");
            xml->releaseDomain("size_policy");
            xml->createElement("fixedwidth","102");
            xml->createElement("fixedheight","98");
            xml->createElement("text",data->text);
            if (!data->pixmap.isEmpty())
                xml->createElement("iconset",data->pixmap);
            xml->createElementSetDomain("data_relationships");
            xml->createElementSetDomain("relation");
            xml->createElement("signal","GDATASIGNAL_XCORE_SET_PRODUCT_OPTION");
            xml->createElement("event","1");
            xml->createElement("content.domain","type");
            xml->createElement("content.value",data->type);
            xml->createElement("content[1].domain","value");
            xml->createElement("content[1].value",data->name);
            xml->releaseDomain("relation");
            xml->releaseDomain("data_relationships");
        }
        else if (data->button_type == ShortcutButtonData::SpecialButton){
            if (data->num == ShortcutButtonWidget::FreePrice){
                xml->createElement("type","button");
                xml->createElementSetDomain("size_policy");
                xml->createElement("x","Expanding");
                xml->createElement("y","Expanding");
                xml->releaseDomain("size_policy");
                xml->createElement("fixedwidth","102");
                xml->createElement("fixedheight","98");
                xml->createElement("text",data->text);
                xml->createElement("name",data->text);
                if (!data->pixmap.isEmpty())
                    xml->createElement("iconset",data->pixmap);
                xml->createElementSetDomain("data_relationships");
                xml->createElementSetDomain("relation");
                xml->createElement("signal", GDATASIGNAL::MAINSTACK_SETPAGE);
                xml->createElement("event","1");
                xml->createElement("content.domain","name");
                xml->createElement("content.value","FREE_PRICE_MENU");
                xml->releaseDomain("relation");
                xml->createElementSetDomain("relation");
                xml->createElement("signal", GDATASIGNAL::MAINWIDGET_SETENABLED);
                xml->createElement("event","1");
                xml->createElement("content.domain","enabled");
                xml->createElement("content.value","false");
                xml->releaseDomain("relation");
                xml->releaseDomain("data_relationships");
            }
            else if (data->num == ShortcutButtonWidget::GenericProduct){
                xml->createElement("type","button");
                xml->createElementSetDomain("size_policy");
                xml->createElement("x","Expanding");
                xml->createElement("y","Expanding");
                xml->releaseDomain("size_policy");
                xml->createElement("fixedwidth","102");
                xml->createElement("fixedheight","98");
                xml->createElement("text",data->text);
                xml->createElement("name",data->text);
                if (!data->pixmap.isEmpty())
                    xml->createElement("iconset",data->pixmap);
                xml->createElementSetDomain("data_relationships");
                xml->createElementSetDomain("relation");
                xml->createElement("signal", GDATASIGNAL::MAINSTACK_SETPAGE);
                xml->createElement("event","1");
                xml->createElement("content.domain","name");
                xml->createElement("content.value","GENERIC_PRODUCT_MENU");
                xml->releaseDomain("relation");
                xml->createElementSetDomain("relation");
                xml->createElement("signal", GDATASIGNAL::MAINWIDGET_SETENABLED);
                xml->createElement("event","1");
                xml->createElement("content.domain","enabled");
                xml->createElement("content.value","false");
                xml->releaseDomain("relation");
                xml->releaseDomain("data_relationships");
            }
            else if (data->num == ShortcutButtonWidget::OpenCashBox){
                xml->createElement("type","button");
                xml->createElementSetDomain("size_policy");
                xml->createElement("x","Expanding");
                xml->createElement("y","Expanding");
                xml->releaseDomain("size_policy");
                xml->createElement("fixedwidth","102");
                xml->createElement("fixedheight","98");
                xml->createElement("text",data->text);
                xml->createElement("name",data->text);
                if (!data->pixmap.isEmpty())
                    xml->createElement("iconset",data->pixmap);
                xml->createElementSetDomain("relationships");
                xml->createElementSetDomain("relation");
                xml->createElement("signal", GSIGNAL::OPEN_CASHBOX);
                xml->createElement("event","1");
                xml->releaseDomain("relation");
                xml->releaseDomain("relationships");
            }
            else if (data->num == ShortcutButtonWidget::PrintAtKitchen){
                xml->createElement("type","button");
                xml->createElementSetDomain("size_policy");
                xml->createElement("x","Expanding");
                xml->createElement("y","Expanding");
                xml->releaseDomain("size_policy");
                xml->createElement("fixedwidth","102");
                xml->createElement("fixedheight","98");
                xml->createElement("text",data->text);
                xml->createElement("name",data->text);
                if (!data->pixmap.isEmpty())
                    xml->createElement("iconset",data->pixmap);
                xml->createElementSetDomain("data_relationships");
                xml->createElementSetDomain("relation");
                xml->createElement("signal", GDATASIGNAL::BARCORE_PRINT_ORDER_AT_SPECIAL_PRINTER);
                xml->createElement("event","1");
                xml->createElement("content.domain","type");
                xml->createElement("content.value","kitchen");
                xml->releaseDomain("relation");
                xml->releaseDomain("data_relationships");
            }
        }
        xml->releaseDomain("item");
        if (!i){ //due to problems at libbslxml FIXME (at libbslxml)
            XmlConfig *aux_xml = 0;
            aux_xml = new XmlConfig();
            xml->delDomain();
            aux_xml->delDomain();
            aux_xml->readXmlFromString(xml->toString());
            xml->deleteElement("widget["+QString::number(j)+"].item[0]");
            xml->copy(aux_xml,"widget["+QString::number(j)+"].item[0]","widget["+QString::number(j)+"].item[1]");
            xml->setDomain("widget["+QString::number(j)+"]");
            delete aux_xml;

            //                        xml->copy(xml,"item[0]","item[1]");
            //                        xml->deleteElement("item[0]");

            cerr << "cambio realizado" << endl;
        }
    }
    xml->save();
    xml->debug();
    delete xml;


}

void ShortcutButtonWidget::showEvent(QShowEvent *e){
    if (first_show)
        first_show = false;
    else{
        stack->setCurrentWidget(reading_page);
        QTimer::singleShot(TIME_OUT, this, &ShortcutButtonWidget::startShowing);
    }
    QWidget::showEvent(e);
}

void ShortcutButtonWidget::startShowing(){
    stack->setCurrentWidget(reading_page);

    main_tab->setCurrentWidget(products_page);
    loadProductsSlot();
    loadOffersSlot();
    loadOptionsSlot();
    loadSpecialButtons();
    loadShortcutButtonsSlot();

    stack->setCurrentWidget(main_page);
}

void ShortcutButtonWidget::itemReadedSlot(
        int num,
        const QString& text1,
        const QString& text2)
{
    progress->setValue(num);
    a_label->setText(text1);
    b_label->setText(text2);
    qApp->processEvents();
}


