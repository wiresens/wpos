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

#include <wposwidget/global.h>

#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include <wposcore/hlist.h>
#include <optionnode.h>
#include <wposwidget/effects.h>

#include <QApplication>
#include <QWidget>
#include <QFile>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QtGlobal>

#include <iostream>

const QString& END_METAPRODUCT = "solo";

using namespace std;



/**
*    this constructor create a list with id, and empty lists.
*/

Product::Product(const QString& product_name,
                 QWidget *parent):

    QToolButton(parent),
    product_list { new QList<Product*>},
    options_list {new HList<OptionNode>}
{
    setObjectName(product_name);
    setProductName(product_name);
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::REREAD_CONFIG, this);

    options_list->setAutoDelete(false);
}

Product::Product(const QString& product_name,
                 XmlConfig *xml,
                 QWidget *parent):

    Product(product_name, parent)
{
    initProduct(product_name , xml);
}

Product::Product(const QString& product_name,
                 const QString& file_name,
                 QWidget *parent):

    Product(product_name, parent)
{
    //check if the file exists
    XmlConfig xml (file_name);
    if ( !QFile::exists(file_name) || !xml.isValid() )
        setProductName(UNKNOWN);  // And throw
    else
        initProduct(product_name, &xml);
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

bool  Product::initProduct(const QString& productName, XmlConfig *xml){
    QString aux;
    int count, i;
    bool found;
    ProductData *product_data=0;
    OptionNode *option=0;

    setToggleFilter(true);
    prepareSignals();
    setButtonAspect();

    xml->pushDomain();
    xml->delDomain();

    if (productName.isEmpty()){
        xml->popDomain();
        return false;
    }

    setProductName(productName);

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

        if (aux == getProductName()){
            found = true;
            xml->setDomain("product["+QString::number(i)+"]");
            break;
        }
    }

    XmlConfig *aux_xml = new XmlConfig();
    aux_xml->createElement("type", "Productos");
    aux_xml->createElement("first_id", productName);
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
        setTextInPixmap(aux == "true");
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
        for(i=0; i<count; i++){
            xml->setDomain("option["+QString::number(i)+"]");
            aux = xml->readString("type");
            option = new OptionNode(aux);
            for(int j=0; j < xml->howManyTags("value");j++){
                aux = xml->readString("value["+QString::number(j)+"]");
                if (xml->readAttribute("value["+QString::number(j)+"]","type")=="default")
                    option->addOption(aux, true);
                else
                    option->addOption(aux);

            }
            xml->releaseDomain("option");
            options->append( option, option->getOptionType() );
        }
        xml->releaseDomain("options");
    }
    count = xml->howManyTags("product");
    for ( i = 0; i < count; i++){
        xml->setDomain("product["+QString::number(i)+"]");
        product_data = new ProductData(xml);

        //add the product to the list and qdict
        append(product_data);
        xml->releaseDomain("product",false);
    }
    xml->delDomain();
    xml->popDomain();
    return true;
}

bool Product::setProductName(const QString& _name){
    product_name = _name;
    setTextAtButton();
    return true;
}

void Product::setTextAtButton(){
    QString text;
    BarCoreDB *db=0;

    db = new BarCoreDB("nTPV_ProductCoreConnection", Files::configFilePath("database"));
    if (!db->connect()){
        text = getProductName();
        text.replace('_',"\n");
        prod_name = text.toUpper();
        setText(prod_name);
        return;
    }

    prod_name = db->getName(getProductName());
    db->disConnect();
    delete db;

    if (prod_name.isEmpty()){
        text = getProductName();
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

bool Product::setProductPixmap(const QString& file_name){
    return setProductPixmap( QPixmap(file_name) ) ;
}

bool Product::setProductPixmap(const QPixmap& pixmap){
    my_pixmap = pixmap;
    setIcon(pixmap);
    setIconSize(pixmap.rect().size());
    has_pixmap = true;
    return true;
}

bool Product::setTextInPixmap(bool on){

    QPixmap pix = my_pixmap;
    if (pix.isNull())
        return false;
    if (on){
        QFont qfont = QApplication::font();
        if (!family.isEmpty()) qfont.setFamily(family);
        qfont.setPointSize(default_font_size);
        qfont.setBold(true);
        Effects::drawTextInPixmap(prod_name, &pix, qfont);
        setIcon(pix);
    }
    else
        setIcon(my_pixmap);
    return true;
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
    connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleChanged(bool)));
}

void Product::setToggleFilter(const bool& filter){
    toggle_filter = filter;
}

