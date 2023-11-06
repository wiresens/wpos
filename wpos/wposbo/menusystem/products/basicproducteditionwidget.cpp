/***************************************************************************
                          bslselectproductwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "basicproducteditionwidget.h"
#include "basicproductupdatewidget.h"
#include "menusystem/utils.h"

#include "productsmodule/productmodule.h"
#include "database/productsmoduledb.h"

#include <wposcore/config.h>
#include <libbslxml/xmlconfig.h>

#include <QListView>
#include <QPixmap>
#include <QMatrix>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>

#include <iostream>
using namespace std;

static const QString& PRODUCTS_DTD = "/etc/ntpv_backoffice/dtds/products_productslist.dtd";
static const QString& SELECT_PRODUCTS_LIST_DTD = "/etc/ntpv_backoffice/dtds/products_productslist.dtd";
static const QString ICON_PATH = Files::ProductsDir;

static const QString& ORDER_ARTICLES_XML = "/etc/ntpv_backoffice/xml_order_articles.xml";
static const QString& PRODUCTS_XML = "/etc/ntpv/bar.xml";

static const QColor DEFAULT_COLOR = QColor(238, 238, 230);
static const double ICON_SIZE = 40.00;

BasicProductEditionWidget::BasicProductEditionWidget(
        ProductModule *_product_mod,
        int _action,
        QWidget *parent,
        const QString& name):
    QWidget(parent),
    product_mod{_product_mod},
    action{_action}
{
    setupUi(this);
    setObjectName(name);
    //          setBackgroundColor(COLOR_DEFAULT);
    select_stack->setPalette(QPalette(DEFAULT_COLOR));
    //@benes    products_list->setAllColumnsShowFocus(true);
    product_list_wgt->setFixedHeight(475);

    if(action == UpdateProduct){
        update_product = new BasicProductUpdateWidget(product_mod, update_page,"modificar");
        QLayout *lay = 0;
        if (!(lay = select_stack->layout())){
            QHBoxLayout *layout = new QHBoxLayout(update_page);
            layout->addWidget(update_product);
        }
        else lay->addWidget(update_product);
    }

    next_button->hide();
    previous_button->hide();
    cancel_button->hide();

    connect(search_product_edit, SIGNAL(textChanged(const QString&)), this, SLOT(productNameChanged(const QString&)));
    connect(product_list_wgt, SIGNAL(clicked(QListViewItem*)), this, SLOT(productSelectedChanged(QListViewItem*)));
    connect(accept_button, SIGNAL(clicked()), this, SLOT(acceptSlot()));
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(cancelSlot()));
    connect(next_button, SIGNAL(clicked()), this, SLOT(nextSlot()));
    connect(previous_button, SIGNAL(clicked()), this, SLOT(previousSlot()));
    connect(up_button,SIGNAL(clicked()),this,SLOT(upClickedSlot()));
    connect(down_button,SIGNAL(clicked()),SLOT(downClickedSlot()));

    up_button->setAutoRepeat(true);
    down_button->setAutoRepeat(true);
    clear();
}

void BasicProductEditionWidget::init(){

    clear();
    delete product_data;

    XmlConfig xml;
    if(!xml.readXmlFromString(product_mod->getUnitaryProducts())){
        cerr << "Could not convert string to xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(PRODUCTS_DTD, true)){
        cerr << "The xml does not have a correct structure " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    xml.delDomain();
    xml.setDomain("products");

    product_list_wgt->clear();
    for(auto i = 0; i < xml.howManyTags("product"); i++){
        xml.setDomain("product[" + QString::number(i) + "]");
        auto name = xml.readString("name");
        auto code = xml.readString("code");

        if ( name.isEmpty() ||  code.isEmpty()) continue;
        if (name == "varios" && code == "varios") continue;

        auto item = new QTreeWidgetItem(product_list_wgt);
        if( ! xml.readString("logo").isEmpty() ){
            auto iconfile = ICON_PATH + xml.readString("logo");
            item->setIcon(Icon, cropedIcon(iconfile, ICON_SIZE));
        }
        item->setText(Name, name);
        item->setText(Code, code);
        xml.releaseDomain("product", true);
    }
    xml.releaseDomain("products");

    product_list_wgt->sortByColumn(Code);
    if ( !last_name.isEmpty() )
        productNameChanged(last_name);

    up_button->show();
    down_button->show();
    cancel_button->hide();
}

void BasicProductEditionWidget::upClickedSlot(){
    QTreeWidgetItem *next{};
    auto selected_items = product_list_wgt->selectedItems();

    if ( selected_items.isEmpty() )  return;
    auto current_item = selected_items.first();

    if( current_item == product_list_wgt->topLevelItem(0)) next = current_item;
    else next = product_list_wgt->itemAbove(current_item);

    next->setSelected(true);
    next->setHidden(false);
    accept_button->setEnabled(true);
}

void BasicProductEditionWidget::downClickedSlot(){

    QTreeWidgetItem *next{};
    auto selected_items = product_list_wgt->selectedItems();

    if ( selected_items.isEmpty() )  return;
    auto current_item = selected_items.first();

    if( current_item == product_list_wgt->topLevelItem(product_list_wgt->topLevelItemCount() - 1 ))
        next = current_item;
    else next = product_list_wgt->itemBelow(current_item);

    next->setSelected(true);
    next->setHidden(false);
    accept_button->setEnabled(true);
}

void BasicProductEditionWidget::productNameChanged(const QString& text){

    if(text.isEmpty()){
        product_list_wgt->clearSelection();
        return;
    }

    auto items = product_list_wgt->findItems(text, Qt::MatchExactly, Name);

    if(!items.isEmpty()){
        product_list_wgt->setCurrentItem(items.first());
        items.first()->setHidden(false);
    }
}

void BasicProductEditionWidget::productSelectedChanged(QTreeWidgetItem *item){

    if (!item)  return;
    accept_button->setEnabled(true);

    auto code = item->text(Code);
    if ( item->childCount() ){
        item->setExpanded( !item->isExpanded() );
        return;
    }

    QString xml_string = product_mod->getCompositionsWithIngredient(code);
    XmlConfig xml;

    if(!xml.readXmlFromString(xml_string)){
        cerr << "Could not convert string to xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(PRODUCTS_DTD, true)){
        cerr << "The xml does not have a valid structure " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
    xml.setDomain("products");
    for( auto i = 0; i < xml.howManyTags("product"); i++){
        auto new_item = new QTreeWidgetItem(item);
        new_item->setIcon(Icon, item->icon(Icon));
        auto name = xml.readString("product[" + QString::number(i) + "].name");
        new_item->setText(Name, name);
        auto code = xml.readString("product[" + QString::number(i) + "].code");
        new_item->setText(Code, code);
        new_item->setExpanded(false);
    }

    item->setExpanded(false);
    xml.releaseDomain("products", true);
}

void BasicProductEditionWidget::acceptSlot(){

    if ( select_stack->currentWidget() == select_product_page){
        auto items = product_list_wgt->selectedItems();
        if(items.isEmpty()){
            accept_button->setEnabled(false);
            return;
        }

        auto item = items.first();
        auto item_parent = item->parent();
        if (item_parent)
            last_name = item_parent->text(Name);
        else
            last_name = item->text(Name);

        getProductFromItem(item);
    }

    switch (action){
    case DeleteProduct: //delete
        if(select_stack->currentWidget() == select_product_page){
            select_stack->setCurrentWidget(delete_product);
            product_name_label->setText(product_data->name);
            accept_button->setEnabled(true);
            accept_button->show();
            next_button->hide();
            previous_button->hide();
            up_button->hide();
            cancel_button->show();
            down_button->hide();
            return;
        }
        if(select_stack->currentWidget() == delete_product){
            if(!product_data){
                showSelectProduct();
                return;
            }
            deleteProduct();
            this->clear();
            showSelectProduct();
            init();
        }
        break;
    case UpdateProduct: // update
        if(select_stack->currentWidget() == select_product_page){
            title_select->setText("Modify product :"+ product_data->name);
            select_stack->setCurrentWidget(update_page);
            update_product->showUpdate();
            update_product->setProduct(product_data);
            up_button->hide();
            down_button->hide();
            if(product_mod->isUnitaryProduct(product_data->code)){
                accept_button->setEnabled(true);
                accept_button->show();
                cancel_button->show();
                next_button->hide();
                previous_button->hide();
            }
            else{
                accept_button->hide();
                next_button->show();
                cancel_button->show();
                next_button->setEnabled(true);
                previous_button->hide();
            }
            return;
        }
        if(select_stack->currentWidget() == update_page){
            if(update_product->updateProduct()){
                delete product_data;
                clear();
                showSelectProduct();
                init();
            }
            else
                cerr << "Could not modify product" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        }
        break;
    }
}

void BasicProductEditionWidget::clear(){

    product_name_label->clear();
    product_list_wgt->clear();

    search_product_edit->clear();
    accept_button->setEnabled(false);

    delete product_data;

    switch (action){
    case DeleteProduct:
        title_select->setText("Select the product to delete");
        break;
    case UpdateProduct:
        title_select->setText("Select the product to modify");
        break;
    }
}

void BasicProductEditionWidget::showSelectProduct(){
    select_stack->setCurrentWidget(select_product_page);
    delete product_data;

    accept_button->show();
    accept_button->setEnabled(false);
    next_button->hide();
    previous_button->hide();
}

void BasicProductEditionWidget::showEvent(QShowEvent *e){
    if (select_stack->currentWidget() == update_page && update_page->isVisible()){
        if(!product_data){
            showSelectProduct();
            QWidget::showEvent(e);
            return;
        }
        update_product->showUpdate();
        update_product->setProduct(product_data);
    }
    else if ( select_stack->currentWidget() == delete_product && delete_product->isVisible() ){
        if( !product_data ){
            showSelectProduct();
            QWidget::showEvent(e);
            return;
        }
    }
    else if (select_stack->currentWidget() == select_product_page && select_product_page->isVisible()){
        init();
    }

    QWidget::showEvent(e);
}

void BasicProductEditionWidget::nextSlot(){
    update_product->showUpdateComposition();
    accept_button->show();
    accept_button->setEnabled(true);
    next_button->hide();
    previous_button->show();
    previous_button->setEnabled(true);
}

void BasicProductEditionWidget::deleteProduct(){

    if (product_mod->isUnitaryProduct(product_data->code)){
        //check if there are more product with this ingredient...
        auto xml_string = product_mod->getCompositionsWithIngredient(product_data->code);
        XmlConfig xml;
        if(!xml.readXmlFromString(xml_string)){
            cerr << "Could not convert string to xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            return;
        }
        if(!xml.validateXmlWithDTD(SELECT_PRODUCTS_LIST_DTD, true)){
            cerr << "The xml does not have a correct structure " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            return;
        }
        xml.delDomain();
        xml.setDomain("products");
        if (xml.howManyTags("product")){
            xml_string = "You cannot delete a product that is an ingredient\n";
            xml_string+="of other combinations... First delete all\n";
            xml_string+="combinations of that product and then you can delete it\n";
            QMessageBox::warning(this,"Failed to delete product", xml_string, QMessageBox::NoButton);
            cerr << "Error deleting the product " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            cerr << "Could not delete product " << product_data->name.toStdString() << endl;
            delete product_data;
            return;
        }

    }
    if (!product_mod->deleteProduct(product_data->code)){
        delete product_data;
        cerr << "Error deleting the product " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "Could not delete product" << product_data->name.toStdString() << endl;
    }
    delete product_data;
    clear();
    showSelectProduct();
}

void BasicProductEditionWidget::hideEvent(QHideEvent *e){
    delete product_data;
    product_data = 0;
    last_name = "";

    if(update_product)
        update_product->hideWidget();

    QWidget::hideEvent(e);
}

void BasicProductEditionWidget::productUpdatedSlot(QString p_code){
    if( action == UpdateProduct && select_stack->currentWidget() == update_page){
        if(product_data->code == p_code){
            QString text   = "There has been a change in the products part of the database...\n";
            text += "the database will be reloaded";
            QMessageBox::warning(this,"Change of product in the database",text, QMessageBox::NoButton);
            showSelectProduct();
            init();
        }
    }
}

void BasicProductEditionWidget::productDeletedSlot(QString p_code){
    if( action == UpdateProduct && select_stack->currentWidget() == update_page ){
        if(product_data->code == p_code){
            QString text   = "There has been a change in the products part of the database...\n";
            text += "the database will be re-read";
            QMessageBox::warning(this,"Change of product in the database",text, QMessageBox::NoButton);
            showSelectProduct();
            init();
        }
    }
}

void BasicProductEditionWidget::previousSlot(){
    update_product->previousSlot();
    update_product->previous()->hide();
    update_product->next()->hide();
    update_product->cancel()->hide();
    accept_button->hide();
    previous_button->hide();
    next_button->show();
    next_button->setEnabled(true);
}

void BasicProductEditionWidget::cancelSlot(){
    this->clear();
    showSelectProduct();
    init();
    if (!last_name.isEmpty())
        this->productNameChanged(last_name);
}

void BasicProductEditionWidget::getProductFromItem(QTreeWidgetItem *item){

    if (!item){
        delete product_data;
        return;
    }

    XmlConfig xml;
    if(!xml.readXmlFromString(product_mod->getProduct(item->text(Code)))){
        cerr << "Could not convert string to xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(PRODUCTS_DTD, true)){
        cerr << "The xml does not have a correct structure" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    delete product_data;
    product_data = new ProductData;
    xml.delDomain();
    xml.setDomain("products[0].product[0]");
    product_data->code = xml.readString("code");
    product_data->name = xml.readString("name");
    product_data->description = xml.readString("description");
    product_data->price = (xml.readString("price")).toDouble();
    product_data->tax = xml.readString("tax");
    product_data->logo = xml.readString("logo");
}
