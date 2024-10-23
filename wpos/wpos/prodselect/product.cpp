/***************************************************************************
                          product.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "product.h"

#include "database/barcoredb.h"

#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include <wposcore/hlist.h>
#include <wposcore/productextrainfo.h>
#include <wposgui/common/global.h>
#include <wposgui/common/effects.h>

#include <QApplication>
#include <QWidget>
#include <QFile>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QtGlobal>

#include <iostream>

const QString& END_METAPRODUCT = "solo";
const QString UNKNOWN {"STRANGER"};

using namespace std;

/**
*    this constructor create a list with id, and empty lists.
*/

BarCoreDB Product::db{"ProductConnection", cfg::xmlFileByKey(cfg::XMLKey::Database)};

Product::Product(
    const QString& name,
    QWidget *parent):
    QToolButton(parent)
{
    setObjectName(name);
    setProductName(name);

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::REREAD_CONFIG, this);

    toggle_filter = true;

    resize(MAX_SIZE);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setPalette(QPalette(Colors::PRODUCT_BG_COLOR));
    setBackgroundRole(QPalette::Button);
    setCheckable(true);

    connect(this, &Product::toggled, this, &Product::toggleChanged);
}

Product::Product(
    const QString& name,
    XmlConfig *xmlDescription,
    QWidget *parent):

    Product(name, parent)
{
    initProduct(name , xmlDescription);
}

Product::Product(
    const QString& name,
    const QString& xmlDescriptionFile,
    QWidget *parent):

    Product(name, parent)
{
    //check if the file exists
    XmlConfig xml (xmlDescriptionFile);
    if ( !QFile::exists(xmlDescriptionFile) || !xml.wellFormed() )
        setProductName(UNKNOWN);  // And throw
    else
        initProduct(base_product_name, &xml);
}

Product::~Product(){
    reset();
}

/**
 * It Makes the real init and contructor for the object, It tries to load the list of combinations
 * from the XML and construct all the articles tree. It's done in the following way:
 * First we search the Product at the XML , if it doesn't exist, the init returns false;
 * If it exists at the XML it gather all the information to construct to data structs
 * one is the QPtrList where the data is at, for real, the second is a QMap wich is used to access
 * the QPtrList with hashed tables (implemented at QT Level)
 * @return true if the product has an equivalence at the XML and it has configured all the data
 * structs
 * @param QString with the name and identifier of this product.
 * @param XmlConfig wich has all the configuration of the products
 */

bool  Product::initProduct(
    const QString& name,
    XmlConfig *xml)
{
    QString aux;
    int count, i;
    bool found;
    RelatedProductGroup *product_data=0;
    ProductExtraInfo *option=0;

    xml->pushDomain();
    xml->delDomain();

    if (name.isEmpty()){
        xml->popDomain();
        return false;
    }

    setProductName(name);

    if (!xml->setDomain("products")){
        xml->popDomain();
        return false;
    }

    found = false;
    //foreach product check if it's name is the productName
    count = xml->howManyTags("product");
    for(i=0; i < count; i++){
        aux = xml->readString("product["+ QString::number(i)+"].name");
        if (aux.isEmpty()) continue;

        if (aux == baseName()){
            found = true;
            xml->setDomain("product["+QString::number(i)+"]");
            break;
        }
    }

    XmlConfig *aux_xml = new XmlConfig(); //use stack allocated
    aux_xml->createElement("type", "Productos");
    aux_xml->createElement("first_id", name);
    emit genericDataSignal(GDATASIGNAL::REREAD_CONFIG, aux_xml);
    delete aux_xml;

    if (!found){
        xml->popDomain();
        return false;
    }

    //check if there is a pixmap tag
    if ( xml->howManyTags("icon") ){
        aux = xml->readString("icon");
        if (!aux.isEmpty()){
            setProductPixmap(aux);
        }
    }

    if ( xml->howManyTags("showtext")){
        aux = xml->readString("showtext");
        aux = aux.toLower();
        setTextInPixmap( aux == "true");
    }
    if (xml->howManyTags("fontsize")){
        int font_size = 0;
        bool ok = false;
        QString f_size;

        f_size = xml->readString("fontsize");
        font_size = f_size.toInt(&ok);
        if (ok) setDefaultFontSize(font_size);
    }

    if (xml->setDomain("options")){
        count = xml->howManyTags("option");

        for(i=0; i< count; i++){
            xml->setDomain("option["+QString::number(i)+"]");
            auto optionType = xml->readString("type");
            option = new ProductExtraInfo(optionType);

            for(int j=0; j < xml->howManyTags("value"); j++){
                optionType = xml->readString("value["+QString::number(j)+"]");
                if (xml->readAttribute("value["+QString::number(j)+"]","type") == "default")
                    option->addOption(optionType, true);
                else
                    option->addOption(optionType);

            }
            xml->releaseDomain("option");
            extraInfos.append( option, option->getOptionType() );
        }
        xml->releaseDomain("options");
    }
    count = xml->howManyTags("product");
    for ( i = 0; i < count; i++){
        xml->setDomain("product["+QString::number(i)+"]");
        product_data = new RelatedProductGroup(xml);

        //add the product to the list and qdict
        append(product_data);
        xml->releaseDomain("product",false);
    }
    xml->delDomain();
    xml->popDomain();
    return true;
}