bool Product::getToggleFilter(){
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
            disconnect(SIGNAL(combineWith(QStringList )));
            disconnect(SIGNAL(productDefinition(XmlConfig *)));
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
    options_list->clear();
    for (int i=0;i<(int)productData->options->count();i++){
        options_list->append(productData->options->at(i),(productData->options->at(i))->getOptionType());
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
    disconnect(SIGNAL(combineWith(QStringList )));
    disconnect(SIGNAL(productDefinition(XmlConfig *)));
    return true;
}

void Product::combinationClicked(Product *product){
    ProductData *data = 0;
    QStringList list;
    XmlConfig *xml = 0;
    //OptionNode *on=0;

    if (!productData) return;

    data =  productData->getProduct(product->getProductName());
    if (!data) return;

    productData = data;
    product_list->append(product);

    for (int i=0;i<(int)productData->options->count();i++){
        //check if the option is at the list
        if (!options_list->find((productData->options->at(i))->getOptionType()))
            options_list->append(productData->options->at(i),(productData->options->at(i))->getOptionType());
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
    disconnect(SIGNAL(combineWith(QStringList )));
    disconnect(SIGNAL(productDefinition(XmlConfig *)));
}

void Product::combinationDeleted(Product *product){
    int count, i;
    bool founded;
    QStringList  list;
    Product *prod = 0;
    founded = false;
    count = product_list->count();
    for(i=0;i<count;i++){
        if (product==product_list->at(i)){
            founded = true;
            break;
        }
    }
    if (!founded)
        return;

    //all the buttons after the i product and also the i product should be detoggled
    while ((prod = product_list->at(i))){
        //may be we will have some problems with these.
        prod->setToggleFilter(true);  //needed to deactivate the emision of the productDeleted Signal

        //@benes prod->setOn(false);

        prod->setToggleFilter(false);
        //        product_list->remove(product_list->at(i));
        product_list->removeAt(i);
    }

    //now we should put the node(ProductData) to the last ProductData of the product_list.
    productData = this;
    options_list->clear();
    for (int i=0;i<(int)productData->options->count();i++){
        options_list->append(productData->options->at(i),(productData->options->at(i))->getOptionType());
    }
    count = product_list->count();
    for(i=0;i<count;i++){
        prod = product_list->at(i);
        productData = productData->getProduct(prod->getProductName());
        for (int i=0;i<(int)productData->options->count();i++){
            options_list->append(productData->options->at(i),(productData->options->at(i))->getOptionType());
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
    //    if (this->isOn())
    //        return;
    if (this->isChecked())
        return;
    this->setEnabled(false);
}

void Product::enableProduct(){
    this->setEnabled(true);
}

void Product::defaultValues(){
    productData = nullptr;
    product_list->clear();
    setToggleFilter(true);
    setEnabled(true);
    //@BENE    this->setOn(false);
    setToggleFilter(false);
}

/*************************************************************************************************************
* all the method that access the productData nodes should also check the options for all the data...
**************************************************************************************************************/
XmlConfig* Product::createProductDefinition(){
    int i,count;
    XmlConfig *xml = 0;
    Product *prod = 0;
    QStringList list;
    QString aux;
    if (!productData)
        return xml;

    //write all products at the xml
    //first get the list.
    if ( getProductName() == END_METAPRODUCT){
        options_list->clear();
        return nullptr;
    }
    list.append(this->getProductName());
    count = product_list->count();
    for(i=0;i<count;i++){
        prod = product_list->at(i);
        aux = prod->getProductName();
        if (aux == END_METAPRODUCT)
            break;
        list.append(prod->getProductName());
    }

    if (list.isEmpty()){     //this only happens when the only product was the END_METAPRODUCT
        return xml;
    }

    list.sort();

    //now write the XML
    xml = new XmlConfig("/tmp/producto_temporal.xml");
    xml->delDomain();

    //delete all products
    while (xml->howManyTags("product")){
        xml->deleteElement("product");
    }

    xml->createElementSetDomain("product");
    //prepare the option section
    //testing purpose
    for (i=0;i<(int)options_list->count();i++){
        OptionNode *node = 0;
        QString option_value="";
        QString option="";
        node = options_list->at(i);
        option = node->getOptionType();
        option_value = node->getDefaultOption();
        if ((option.isEmpty())||(option_value.isEmpty()))
            continue;
        xml->createElement("options.option["+QString::number(i)+"].type",option);
        xml->createElement("options.option["+QString::number(i)+"].value",option_value);
    }

    xml->createElementSetDomain("articles");
    count = list.count();
    for (i=0;i<count;i++){
        xml->createElement("article["+QString::number(i)+"].name",list[i]);
    }
    xml->save();
    xml->delDomain();

    this->defaultValues();
    return xml;
}
