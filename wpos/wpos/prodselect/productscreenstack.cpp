/***************************************************************************
                          screenstack.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "prodselect/productscreenstack.h"
#include <xmlconfig.h>
#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>

#include <QFile>
#include <QLayout>
#include <iostream>
#include <QTimer>

using namespace std;

//Normally the constructors  should throw if unable to read the config files
// Presently we are not throwing but will add that when everything works
//We will also find a way to factor out the constructors by using forwarding constructors.

// ProductScreenStack::ProductScreenStack(
//     const QString& xmlDescriptionFile,
//     QWidget *parent,
//     const QString& name):
//     ProductScreenStack(parent, name)
// {
//     if (!QFile::exists(xmlDescriptionFile))
//         cerr << " Missing file: " << xmlDescriptionFile.toStdString() << "cant be opened to read the screen definitions" << endl;
//     else{
//         XmlConfig xml{xmlDescriptionFile};
//         if (xml.wellFormed()) initScreenStack(&xml);
//     }
// }

// ProductScreenStack::ProductScreenStack(
//     XmlConfig *xml,
//     QWidget *parent,
//     const QString& name):
//     ProductScreenStack(parent, name)
// {
//     if (xml->wellFormed()) initScreenStack(xml);
// }

ProductScreenStack::ProductScreenStack(
    QWidget *parent,
    const QString& name):
    QStackedWidget(parent)
{
    setObjectName(name);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::PRODUCTS_SETPAGE, this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::PRODSELECT_COLOR_MODE, this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::SETCORE_MODE, this);
    gsm->subscribeToGenericSignal(GSIGNAL::LOAD_PRODUCTS, this);
}

ProductScreenStack::~ProductScreenStack(){
    reset();
}

void ProductScreenStack::setDefaultScreenName(const QString &name){
    default_screen = name;
}

QString ProductScreenStack::defaultScreenName(){
    return default_screen;
}

bool ProductScreenStack::setDefaultScreen(){
    if ( defaultScreenName().isEmpty() )
        return false;

    setScreen( defaultScreenName() );
    return true;
}

bool ProductScreenStack::initScreenStack(const QString& xmlDescriptionFile){
    XmlConfig xml (xmlDescriptionFile);
    if ( !QFile::exists(xmlDescriptionFile)  || !xml.wellFormed()){
        cerr << " Missing file: " << xmlDescriptionFile.toStdString()
             << "can't be opened to read the screen definitions" << endl;
        return false;
    }

    return  initScreenStack(&xml);
}

bool ProductScreenStack::initScreenStack(XmlConfig *xml){

    if( !xml) return false;
    screen_dict = new QMap<QString, ProductScreen*>;
    screen_list = new QList<ProductScreen*>;

    xml->popDomain();
    xml->delDomain();

    if (!xml->setDomain("screens")){
        xml->popDomain();
        return false;
    }

    auto tmp = xml->readString("fontfamily");
    if( !tmp.isEmpty() ) fontfamily = tmp;

    tmp = xml->readString("textfontfamily");
    if( !tmp.isEmpty() ) text_fontfamily = tmp;

    tmp = xml->readString("fontsize");

    bool ok {false};
    tmp.toInt(&ok);
    if (ok) fontsize = tmp.toInt(&ok);

    tmp = xml->readString("textfontsize");
    tmp.toInt(&ok);
    if (ok) text_fontsize = tmp.toInt(&ok);

    tmp = xml->readString("textbackgroundcolor");
    if( !tmp.isEmpty() ) text_background_color = tmp;

    for(auto i=0; i < xml->howManyTags("screen"); i++){
        tmp = xml->readString("screen["+QString::number(i)+"].name");
        emit splashRequested(tr("Product Screen : ")+tmp, Qt::AlignBottom | Qt::AlignRight , Qt::darkBlue);
        xml->pushDomain();
        addScreen(tmp, xml);
        xml->popDomain();
    }

    tmp = xml->readString("defaultscreen");
    if( !tmp.isEmpty()  ) default_screen = tmp;

    xml->popDomain();
    return true;
}

int ProductScreenStack::getDefaultSize(){
    return fontsize;
}

QString ProductScreenStack::getDefaultFamily(){
    return fontfamily;
}

int ProductScreenStack::getDefaultTextSize(){
    return text_fontsize;
}

QString ProductScreenStack::getDefaultTextFamily(){
    return text_fontfamily;
}

QString ProductScreenStack::getTextBackgroundColor(){
    return text_background_color;
}

bool ProductScreenStack::reset(){
    ProductScreen *screen = 0;

    while ( screen = qobject_cast<ProductScreen *>( currentWidget() ) ){
        remove(screen->screenName());
    }

    screen_dict->clear();
    screen_list->clear();
    delete screen_list;
    delete screen_dict;
    return true;
}

bool ProductScreenStack::remove(const QString& screen_name){

    ProductScreen *screen = screen_dict->take(screen_name);
    if (!screen)  return false;

    screen_list->removeAll(screen);
    screen->resetScreen();
    removeWidget( screen );
    delete screen;
    return true;
}

void ProductScreenStack::addScreen(const QString& screen_name, XmlConfig* xml){
    ProductScreen *screen = new ProductScreen(screen_name, xml, this);
    addWidget(screen);
    screen_dict->insert(screen->screenName(), screen);
    screen_list->append(screen);
    setCurrentWidget(screen);
}

bool ProductScreenStack::setScreen(const QString& screen_name){
    ProductScreen *screen = screen_dict->value(screen_name);
    if (!screen) return false;

    setCurrentWidget(screen);
    return true;
}

QString ProductScreenStack::nextScreen(){

    //get the visible widget
    ProductScreen *curr_screen = qobject_cast<ProductScreen*>(currentWidget());
    if ( !curr_screen )  return QString("");

    auto i = screen_list->indexOf(curr_screen);
    auto n = screen_list->count();

    ProductScreen *next_screen = screen_list->at( ( i+1) % n );
    setCurrentWidget(next_screen);
    return next_screen->screenName();
}

QString ProductScreenStack::prevScreen(){

    //get the visible widget
    ProductScreen *curr_screen = qobject_cast<ProductScreen*>(currentWidget());
    if ( !curr_screen )  return QString("");

    auto i = screen_list->indexOf(curr_screen);
    auto n = screen_list->count();

    ProductScreen *prev_screen = screen_list->at( (i-1+n) % n );
    setCurrentWidget(prev_screen);
    return prev_screen->screenName();
}

QString ProductScreenStack::visibleScreenName(){

    ProductScreen *screen = qobject_cast<ProductScreen*>(currentWidget());
    if (  !screen)  return QString("");
    return screen->screenName();
}

void ProductScreenStack::setSpecialModeColor(ProductScreen::ModeColor mode){

    color_mode = mode;
    for ( auto screen : *screen_list)
        screen->setSpecialModeColor(color_mode);
}

void ProductScreenStack::disableAll(){

    for ( auto screen : *screen_list)
        screen->disableAll();
}

void ProductScreenStack::defaultValuesAll(){

    for ( auto screen : *screen_list)
        screen->defaultValuesAll();
    emit defaultValue();
}

void ProductScreenStack::productClickedSlot(Product *prod){

    bool completed {false};
    if (!selected_product){
        selected_product = prod;
        //prepare the new connections
        connect(selected_product, &Product::combineWith,
                this, &ProductScreenStack::combineWithSlot);

        connect(selected_product, &Product::productDefinition,
                this, &ProductScreenStack::productDefinitionSlot);

        completed = selected_product->lead();

        //go to the preferred next screen.
        QString screen_name = visibleScreenName();
        if ( defaultScreenName().isEmpty() && !completed)
            last_screen_name = screen_name;

        ProductScreen *screen = screen_dict->value(screen_name);
        if ( screen &&  !completed ){
            screen_name = screen->nextPreferredScreenName();
            setScreen(screen_name);
        }
    }
    else{

        selected_product->combinationClicked(prod);
    }
}

void ProductScreenStack::deleteProductSlot(Product *prod){
    if (!selected_product)
        return;
    else
        selected_product->combinationDeleted(prod);
}

void ProductScreenStack::combineWithSlot(QStringList list){

    disableAll();
    for(ProductScreen *screen : *screen_list) {
        screen->combineWithSlot(list);
    }
}

void ProductScreenStack::productDefinitionSlot(XmlConfig *xml){
    if (xml){
        //all the actions to do when the product has been selected
        emit productDefinition(xml);
    }
    //end of all actions
    emit defaultValue();
    defaultValuesAll();
    selected_product = 0;

    if ((defaultScreenName().isEmpty()) && (!last_screen_name.isEmpty())){
        setScreen(last_screen_name);
        last_screen_name = "";
    }

}

void ProductScreenStack::genericDataSignalSlot(const QString& signal_name, XmlConfig *xml){
    if (signal_name == GDATASIGNAL::PRODUCTS_SETPAGE){
        QString aux;
        xml->pushDomain();
        xml->delDomain();
        aux = xml->readString("name");
        setScreen(aux);
        xml->popDomain();
    }
    else if (signal_name == GDATASIGNAL::PRODSELECT_COLOR_MODE){
        QString aux;
        xml->pushDomain();
        xml->delDomain();
        aux = xml->readString("mode");
        xml->popDomain();
        if (aux == "normal"){
            previous_color_mode = color_mode;
            setSpecialModeColor(ProductScreen::Normal);
        }
        else if (aux == "special"){
            previous_color_mode = color_mode;
            setSpecialModeColor(ProductScreen::Invitation);
        }
        else if (aux == "last")
            setSpecialModeColor(previous_color_mode);
        else if (aux == "toggle"){
            is_color_mode = (!is_color_mode);
            if (is_color_mode)
                setSpecialModeColor(ProductScreen::Normal);
            else
                setSpecialModeColor(color_mode);
        }
    }
    else if (signal_name == GDATASIGNAL::SETCORE_MODE){
        xml->delDomain();
        QString aux=xml->readString("mode");
        if (aux == "normal")
            setSpecialModeColor(ProductScreen::Normal);
        else if (aux == "product_anulation")
            setSpecialModeColor(ProductScreen::ProductAnulation);
        else if (aux == "default_offer")
            setSpecialModeColor(ProductScreen::DefaultOffer);
    }
}

void ProductScreenStack::genericSignalSlot(const QString& signal_name){
    if (signal_name == GSIGNAL::LOAD_PRODUCTS){
        XmlConfig *aux_xml = new XmlConfig(cfg::xmlFileByKey(cfg::XMLKey::Bar));
        reset();
        initScreenStack(aux_xml);
        delete aux_xml;
    }
}