void Product::setProductName(const QString& _name){
    base_product_name = _name;
    setTextAtButton();
}

void Product::setTextAtButton(){
    QString text;

    if (!db.isConnected()){
        text = baseName();
        text.replace('_',"\n");
        prod_name = text.toUpper();
        setText(prod_name);
        return;
    }

    prod_name = db.getName(baseName());

    if (prod_name.isEmpty()){
        text = baseName();
        text.replace('_',"\n");
        prod_name = text.toUpper();
        setText(prod_name);
        return;
    }

    prod_name.replace(' ',"\n");
    prod_name = prod_name.toUpper();
    setText(prod_name);
}

void Product::paintEvent(QPaintEvent* event){
    //    QStylePainter painter(this);
    //    QStyleOptionToolButton opt;
    //    initStyleOption(&opt);

    //    QRect r{opt.rect};
    //    int h = r.height();
    //    int w = r.width();
    //    int iconSize = qMax(qMin(h,w) - 2 * pad, minSize);
    //    opt.iconSize = QSize(iconSize, iconSize);
    //    painter.drawControl(QStyle::CE_ToolButtonLabel, opt);
    QToolButton::paintEvent(event);
}

void Product::resizeEvent(QResizeEvent* event){
    //    setIconSize(rect().size());
    QToolButton::resizeEvent(event);
}

bool Product::setProductPixmap(
    const QString& file_name)
{
    return setProductPixmap( QPixmap(file_name) ) ;
}

bool Product::setProductPixmap(
    const QPixmap& pixmap)
{
    setIcon(pixmap);
    setIconSize(pixmap.rect().size());
    return true;
}

void Product::setTextInPixmap(bool on){
    QPixmap pixmap = icon().pixmap(iconSize());

    if ( !pixmap.isNull() && on){
        QFont qfont = QApplication::font();
        if (!family.isEmpty()) qfont.setFamily(family);
        qfont.setPointSize(default_font_size);
        qfont.setBold(true);
        Effects::drawTextInPixmap(prod_name, &pixmap, qfont);
        setIcon(pixmap);
    }
}

void Product::setDefaultFontFamily(const QString& _family){
    if (!_family.isEmpty())
        family = _family;
}

void Product::setDefaultFontSize(int num){
    if ((num>0) && (num<20))
        default_font_size = num;
}

void Product::setDefaultTextFontFamily(const QString& _family){
    QFont font;
    if (family.isEmpty())
        return;
    font = this->font();
    font.setFamily(_family);
    setFont(font);
    setText(text());
    //        if (!has_pixmap)
    //                setText(prod_name);
}

void Product::setDefaultTextFontSize(int num){
    QFont font;
    if (num<=0)
        return;
    font = this->font();
    font.setPointSize(num);
    setFont(font);
    setText(text());
    //        if (!has_pixmap)
    //                setText(prod_name);
}

bool Product::setButtonAspect(){
    resize(MAX_SIZE);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setPalette(QPalette(Colors::PRODUCT_BG_COLOR));
    setBackgroundRole(QPalette::Button);
    setCheckable(true);
    return true;
}

void Product::prepareSignals(){
    connect(this, &Product::toggled, this, &Product::toggleChanged);
}

void Product::setToggleFilter(const bool& filter){
    toggle_filter = filter;
}

bool Product::getToggleFilter() const{
    return toggle_filter;
}

void Product::toggleChanged(bool pressed){
    if (toggle_filter) return;

    if (pressed){
        qApp->processEvents();
        emit productClicked(this);
    }
    else{
        if (productData){
            emit productDefinition(nullptr);
            //disconnecting the signals that create the product.
            disconnect(this, &Product::combineWith, nullptr, nullptr);
            disconnect(this, &Product::productDefinition, nullptr, nullptr);
        }
        else{
            emit deleteProduct(this);
        }
    }
}

bool Product::lead(){
    XmlConfig *xml = 0;
    QStringList list;
    if (productData)
        return false;
    //     cout << "Producto " << getProductName().toLatin1() << " seleccionado para LIDERAR" << endl;

    productData = this;

    //prepare the option section. (append the node-sections)
    productExtraInfos.clear();
    for (int i=0;i<(int)productData->extraInfos.count();i++){
        productExtraInfos.append(productData->extraInfos.at(i),(productData->extraInfos.at(i))->getOptionType());
    }

    list = this->getCombinations();
    if (!list.isEmpty()){
        //it's not the first and the only object.
        emit combineWith(list);
        return false;
    }

    //it's the first and last so the productDefinition should be given
    xml = createProductDefinition();
    emit productDefinition(xml);
    delete xml;
    xml = 0;

    //disconnecting the signals that create the product.
    disconnect(this, &Product::combineWith, nullptr, nullptr);
    disconnect(this, &Product::productDefinition, nullptr, nullptr);
    return true;
}

