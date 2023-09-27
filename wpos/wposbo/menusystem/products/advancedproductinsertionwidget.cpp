/***************************************************************************
                          nupdateproductwidget.cpp  -  description
                             -------------------
    begin                : lun may 3 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/

#include "advancedproductinsertionwidget.h"
#include "menusystem/utils.h"

#include "productsmodule/productmodule.h"
#include "productsmodule/optionsmodule/productoptionmodule.h"
#include "productsmodule/offersmodule/productoffermodule.h"

#include <wposwidget/dragobjects.h>
#include <wposwidget/floatkeyboardbox.h>
#include <wposwidget/basickeyboardbox.h>


#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QMatrix>
#include <QLabel>
#include <QButtonGroup>
#include <QColor>
#include <QTableWidget>
#include <QLayout>
#include <QStackedWidget>
#include <QMessageBox>
#include <QCheckBox>

extern "C"{
#include <math.h>
}

static const QString& PRODUCTS_LIST_DTD {"/etc/ntpv_backoffice/dtds/products_productslist.dtd"};
static const QString& PRODUCT_COMPOSITION_DTD {"/etc/ntpv_backoffice/dtds/products_composition.dtd"};
static const QString& TAXES_DTD {"/etc/ntpv_backoffice/dtds/products_taxeslist.dtd"};
static const QString& OPTIONS_LIST_DTD {"/etc/ntpv_backoffice/dtds/products_optionslist.dtd"};
static const QString& OFFERS_LIST_DTD  {"/etc/ntpv_backoffice/dtds/products_offerslist.dtd"};

//static const QPixmap NULL_LOGO_PIXMAP = QPixmap();
static const QString& WITHOUT_LOGO {"WITHOUT LOGO"};
static const QString& ICON_PATH {"/usr/share/ntpv/logos/"};
static const double ICON_BUTTON_SIZE  {50.00};
static const double SIZE_ICON {32.00};

static const QString& DEFAULT_TAX {"iva16"};

#include <iostream>
using namespace std;

AdvancedProductInsertionWidget::AdvancedProductInsertionWidget(ProductModule *product_module ,int mode,QWidget *parent, const QString& name):
    ProductInsertionWidget(product_module, mode, parent, name){}

void AdvancedProductInsertionWidget::clear(){
    p_code.clear();
    ProductInsertionWidget::clear();
}

void AdvancedProductInsertionWidget::setProductWithName(const QString& product){
    clear();
    QString product_code = product_model->getProductCodeByProductName(product);
    if (product_code.isEmpty()) return;
    setProduct(product_code);
}

void AdvancedProductInsertionWidget::setMode(int num){
    ProductInsertionWidget::setMode(num);
}

void AdvancedProductInsertionWidget::setProduct(const QString& product_code){

    clear();
    main_stack->setCurrentWidget(progress);
    setMode(ProductInsertionWidget::Unitary);
    if ( product_model->getProductName(product_code).isEmpty())
        return;
    if (!product_model->isUnitaryProduct(product_code))
        this->setMode(ProductInsertionWidget::Composed);
    main_stack->setCurrentWidget(main);
    XmlConfig xml;
    auto xml_string = product_model->getProduct(product_code);
    if(!xml.readXmlFromString(xml_string)){
        cerr << "can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }
    xml.delDomain();
    xml.setDomain("products.product");
    p_code = xml.readString("code");
    product_name_lineedit->setText(xml.readString("name"));
    float_kb->setNumber((xml.readString("price")).toDouble());
    float_kb->setInitMode(true);
    xml_string = xml.readString("tax");

    for (auto button : tax_button_group->buttons())
        if ( button->objectName() == xml_string && button->isDown() ) button->toggle();

    taxChanged();
    setLogo(xml.readString("logo"));

    if (!product_model->isUnitaryProduct(product_code))
        setComposition(product_code);

    setOptions(product_code);
    setOffers(product_code);

    ok_button->setEnabled(checkAllValues());
    kitchen_checkbox_button->setChecked(product_model->getProductAtPrinter(product_name_lineedit->text(),"kitchen"));
}

void AdvancedProductInsertionWidget::setComposition(const QString& product_code){

    auto xml_string = product_model->getIngredients(product_code);
    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(PRODUCT_COMPOSITION_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    table->setRowCount(0);
    xml.setDomain("composition");
    for(auto i=0;i < xml.howManyTags("ingredient"); i++){
        xml.setDomain("ingredient["+QString::number(i)+"]");
        auto i_code = xml.readString("ingredient_code");
        auto i_name = product_model->getProductName(i_code);
        auto quantity = xml.readString("quantity").toDouble();

        if ( quantity <= 0.0 ) quantity = 1.0;

        auto pos = table->rowCount();
        table->insertRow(pos);
        table->item(pos,0)->setText(i_name);
        table->item(pos,1)->setText(QString::number(quantity,'f',2));
        xml.releaseDomain("ingredient");
    }
}

void AdvancedProductInsertionWidget::setOptions(const QString& product_code){

    clearOptions();
    XmlConfig xml;

    if(!xml.readXmlFromString(product_option_model.getProductOptions(product_code))){
        cerr << "can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    xml.delDomain();
    xml.setDomain("options");

    bool is_default { false};
    for (auto i=0 ; i < xml.howManyTags("option"); i++){
        xml.setDomain("option["+QString::number(i)+"]");
        auto type = xml.readString("option_type");
        auto name = xml.readString("option_name");
        auto value = xml.readString("value").toDouble() + float_kb->getNumber();

        if (xml.readString("default") == "true") is_default = true;
        else is_default = false;

        NOTypes *no_type = options_list->find(type);
        if (! no_type){
            no_type = new NOTypes();
            no_type->type = type;
            options_list->append(no_type,no_type->type);
        }

        NOData *no_data = no_type->list.find(name);
        if (!no_data){
            no_data = new NOData;
            no_data->is_default = is_default;
            no_data->name = name;
            no_data->state = NOData::Original;
            no_data->value = QString::number(value,'f',2);
            no_type->list.append(no_data,no_data->name);
        }
        xml.releaseDomain("option");
    }
}

void AdvancedProductInsertionWidget::setOffers(const QString& product_code){

    bool is_default = false;
    clearOffers();
    QString xml_string = ProductOfferModule().getProductOffers(product_code);
    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    xml.delDomain();
    xml.setDomain("offers");

    for ( auto i=0; i < xml.howManyTags("offer"); i++){
        xml.setDomain("offer["+QString::number(i)+"]");
        auto type = xml.readString("offer_type");
        auto name = xml.readString("offer_name");
        auto cpp_operator = xml.readString("cpp_operator");
        auto value = xml.readString("value").toFloat();
        is_default = false;

        NOTypes *no_type = offers_list->find(type);
        if (! no_type){
            no_type = new NOTypes();
            no_type->type = type;
            offers_list->append(no_type,no_type->type);
        }
        NOData *no_data = no_type->list.find(name);
        if (!no_data){
            no_data = new NOData;
            no_data->is_default = is_default;
            no_data->name = name;
            no_data->state = NOData::Original;
            if (cpp_operator == "x"){
                auto no_data_value = (int) lround ( (1.00 - value) *100);
                no_data->value = QString::number(no_data_value);
            }
            else
                no_data->value = QString::number(value,'f',2);
            no_type->list.append(no_data,no_data->name);
        }
        xml.releaseDomain("offer");
    }

}

void AdvancedProductInsertionWidget::setLogo(const QString& _logo){
    logo = _logo;
    if (logo.isEmpty()){
        logo_button->setText(WITHOUT_LOGO);
        logo_button->setIcon(QPixmap());
        return;
    }

    logo_button->setIcon(cropedIcon(ICON_PATH + logo, ICON_BUTTON_SIZE ));
}

void AdvancedProductInsertionWidget::showEvent(QShowEvent *e){
    QWidget::showEvent(e);
}

void AdvancedProductInsertionWidget::numkeyChangedSlot(double num){
    QString product_code;
    price_label->setText(QString::number(num,'f',2)+tr(" Euros"));

    product_code = product_model->getProductCodeByProductName(product_name_lineedit->text());
    if (product_code.isEmpty())
        return;

    setOptions(product_code);
}

bool AdvancedProductInsertionWidget::saveProduct(){
    QString product_code;
    QString xml_string;

    product_code = getCurrentProductCode();
    if (product_code.isEmpty()){
        QString text = tr("There is no product code.\nMaybe you have not entered the name\n or the composition of the product\n");
        QMessageBox::information(this, tr("Can't insert product"), text, QMessageBox::Ok);
        return false;
    }

    if  (mode){
        product_model->deleteProduct( p_code );
        p_code = getCurrentProductCode();
    }

    XmlConfig xml;
    xml.createElementSetDomain("products");
    xml.createElementSetDomain("product");
    xml.createElement("code", p_code);
    xml.createElement("name",product_name_lineedit->text());
    xml.createElement("price", QString::number(float_kb->getNumber(),'f',2));
    xml.createElement("tax", getActualTax());
    xml.createElement("logo", logo);
    if (!mode)
        xml.createElement("description", product_name_lineedit->text()+"_unitary_product");
    else
        xml.createElement("description", product_name_lineedit->text()+"_composed_product");
    xml.releaseDomain("products");
    xml_string = xml.toString();

    if  (mode)  return product_model->insertProduct(xml_string);
    return product_model->updateProduct(xml_string);
}

bool AdvancedProductInsertionWidget::saveProductComposition(){

    if (!mode)  return true;
    XmlConfig xml;
    xml.createElementSetDomain("composition");
    xml.createElement("composition_code", p_code);
    for ( auto i = 0; i <table->rowCount();i++ ){
        auto name = table->item(i,0)->text();
        auto quantity = table->item(i,1)->text();
        auto code = product_model->getProductCodeByProductName(name);
        xml.createElementSetDomain("ingredient");
        xml.createElement("ingredient_code",code);
        xml.createElement("quantity", quantity);
        xml.releaseDomain("ingredient");
    }
    xml.releaseDomain("composition");
    return product_model->insertProductComposition(xml.toString());
}

void AdvancedProductInsertionWidget::acceptSlot(){
    ProductInsertionWidget::acceptSlot();
    p_code.clear();
}
