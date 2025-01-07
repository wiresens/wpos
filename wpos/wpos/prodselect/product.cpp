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
#include <QDir>

const QString& END_META_PRODUCT = "solo";
const QString UNKNOWN {"STRANGER"};

using namespace std;

/**
*    this constructor create a list with id, and empty lists.
*/

BarCoreDB Product::m_bar_core_db{"ProductConnection", cfg::xmlFileByKey(cfg::XMLKey::Database)};

Product::Product(
    const QString& name,
    QWidget *parent):
    QToolButton(parent)
{
    setObjectName(name);
    setProductName(name);

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::REREAD_CONFIG, this);

    m_toggle_filter = true;

    resize(MAX_SIZE);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setPalette(QPalette(Colors::PRODUCT_BG_COLOR));
    setBackgroundRole(QPalette::Button);
    setCheckable(true);

    connect(this, &Product::toggled, this, &Product::toggleChanged);
}

Product::Product(
    const QString& name,
    XmlConfig &xml,
    QWidget *parent)
    :Product(name, parent)
{
    initProduct(name , xml);
}

Product::Product(
    const QString& name,
    const QString& xmlfile,
    QWidget *parent)
    :Product(name, parent)
{
    //check if the file exists
    XmlConfig xml (xmlfile);
    if ( !QFile::exists(xmlfile) || !xml.wellFormed() )
        setProductName(UNKNOWN);  // And throw
    else
        initProduct(base_product_name, xml);
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
    XmlConfig &xml)
{
    QString aux;
    int count, i;
    bool found;
    RelatedProductGroup *product_data=0;
    ProductExtraInfo *option=0;

    xml.pushDomain();
    xml.delDomain();

    if (name.isEmpty()){
        xml.popDomain();
        return false;
    }

    setProductName(name);

    if (!xml.setDomain("products")){
        xml.popDomain();
        return false;
    }

    found = false;
    //foreach product check if it's name is the productName
    count = xml.howManyTags("product");
    for(i=0; i < count; i++){
        aux = xml.readString("product["+ QString::number(i)+"].name");
        if (aux.isEmpty()) continue;

        if (aux == baseName()){
            found = true;
            xml.setDomain("product["+QString::number(i)+"]");
            break;
        }
    }

    XmlConfig tmp_xml; //use stack allocated
    tmp_xml.createElement("type", "product");
    tmp_xml.createElement("first_id", name);
    emit genericDataSignal(GDATASIGNAL::REREAD_CONFIG, &tmp_xml);

    if (!found){
        xml.popDomain();
        return false;
    }

    //check if there is a pixmap tag
    if ( xml.howManyTags("icon") ){
        aux = xml.readString("icon");
        if (!aux.isEmpty()){
            setProductPixmap(aux);
        }
    }

    if ( xml.howManyTags("showtext")){
        aux = xml.readString("showtext");
        aux = aux.toLower();
        setTextInPixmap( aux == "true");
    }
    if (xml.howManyTags("fontsize")){
        int font_size = 0;
        bool ok = false;
        QString f_size;

        f_size = xml.readString("fontsize");
        font_size = f_size.toInt(&ok);
        if (ok) setDefaultFontSize(font_size);
    }

    if (xml.setDomain("options")){
        count = xml.howManyTags("option");

        for(i=0; i< count; i++){
            xml.setDomain("option["+QString::number(i)+"]");
            auto optionType = xml.readString("type");
            option = new ProductExtraInfo(optionType);

            for(int j=0; j < xml.howManyTags("value"); j++){
                optionType = xml.readString("value["+QString::number(j)+"]");
                if (xml.readAttribute("value["+QString::number(j)+"]","type") == "default")
                    option->addOption(optionType, true);
                else
                    option->addOption(optionType);

            }
            xml.releaseDomain("option");
            extraInfos.append( option, option->getOptionType() );
        }
        xml.releaseDomain("options");
    }
    count = xml.howManyTags("product");
    for ( i = 0; i < count; i++){
        xml.setDomain("product["+QString::number(i)+"]");
        product_data = new RelatedProductGroup(xml);

        //add the product to the list and qdict
        append(product_data);
        xml.releaseDomain("product",false);
    }
    xml.delDomain();
    xml.popDomain();
    return true;
}

void Product::setProductName(const QString& _name){
    base_product_name = _name;
    setTextAtButton();
}

void Product::setTextAtButton(){
    QString text;

    if (!m_bar_core_db.isConnected()){
        text = baseName();
        text.replace('_',"\n");
        m_prod_name = text.toUpper();
        setText(m_prod_name);
        return;
    }

    m_prod_name = m_bar_core_db.getName(baseName());

    if (m_prod_name.isEmpty()){
        text = baseName();
        text.replace('_',"\n");
        m_prod_name = text.toUpper();
        setText(m_prod_name);
        return;
    }

    m_prod_name.replace(' ',"\n");
    m_prod_name = m_prod_name.toUpper();
    setText(m_prod_name);
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
    return setProductPixmap( QPixmap("products:" + file_name) ) ;
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
        if (!m_prod_family.isEmpty()) qfont.setFamily(m_prod_family);
        qfont.setPointSize(m_font_size);
        qfont.setBold(true);
        Effects::drawTextInPixmap(m_prod_name, &pixmap, qfont);
        setIcon(pixmap);
    }
}

