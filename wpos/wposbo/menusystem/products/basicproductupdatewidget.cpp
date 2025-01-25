/***************************************************************************
                          bslupdateproductswidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "basicproductupdatewidget.h"
#include "productoffercreationwidget.h"
#include "productoptioncreationwidget.h"

#include "database/productsmoduledb.h"
#include "productsmodule/productmodule.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/config.h>
#include <wposgui/common/dragobjects.h>
#include <wposgui/keyboard/floatkeyboard.h>

#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTextEdit>

#include <iostream>
using namespace std;

const QString& UPDATE_COMPOSITION_DTD = "dtddocs:products_composition.dtd";
const QString& UPDATE_PRODUCTS_LIST_DTD = "dtddocs:products_productslist.dtd";
const QString& UPDATE_OPTIONS_LIST_DTD = "dtddocs:products_optionslist.dtd";
const QString& UPDATE_OFFERS_LIST_DTD = "dtddocs:products_offerslist.dtd";
const QString& UPDATE_OFFER_ICON_PATH = cfg::PRODUCT_DIR;

BasicProductUpdateWidget::BasicProductUpdateWidget(
    ProductModule* _product_mod,
    QWidget* parent, const QString& name)
    : BasicProductCreationWidget(_product_mod, 0, parent, name)
    , product_mod { _product_mod }
{

    accept_button->hide();
    cancel_button->hide();
    next_button->hide();
    previous_button->hide();
    title->hide();
    caption_frame->hide();
}

bool BasicProductUpdateWidget::updateProduct()
{

    if (product_mod->isUnitaryProduct(product->code)) {
        if (!updateUnitaryProduct()) {
            cerr << "Could not modify unit product" << product->code.toStdString() << " "
                 << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            return false;
        }
    } else {
        if (!updateCompositionProduct()) {
            cerr << "Could not modify the combination" << product->code.toStdString() << " "
                 << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            return false;
        }
    }

    if (!option_widget->saveOptionsProduct(product->code)) {
        cerr << "Could not save product options " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (!offer_widget->saveOffersProduct(product->code)) {
        cerr << "Could not save product offers" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    clear();
    return true;
}

void BasicProductUpdateWidget::setProduct(ProductData* prod)
{
    if (!prod || isProductEmpty(prod))
        return;

    if (!product)
        product = new ProductData;

    product = prod;

    product_name->setText(prod->name);
    description_text->setText(prod->description);

    if (!prod->logo.isEmpty()) {
        getLogo(UPDATE_OFFER_ICON_PATH + prod->logo);
        logo = prod->logo;
    } else {
        logo.clear();
        logo_button->setIcon(QIcon());
        logo_button->setText("Logo");
    }

    setTax(prod->tax);
    float_keyboard_product->setValue(prod->price);

    if (product_mod->isUnitaryProduct(prod->code))
        setModeProduct(UnitaryProduct);
    else {
        setModeProduct(CompositionProduct);

        if (ingredients_list_old) {
            ingredients_list_old = 0;
            delete ingredients_list_old;
        }
        ingredients_list_old = getIngredientsProduct(prod->code);
    }
}

bool BasicProductUpdateWidget::updateCompositionProduct()
{

    QString code_aux = getCode();

    XmlConfig xml;
    xml.createElementSetDomain("products");
    xml.createElementSetDomain("product");

    xml.createElement("code", code_aux);
    xml.createElement("name", product_name->text());
    xml.createElement("price", QString::number(float_keyboard_product->value()));
    xml.createElement("tax", getTax());
    xml.createElement("logo", logo);

    //     xml.createElement("description", description_text->text());

    xml.releaseDomain("product", true);
    xml.releaseDomain("products", true);

    if (!xml.validateXmlWithDTD(UPDATE_PRODUCTS_LIST_DTD, true)) {
        cerr << "The xml does not have a valid structure" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }
    auto xml_string = xml.toString();

    // If the product's code not changed update this product
    if (code_aux == product->code)
        product_mod->updateProduct(xml_string);
    else {
        if (!product_mod->deleteProduct(product->code))
            return false;

        if (!product_mod->insertProduct(xml_string)) {
            cerr << "Could not save product" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            return false;
        }
    }

    if (!createComposition(code_aux)) {
        cerr << "Could not save product composition" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }
    return true;
}

bool BasicProductUpdateWidget::updateUnitaryProduct()
{
    QString xml_string;
    XmlConfig xml;
    xml.createElementSetDomain("products");
    xml.createElementSetDomain("product");

    QString code = getCode();
    xml.createElement("code", code);
    xml.createElement("name", product_name->text());
    xml.createElement("price", QString::number(float_keyboard_product->value()));
    xml.createElement("tax", getTax());
    xml.createElement("logo", logo);
    //     xml.createElement("description", description_text->text());

    xml.releaseDomain("product", true);
    xml.releaseDomain("products", true);

    if (!xml.validateXmlWithDTD(UPDATE_PRODUCTS_LIST_DTD, true)) {
        cerr << "El xml no tiene una estructura valida " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }
    xml_string = xml.toString();

    if (!product_mod->updateProduct(xml_string))
        return false;

    if (!product_mod->isUnitaryProduct(code))
        if (!createComposition(code))
            return false;

    if (!option_widget->saveOptionsProduct(code))
        return false;
    return true;
}

bool BasicProductUpdateWidget::createComposition(const QString& code)
{
    int count, index;
    QString xml_string;

    IngredientData* ingredient = 0;
    clearCodeList();

    for (index = 0; index < table->rowCount(); index++) {
        ingredient = 0;
        ingredient = new IngredientData;
        ingredient->ingredient_code = product_mod->getProductCodeByProductName(table->item(index, 1)->text());
        ingredient->quantity = table->item(index, 2)->text().toDouble();
        code_list.append(ingredient);
    }

    XmlConfig xml;
    xml.createElementSetDomain("composition");
    xml.createElement("composition_code", code);
    count = code_list.count();
    for (index = 0; index < count; index++) {
        ingredient = 0;
        ingredient = code_list.at(index);

        xml.createElementSetDomain("ingredient");
        xml.createElement("ingredient_code", ingredient->ingredient_code);
        xml.createElement("quantity", QString::number(ingredient->quantity));
        xml.releaseDomain("ingredient", true);
    }
    xml.releaseDomain("composition", true);

    if (!xml.validateXmlWithDTD(UPDATE_COMPOSITION_DTD, true)) {
        cerr << "The xml does not have a valid structure " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }

    xml_string = xml.toString();

    if (!product_mod->insertProductComposition(xml_string))
        return false;

    return true;
}

void BasicProductUpdateWidget::showUpdate()
{
    init();
    accept_button->hide();
    cancel_button->hide();
    next_button->hide();
    previous_button->hide();
}

void BasicProductUpdateWidget::showUpdateComposition()
{
    int index, count;
    IngredientData* ingredient = 0;

    product_stack->setCurrentWidget(product_composition_page);

    table->setRowCount(0);
    if (!ingredients_list_old)
        ingredients_list_old = getIngredientsProduct(product->code);

    count = ingredients_list_old->count();
    for (index = 0; index < count; index++) {
        ingredient = 0;
        ingredient = ingredients_list_old->at(index);
        table->setRowCount(table->rowCount() + 1);
        table->item(index, 1)->setText(product_mod->getProductName(ingredient->ingredient_code));
        table->item(index, 2)->setText(QString::number(ingredient->quantity));
    }

    insertItems(icon_view, true);
}

void BasicProductUpdateWidget::showEvent(QShowEvent* e)
{
    initLogos(logo_view);
    init();
    QWidget::showEvent(e);
}

void BasicProductUpdateWidget::hideWidget()
{
    clear();
    delete product;
    clearCodeList();
    clearButtonList();
}

QPushButton* BasicProductUpdateWidget::next() const
{
    return next_button;
}

QPushButton* BasicProductUpdateWidget::previous() const
{
    return previous_button;
}
QPushButton* BasicProductUpdateWidget::cancel() const
{
    return cancel_button;
}

QList<IngredientData*>* BasicProductUpdateWidget::getIngredientsProduct(const QString& product_code)
{

    QString xml_string = product_mod->getIngredients(product_code);

    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Could not convert string to xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return nullptr;
    }

    if (!xml.validateXmlWithDTD(UPDATE_COMPOSITION_DTD, true)) {
        cerr << "The xml does not have a valid structure " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return nullptr;
    }

    QList<IngredientData*>* ret_list = new QList<IngredientData*>;
    xml.delDomain();
    xml.setDomain("composition");
    for (auto i = 0; i < xml.howManyTags("ingredient"); i++) {
        auto ingredient = new IngredientData;
        xml.setDomain("ingredient[" + QString::number(i) + "]");
        ingredient->ingredient_code = xml.readString("ingredient_code");
        ingredient->quantity = (xml.readString("quantity")).toDouble();
        xml.releaseDomain("ingredient");
        ret_list->append(ingredient);
    }
    xml.releaseDomain("composition");
    return ret_list;
}

bool BasicProductUpdateWidget::isProductEmpty(ProductData* prod)
{
    return (prod->code.isEmpty() && prod->name.isEmpty());
}
