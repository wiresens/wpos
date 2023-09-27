/***************************************************************************
                          productdata.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productdata.h"
#include <QString>
#include <xmlconfig.h>

#include <optionnode.h>


#include <iostream>
namespace std{}
using namespace std;


ProductData::ProductData(XmlConfig *xml): list(0),options(0){
    list = new HList<ProductData>;
    list->setAutoDelete(true);

    options = new HList<OptionNode>;
    options->setAutoDelete(true);

    initProduct(xml);
}

ProductData::ProductData():list(0),options(0){

    list = new HList<ProductData>;
    list->setAutoDelete(true);

    options = new HList<OptionNode>;
    options->setAutoDelete(true);

    setProductName(UNKNOWN);
}

ProductData::~ProductData(){
    this->reset();
    delete list;

    delete options;
    list = 0;
}


bool ProductData::initProduct(XmlConfig *xml){
    QString aux;
    int i, count;
    ProductData *prod=0;
    OptionNode *option=0;


    xml->pushDomain();

    list->clear();
    options->clear();

    aux = xml->readString("name");
    if (aux.isEmpty()){
        setProductName(UNKNOWN);
        xml->popDomain();
        return false;
    }

    setProductName(aux);

    //check the options...
    if (xml->setDomain("options")){
        count = xml->howManyTags("option");
        for(i=0;i<count;i++){
            xml->setDomain("option["+QString::number(i)+"]");
            aux = xml->readString("type");
            option = new OptionNode(aux);
            for(int j=0;j<xml->howManyTags("value");j++){
                aux = xml->readString("value["+QString::number(j)+"]");
                if (xml->readAttribute("value["+QString::number(j)+"]","type")=="default")
                    option->addOption(aux,true);
                else
                    option->addOption(aux);

            }
            xml->releaseDomain("option");
            options->append(option, option->getOptionType());
        }
        xml->releaseDomain("options");
    }
    //the elements should be deleted only at the list
    //but should be removed from both.

    //save the domain.

    count = xml->howManyTags("product");
    for (i=0; i< count; i++){
        xml->setDomain("product["+QString::number(i)+"]");
        prod = new ProductData(xml);
        append(prod);
        xml->releaseDomain("product",false);
    }
    xml->popDomain();
    return true;
}


QString ProductData::getProductName(){
    return product_name;
}


bool ProductData::setProductName(const QString& _name){
    this->product_name = _name;
    return true;
}

//make a reset to all the products and then
//delete all the products
bool ProductData::reset(){
    int i,count;
    ProductData *prod=0;
    count = this->list->count();

    for(i=0;i<count;i++){
        prod = this->list->at(i);
        prod->reset();
    }

    this->list->clear();
    this->setProductName(UNKNOWN);
    return true;
}

QStringList ProductData::getCombinations(){
    int count,i;
    QStringList ret;
    QString aux;
    ProductData *prod = 0;

    count = list->count();
    for (i=0;i<count;i++){
        prod = list->at(i);
        aux = prod->getProductName();
        ret.append(aux);
    }

    return ret;
}

//append a product at the dict and the lists
bool ProductData::append(ProductData *product){
    this->list->append(product,product->getProductName());
    return true;
}

//the number of actual products at the list
int ProductData::count(){
    return this->list->count();
}

//get the product at position pos from the list or 0 if does not exist that position
ProductData* ProductData::getProductAt(const int pos){
    if ((pos > (int) list->count()) || (pos < 0))
        return nullptr;
    return this->list->at(pos);
}

//get a product from the list
ProductData* ProductData::getProduct(const QString& name){
    return (list->find(name));
}

bool ProductData::removeProductAt(const int pos){
    ProductData *prod;
    QString aux;
    if ((pos > (int) list->count()) || (pos < 0))
        return false;

    prod = this->list->at(pos);
    prod->reset();

    list->remove(pos);
    return true;
}

bool ProductData::removeProduct(const QString& name){
    int count,i;
    ProductData *prod=0;
    ProductData *prod_aux=0;
    QString aux;

    prod = list->find(name);
    if (!prod)
        return false;

    aux = prod->getProductName();

    count = list->count();
    for (i=0;i<count;i++){
        prod_aux = list->at(i);
        if (prod==prod_aux){
            list->remove(i);
            break;
        }
    }
    return true;
}
