/***************************************************************************
                          productscreen.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "prodselect/productscreen.h"
#include "prodselect/product.h"
#include "prodselect/productscreenstack.h"
#include <xmlconfig.h>
#include <wposgui/common/global.h>

#include <iostream>
#include <QString>
#include <QLabel>
#include <QLayout>
#include <QFile>

#include <QApplication>

#define SCREEN_DEFAULT_NAME QString("UNKNOWN_SCREEN")
#define DEFAULT_FONT_SIZE 9

using namespace std;

int ProductScreen::SCREEN_PRODUCT_SPACING{0};

//the products should be deleted in the list and only cleared from the dict
//the layout should be defined in the init method
ProductScreen::ProductScreen(
    const QString& screenName,
    XmlConfig *xmlDescription,
    QWidget *parent,
    const QString& name):
    QWidget(parent)
{
    setObjectName(name);
    layout = new QGridLayout(this);
    initScreen(screenName, xmlDescription);
}

ProductScreen::ProductScreen(
    const QString& screenName,
    const QString& xmlDescriptionFile,
    QWidget *parent,
    const QString& name):
    QWidget(parent)
{
    setObjectName(name);
    layout = new QGridLayout(this); //the layout will be defined in the initScreen method

    XmlConfig xml(xmlDescriptionFile);
    if ( !QFile::exists(xmlDescriptionFile) || !xml.wellFormed() ){
        screenName_ = SCREEN_DEFAULT_NAME;
        return;
    }

    initScreen(screenName, &xml);
}

ProductScreen::~ProductScreen(){
    resetScreen();
    delete layout;
    delete productList;
    delete productMap;
}

bool ProductScreen::initScreen(
    const QString& screen_name,
    XmlConfig *xml)
{
    setSpecialModeColor(ProductScreen::Normal);
    screenName_ = screen_name;

    xml->pushDomain();
    xml->delDomain();
    if (!xml->setDomain("screens")){
        xml->popDomain();
        return false;
    }

    QString aux_string;
    bool tag_found {false};
    for ( auto i = 0; i <  xml->howManyTags("screen") ; i++){
        aux_string = xml->readString("screen["+QString::number(i)+"].name");
        if ( aux_string == screenName_ ){
            xml->setDomain("screen["+QString::number(i)+"]");
            tag_found = true;
            break;
        }
    }

    if ( !tag_found ){
        xml->popDomain();
        return false;
    }

    //the screen definition has been found in the XML config file
    //so first get the cols and rows
    resetScreen();

    next_preferred_screen = screenName_;
    if ( xml->howManyTags("nextscreen") ){
        aux_string = xml->readString("nextscreen");
        if ( !aux_string.isEmpty() )
            next_preferred_screen = aux_string;
    }

    if ( xml->howManyTags("cols") ) cols = xml->readInt("cols");
    
    if ( xml->howManyTags("rows") ) rows= xml->readInt("rows");

    aux_string = xml->readString("showtext");

    bool show_text {false};
    if ( aux_string.toLower() == "true" )  show_text = true;

    //The fact that we don't check the pointer before using, means that the parent
    //of ProdcutScreen will always be a ScreenStack. We need to update that in the design diagram.
    auto productScreenStack = qobject_cast<ProductScreenStack *>( parent() );

    auto font_family = xml->readString("fontfamily");
    if ( font_family.isEmpty() )
        font_family = productScreenStack->getDefaultFamily();

    auto text_font_family = xml->readString("textfontfamily");
    if (text_font_family.isEmpty())
        text_font_family = productScreenStack->getDefaultTextFamily();

    auto text_background_color = xml->readString("textbackgroundcolor");
    if (text_background_color.isEmpty())
        text_background_color = productScreenStack->getTextBackgroundColor();

    bool converted {false};
    aux_string = xml->readString("fontsize");
    auto font_size = aux_string.toInt(&converted);

    if ( !converted ) font_size = -1;
    if (font_size ==-1 ) font_size = productScreenStack->getDefaultSize();

    aux_string = xml->readString("textfontsize");
    auto text_font_size = aux_string.toInt(&converted);

    if ( !converted ) text_font_size = -1;
    if (text_font_size == -1) text_font_size = productScreenStack->getDefaultTextSize();

    /*************   LAYOUT PROPERTIES   ***************/
    delete layout;
    layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(ProductScreen::SCREEN_PRODUCT_SPACING);

    //insert all the products in the layout. Here we don't yet manage dynamic number
    //of product to display in the ProductScreen as a respond to a screen resize.
    //The number of product is fixed and determine at this place. We should find a way to adjust that
    //to fit the entire screen as it's size changes. This implies also that the product image size should
    //be able to resize automatically. Maybe we should override the resizeEvent and see how to do that.

    for( auto i=0;  i < xml->howManyTags("product"); i++){

        //get the col and row where the product should be.
        aux_string = xml->readAttribute("product["+QString::number(i)+"]","col");
        auto col = aux_string.toInt(&tag_found);
        if (!tag_found) continue;  // N.a.N.

        aux_string = xml->readAttribute("product["+QString::number(i)+"]","row");
        auto row = aux_string.toInt(&tag_found);
        if (!tag_found) continue;  // N.a.N.

        //check the product
        if ( row >= rows || col >= cols ){
            cerr << "Error in acquisition of buttons: product column != screen column space" << endl;
            //do not add this product.
            continue;
        }

        //prepare the construction of the product.
        auto productName = xml->readString("product["+QString::number(i)+"].name");

        //do not accept empty named products.
        if (productName.isEmpty()) continue; //prepare the reread of the product

        xml->pushDomain();
        Product *product = new Product(productName, xml, this);
        if ( !font_family.isEmpty() )
            product->setDefaultFontFamily(font_family);

        if (font_size !=-1 )
            product->setDefaultFontSize(font_size);

        if (!text_font_family.isEmpty())
            product->setDefaultTextFontFamily(text_font_family);

        if ( text_font_size !=-1 )
            product->setDefaultTextFontSize(text_font_size);

        if ( !text_background_color.isEmpty() ){
            //@benes  prod->setPaletteBackgroundColor(QColor(text_background_color));
            //we thought the folliwing fixed the pb , but it didn't.
            //            QPalette palette(QPalette(QColor(text_background_color) , QPalette::WindowText));
            //            prod->setPalette(palette);
        }

        if (show_text)  product->setTextInPixmap(true);

        xml->popDomain();

        //connect of all signals
        prepareConnects(product);

        layout->addWidget(product, row, col);
        productList->append(product);
        productMap->insert(product->baseName(), product);
    }
    xml->delDomain();
    xml->popDomain();
    return true;
}

