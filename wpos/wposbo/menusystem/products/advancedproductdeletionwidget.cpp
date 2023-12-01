#include "advancedproductdeletionwidget.h"

#include "productsmodule/productmodule.h"

#include <libbslxml/xmlconfig.h>

#include <QMessageBox>

#include <iostream>
using namespace std;

static const QString& PRODUCTS_DTD {"/etc/ntpv_backoffice/dtds/products_productslist.dtd"};

AdvancedProductDeletionWidget::AdvancedProductDeletionWidget(
    ProductModule* product_model,
    QWidget* parent,
    const QString& name):
    AdvancedProductEditionWidget(product_model, parent, name)
{
    connect(ok_delete_button,  &QPushButton::clicked, this, &AdvancedProductDeletionWidget::deleteSlot);
    connect(cancel_delete_button, &QPushButton::clicked, this, &AdvancedProductDeletionWidget::cancelDeletedSlot);
}

void AdvancedProductDeletionWidget::acceptSlot(){
    auto items = product_listview->selectedItems();
    if( items.isEmpty()) return;
    auto item = items.first();
    product_name_label->setText(item->text(Name));
    stack->setCurrentWidget(delete_page);
}

void AdvancedProductDeletionWidget::cancelDeletedSlot(){
    stack->setCurrentWidget(product_page);
}

void AdvancedProductDeletionWidget::deleteSlot(){
    auto items = product_listview->selectedItems();
    if( items.isEmpty()) return;
    auto item = items.first();
    auto name = item->text(Name);
    auto code = item->text(Code);

    if ((productModel().isUnitaryProduct(code))&&(checkUnitaryUse(code))){
        QString msg = tr("You have chosen to delete a unit product \n"
                         "found as an ingredient in other products.\n"
                         "If you want to delete the chosen product you must also delete\n"
                         "all products that contain it.\n"
                         "Do you want to delete the compound products that contain the product %1?").arg(name);

        if( QMessageBox::question( this, tr("Update product"), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No ){
            cancelDeletedSlot();
            return;
        }
        else deleteAllProductsContaining(code);
    }

    if ( !productModel().deleteProduct(code)) {
        QString text = tr("The product could not be deleted %1.\n\n").arg(code);
        QMessageBox::information(this, tr("Can't delete the product %1").arg(code),text, QMessageBox::Ok);
        return;
    }
    startShowing();
}

bool AdvancedProductDeletionWidget::checkUnitaryUse(const QString& product_code){
    XmlConfig xml;
    if(!xml.readXmlFromString(productModel().getCompositionsWithIngredient(product_code))){
        cerr << "can not convert the string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(!xml.validateXmlWithDTD(PRODUCTS_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("products");
    for(auto i = 0; i < xml.howManyTags("product"); i++){
        auto code = xml.readString("product[" + QString::number(i) + "].code");
        if ( code != product_code) return true;
    }

    return false;
}

void AdvancedProductDeletionWidget::deleteAllProductsContaining(const QString& product_code){

    XmlConfig xml;
    if(!xml.readXmlFromString(productModel().getCompositionsWithIngredient(product_code))){
        cerr << "can not convert the string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(PRODUCTS_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
    xml.setDomain("products");
    for(auto i = 0; i < xml.howManyTags("product"); i++){
        auto code = xml.readString("product[" + QString::number(i) + "].code");
        if ( code != product_code){
            if ( !productModel().deleteProduct(code) ){
                QString text = tr("The product could not be deleted %1.\n\n").arg(code);
                QMessageBox::information(this, tr("Deletion Error").arg(code), text, QMessageBox::Ok);
            }
        }
    }
}