void Product::setDefaultFontFamily(const QString& _family){
    if (!_family.isEmpty())
        m_prod_family = _family;
}

void Product::setDefaultFontSize(int num){
    if ((num>0) && (num<20))
        m_font_size = num;
}

void Product::setDefaultTextFontFamily(const QString& _family){
    QFont font;
    if (m_prod_family.isEmpty())
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
    m_toggle_filter = filter;
}

bool Product::getToggleFilter() const{
    return m_toggle_filter;
}

void Product::toggleChanged(bool pressed){
    if (m_toggle_filter) return;

    if (pressed){
        qApp->processEvents();
        emit productClicked(this);
    }
    else{
        if (m_prod_data){
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
    if (m_prod_data)
        return false;
    //     cout << "Producto " << getProductName().toLatin1() << " seleccionado para LIDERAR" << endl;

    m_prod_data = this;

    //prepare the option section. (append the node-sections)
    m_prod_extra_infos.clear();
    for (int i=0;i<(int)m_prod_data->extraInfos.count();i++){
        m_prod_extra_infos.append(m_prod_data->extraInfos.at(i),(m_prod_data->extraInfos.at(i))->getOptionType());
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

    if (!m_prod_data) return;

    data =  m_prod_data->getProduct(product->baseName());
    if (!data) return;

    m_prod_data = data;
    m_prod_variants.append(product);

    for (int i=0;i<(int)m_prod_data->extraInfos.count();i++){
        //check if the option is at the list
        if (!m_prod_extra_infos.find((m_prod_data->extraInfos.at(i))->getOptionType()))
            m_prod_extra_infos.append(m_prod_data->extraInfos.at(i),(m_prod_data->extraInfos.at(i))->getOptionType());
    }

    list = m_prod_data->getCombinations();
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
    count = m_prod_variants.count();
    for(i=0;i<count;i++){
        if (product==m_prod_variants.at(i)){
            founded = true;
            break;
        }
    }
    if (!founded)
        return;

    //all the buttons after the i product and also the i product should be detoggled
    while ((prod = m_prod_variants.at(i))){
        //may be we will have some problems with these.
        prod->setToggleFilter(true);  //needed to deactivate the emision of the productDeleted Signal

        //@benes prod->setOn(false);

        prod->setToggleFilter(false);
        //        product_list->remove(product_list->at(i));
        m_prod_variants.removeAt(i);
    }

    //now we should put the node(ProductData) to the last ProductData of the product_list.
    m_prod_data = this;
    m_prod_extra_infos.clear();
    for (int i=0;i<(int)m_prod_data->extraInfos.count();i++){
        m_prod_extra_infos.append(m_prod_data->extraInfos.at(i),(m_prod_data->extraInfos.at(i))->getOptionType());
    }
    count = m_prod_variants.count();
    for(i=0;i<count;i++){
        prod = m_prod_variants.at(i);
        m_prod_data = m_prod_data->getProduct(prod->baseName());
        for (int i=0;i<(int)m_prod_data->extraInfos.count();i++){
            m_prod_extra_infos.append(m_prod_data->extraInfos.at(i),(m_prod_data->extraInfos.at(i))->getOptionType());
        }
    }

    list = m_prod_data->getCombinations();
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
    m_prod_data = nullptr;
    m_prod_variants.clear();
    setToggleFilter(true);
    setEnabled(true);
    //@BENE    this->setOn(false);
    setToggleFilter(false);
}

/*************************************************************************************************************
* all the method that access the m_prod_data nodes should also check the options for all the data...
**************************************************************************************************************/
XmlConfig* Product::createProductDefinition()
{
    if (!m_prod_data) return nullptr;

    //write all products in the xml, first get the list.
    if ( baseName() == END_META_PRODUCT){
        m_prod_extra_infos.clear();
        return nullptr;
    }

    QStringList names;
    names.append(baseName());
    for(auto* prod : m_prod_variants ){
        QString tmp = prod->baseName();
        if (tmp == END_META_PRODUCT) break;
        names.append(prod->baseName());
    }

    //this only happens when the only product was the END_METAPRODUCT
    if (names.isEmpty()) return nullptr;
    names.sort();

    //now write the XML
    auto xml = new XmlConfig("tmps:ordered_product.xml");
    while (xml->howManyTags("product"))
        xml->deleteElement("product");

    xml->createElementSetDomain("product");
    for ( auto i=0; i < m_prod_extra_infos.count(); i++){
        ProductExtraInfo *node = m_prod_extra_infos.at(i);
        auto option = node->getOptionType();
        auto option_value = node->getDefaultOption();
        if ( option.isEmpty() || option_value.isEmpty() ) continue;

        xml->createElement("options.option["+QString::number(i)+"].type",option);
        xml->createElement("options.option["+QString::number(i)+"].value",option_value);
    }

    xml->createElementSetDomain("articles");
    for (auto i=0; i < names.count(); i++)
        xml->createElement( "article["+QString::number(i)+"].name", names[i] );

    xml->save();
    defaultValues();
    return xml;
}