bool ProductScreen::resetScreen(){
    setSpecialModeColor(ProductScreen::Normal);
    productList->clear();
    productMap->clear();
    delete layout;
    cols = DEFAULT_COLS;
    rows = DEFAULT_ROWS;
    layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);
    return true;
}

QString ProductScreen::screenName(){
    return screenName_;
}

void ProductScreen::showAll(){

    for( auto product : *productList)
        product->show();

}

void ProductScreen::disableAll(){

    for( auto prod : *productList)
        prod->disableProduct();

}

void ProductScreen::defaultValuesAll(){

    for(auto prod : *productList )
        prod->defaultValues();

    emit defaultValue();
}

void ProductScreen::combineWithSlot(QStringList list){
    int count,i;
    Product *prod=0;

    //enable only the buttons at the list.
    disableAll();

    count = list.count();
    for (i=0;i<count;i++){
        //search at the dict
        prod = productMap->operator [](list[i]);
        if (prod)
            prod->enableProduct();
    }
}

void ProductScreen::productDefinitionSlot(XmlConfig *xml){
    if (xml){
        //all the actions to do when the product has been selected
        emit productDefinition(xml);
    }
    //end of all actions
    this->defaultValuesAll();
    this->selected_product = 0;
}

void ProductScreen::productClickedSlot(Product *prod){
    if (!selected_product){
        selected_product = prod;
        //prepare the new connections
        connect(selected_product, &Product::combineWith,
                this, &ProductScreen::combineWithSlot);
        connect(selected_product, &Product::productDefinition,
                this, &ProductScreen::productDefinitionSlot);
        selected_product->lead();
    }
    else{
        selected_product->combinationClicked(prod);
    }
}

void ProductScreen::setSpecialModeColor(ModeColor mode){
    switch (mode){
    case ProductScreen::Normal:
        setPalette(QPalette(Colors::PRODUCT_SCREEN_BG_COLOR));
        break;
    case ProductScreen::Invitation:
        setPalette(QPalette(Colors::PRODUCT_SCREEN_SPECIAL_COLOR));;
        break;
    case ProductScreen::ProductAnulation:
        setPalette(QPalette(Colors::PRODUCT_SCREEN_ANULATION_COLOR));
        break;
    case ProductScreen::DefaultOffer:
        setPalette(QPalette(Colors::PRODUCT_SCREEN_DEFAULT_COLOR));
        break;
    }
    setBackgroundRole(QPalette::Window);
}

void ProductScreen::deleteProductSlot(Product *prod){
    if (!selected_product)
        return;
    else
        selected_product->combinationDeleted(prod);
}

void ProductScreen::prepareConnects(Product *prod){

    if ( !prod )  return;

    ProductScreenStack * stack = qobject_cast<ProductScreenStack*>( parent() );
    //check if the parent is a ScreenStack
    //     if ( QString(parent()->metaObject()->className()) == "ScreenStack"){
    if (stack){
        connect(prod, &Product::productClicked, stack, &ProductScreenStack::productClickedSlot);
        connect(prod, &Product::deleteProduct, stack, &ProductScreenStack::deleteProductSlot);
    }
    else{
        //not owned by a screen stack so connect all the methods to the screen
        connect(prod, &Product::productClicked, this, &ProductScreen::productClickedSlot);
        connect(prod, &Product::deleteProduct, this, &ProductScreen::deleteProductSlot);
    }
}

QString ProductScreen::nextPreferredScreenName(){
    return next_preferred_screen;
}
