/***************************************************************************
                          productdata.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "relatedproductgroup.h"
#include <QString>
#include <libbslxml/xmlconfig.h>

#include <wposcore/productextrainfo.h>


const QString UNKNOWN {"STRANGER"};

RelatedProductGroup::RelatedProductGroup(XmlConfig &xml)
{
    initProduct(xml);
}

RelatedProductGroup::RelatedProductGroup()
{
    setProductName(UNKNOWN);
}

RelatedProductGroup::~RelatedProductGroup(){
    reset();
}

bool RelatedProductGroup::initProduct(XmlConfig &xml){
    QString aux;
    int i, count;
    RelatedProductGroup *prod=0;
    ProductExtraInfo *option=0;

    xml.pushDomain();

    relatedGroup.clear();
    extraInfos.clear();

    aux = xml.readString("name");
    if (aux.isEmpty()){
        setProductName(UNKNOWN);
        xml.popDomain();
        return false;
    }

    setProductName(aux);

    //check the options...
    if (xml.setDomain("options")){
        count = xml.howManyTags("option");
        for(i=0;i<count;i++){
            xml.setDomain("option["+QString::number(i)+"]");
            aux = xml.readString("type");
            option = new ProductExtraInfo(aux);
            for(int j=0;j<xml.howManyTags("value");j++){
                aux = xml.readString("value["+QString::number(j)+"]");
                if ( xml.readAttribute("value["+QString::number(j)+"]", "type") == "default" )
                    option->addOption(aux, true);
                else
                    option->addOption(aux);

            }
            xml.releaseDomain("option");
            extraInfos.append(option, option->getOptionType());
        }
        xml.releaseDomain("options");
    }
    //the elements should be deleted only at the list
    //but should be removed from both.

    //save the domain.

    count = xml.howManyTags("product");
    for (i=0; i < count; i++){
        xml.setDomain("product["+QString::number(i)+"]");
        prod = new RelatedProductGroup(xml);
        append(prod);
        xml.releaseDomain("product", false);
    }
    xml.popDomain();
    return true;
}

QString RelatedProductGroup::baseName() const{
    return base_product_name;
}

void RelatedProductGroup::setProductName(const QString& _name){
    base_product_name = _name;
}

//make a reset to all the products and then
//delete all the products
void RelatedProductGroup::reset(){
    for( auto item : relatedGroup)  item->reset();
    relatedGroup.clear();
    setProductName(UNKNOWN);
}

QStringList RelatedProductGroup::getCombinations() const{
    QStringList combination;
    for (auto item : relatedGroup)
        combination.append(item->baseName());
    return combination;
}

//append a product at the dict and the lists
void RelatedProductGroup::append(RelatedProductGroup *relative){
    relatedGroup.append(relative, relative->baseName());
}

//the number of actual products at the list
int RelatedProductGroup::count() const{
    return relatedGroup.count();
}

//get the product at position pos from the list or nullptr if does not exist that position
RelatedProductGroup* RelatedProductGroup::getProductAt(int pos) const{
    if ( pos > relatedGroup.count() || pos < 0 )  return nullptr;
    return relatedGroup.at(pos);
}

//get a product from the list
RelatedProductGroup* RelatedProductGroup::getProduct(const QString& name) const{
    return relatedGroup.find(name);
}

void RelatedProductGroup::removeProductAt(const int pos){
    if ( pos > relatedGroup.count() || pos < 0) return;
    RelatedProductGroup *prod = relatedGroup.at(pos);
    prod->reset();
    relatedGroup.remove(pos);
}

bool RelatedProductGroup::removeProduct(const QString& name){
    int count,i;
    RelatedProductGroup *prod=0;
    RelatedProductGroup *prod_aux=0;
    QString aux;

    prod = relatedGroup.find(name);
    if (!prod)
        return false;

    aux = prod->baseName();

    count = relatedGroup.count();
    for (i=0;i<count;i++){
        prod_aux = relatedGroup.at(i);
        if (prod==prod_aux){
            relatedGroup.remove(i);
            break;
        }
    }
    return true;
}