void Product::combinationClicked(Product *product){
    RelatedProductGroup *data = 0;
    QStringList list;
    XmlConfig *xml = 0;
    //OptionNode *on=0;

    if (!productData) return;

    data =  productData->getProduct(product->baseName());
    if (!data) return;

    productData = data;
    productVariants.append(product);

    for (int i=0;i<(int)productData->extraInfos.count();i++){
        //check if the option is at the list
        if (!productExtraInfos.find((productData->extraInfos.at(i))->getOptionType()))
            productExtraInfos.append(productData->extraInfos.at(i),(productData->extraInfos.at(i))->getOptionType());
    }

    list = productData->getCombinations();
    if (!list.isEmpty()){
        //if it's not the last element of the list
        emit combineWith(list);
        return;
    }

    //else, it's the last item of the list...
    //it's the first and last so the productDefinition should be given
    xml = createProductDefinition();
    emit productDefinition(xml);
    delete xml;
    xml = 0;

    //disconnecting the signals that create the product.
    disconnect(this, &Product::combineWith, nullptr, nullptr);
    disconnect(this, &Product::productDefinition, nullptr, nullptr);
}

void Product::combinationDeleted(Product *product){
    int count, i;
    bool founded;
    QStringList  list;
    Product *prod = 0;
    founded = false;
    count = productVariants.count();
    for(i=0;i<count;i++){
        if (product==productVariants.at(i)){
            founded = true;
            break;
        }
    }
    if (!founded)
        return;

    //all the buttons after the i product and also the i product should be detoggled
    while ((prod = productVariants.at(i))){
        //may be we will have some problems with these.
        prod->setToggleFilter(true);  //needed to deactivate the emision of the productDeleted Signal

        //@benes prod->setOn(false);

        prod->setToggleFilter(false);
        //        product_list->remove(product_list->at(i));
        productVariants.removeAt(i);
    }

    //now we should put the node(ProductData) to the last ProductData of the product_list.
    productData = this;
    productExtraInfos.clear();
    for (int i=0;i<(int)productData->extraInfos.count();i++){
        productExtraInfos.append(productData->extraInfos.at(i),(productData->extraInfos.at(i))->getOptionType());
    }
    count = productVariants.count();
    for(i=0;i<count;i++){
        prod = productVariants.at(i);
        productData = productData->getProduct(prod->baseName());
        for (int i=0;i<(int)productData->extraInfos.count();i++){
            productExtraInfos.append(productData->extraInfos.at(i),(productData->extraInfos.at(i))->getOptionType());
        }
    }

    list = productData->getCombinations();
    if (list.isEmpty()){
        // ??????????????????????????????
        // this should never happen
        return;
    }
    emit combineWith(list);
    return;
}

void Product::disableProduct(){
    //it should only be disabled if the object is not toggled
    //    if (this->isOn())  return;
    if (isChecked()) return;
    setEnabled(false);
}

void Product::enableProduct(){
    setEnabled(true);
}

void Product::defaultValues(){
    productData = nullptr;
    productVariants.clear();
    setToggleFilter(true);
    setEnabled(true);
    //@BENE    this->setOn(false);
    setToggleFilter(false);
}

/*************************************************************************************************************
* all the method that access the productData nodes should also check the options for all the data...
**************************************************************************************************************/
XmlConfig* Product::createProductDefinition()
{
    int i,count;
    XmlConfig *xml = 0;
    Product *prod = 0;
    QStringList names;
    QString aux;

    if (!productData) return nullptr;

    //write all products in the xml
    //first get the list.
    if ( baseName() == END_METAPRODUCT){
        productExtraInfos.clear();
        return nullptr;
    }

    names.append(baseName());
    count = productVariants.count();
    for(i=0; i < count; i++){
        prod = productVariants.at(i);
        aux = prod->baseName();
        if (aux == END_METAPRODUCT) break;
        names.append(prod->baseName());
    }

    //this only happens when the only product was the END_METAPRODUCT
    if (names.isEmpty()) return xml;


    names.sort();

    //now write the XML
    xml = new XmlConfig("producto_temporal.xml");
    xml->delDomain();

    //delete all products
    while (xml->howManyTags("product"))
        xml->deleteElement("product");

    xml->createElementSetDomain("product");
    //prepare the option section
    //testing purpose
    for (i=0; i<(int)productExtraInfos.count();i++){
        ProductExtraInfo *node = 0;
        QString option_value;
        QString option;
        node = productExtraInfos.at(i);
        option = node->getOptionType();
        option_value = node->getDefaultOption();

        if ((option.isEmpty())||(option_value.isEmpty())) continue;
        xml->createElement("options.option["+QString::number(i)+"].type",option);
        xml->createElement("options.option["+QString::number(i)+"].value",option_value);
    }

    xml->createElementSetDomain("articles");
    count = names.count();

    for (i=0;i<count;i++)
        xml->createElement("article["+QString::number(i)+"].name",names[i]);

    xml->save();
    xml->delDomain();

    defaultValues();
    return xml;
}
