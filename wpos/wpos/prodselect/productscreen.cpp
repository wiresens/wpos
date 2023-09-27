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
#include <wposwidget/global.h>

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
ProductScreen::ProductScreen(const QString& screen_name,
                             XmlConfig *xml,
                             QWidget *_parent,
                             const QString& name):
    QWidget(_parent)
  //    parent{_parent}
{
    setObjectName(name);
    //the products should be deleted at the list an only cleared from the dict
    //the layout should be defined at the init method
    layout = new QGridLayout(this);
    initScreen(screen_name, xml);
}

ProductScreen::ProductScreen(const QString& screen_name,
                             const QString& file_name,
                             QWidget *_parent,
                             const QString& name):
    QWidget(_parent)
  //    parent{_parent}
{
    setObjectName(name);
    //the layout will be defined in the initScreen method
    layout = new QGridLayout(this);


    XmlConfig xml(file_name);
    if ( !QFile::exists(file_name) || !xml.isValid() ){
        setScreenName(SCREEN_DEFAULT_NAME);
        return;
    }

    initScreen(screen_name, &xml);
}

ProductScreen::~ProductScreen(){
    resetScreen();
    delete layout;
    delete product_list;
}

bool ProductScreen::initScreen(const QString& screen_name, XmlConfig *xml){

    setSpecialModeColor(ProductScreen::Normal);
    setScreenName(screen_name);

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
        if ( aux_string == screenName() ){
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

    next_preferred_screen = screenName();
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
    ProductScreenStack *screen_stack = qobject_cast<ProductScreenStack *>( parent() );

    auto font_family = xml->readString("fontfamily");
    if ( font_family.isEmpty() )
        font_family = screen_stack->getDefaultFamily();

    auto text_font_family = xml->readString("textfontfamily");
    if (text_font_family.isEmpty())
        text_font_family = screen_stack->getDefaultTextFamily();

    auto text_background_color = xml->readString("textbackgroundcolor");
    if (text_background_color.isEmpty())
        text_background_color = screen_stack->getTextBackgroundColor();

    bool converted {false};
    aux_string = xml->readString("fontsize");
    auto font_size = aux_string.toInt(&converted);

    if ( !converted ) font_size = -1;
    if (font_size ==-1 ) font_size = screen_stack->getDefaultSize();

    aux_string = xml->readString("textfontsize");
    auto text_font_size = aux_string.toInt(&converted);

    if ( !converted ) text_font_size = -1;
    if (text_font_size == -1) text_font_size = screen_stack->getDefaultTextSize();

    delete layout;

    /*************   LAYOUT PROPERTIES   ***************/
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
        aux_string = xml->readString("product["+QString::number(i)+"].name");

        //do not accept empty named products.
        if (aux_string.isEmpty()) continue;//prepare the reread of the product

        xml->pushDomain();
        Product *prod = new Product(aux_string, xml, this);
        if ( !font_family.isEmpty() )
            prod->setDefaultFontFamily(font_family);

        if (font_size!=-1)
            prod->setDefaultFontSize(font_size);

        if (!text_font_family.isEmpty())
            prod->setDefaultTextFontFamily(text_font_family);

        if ( text_font_size !=-1 )
            prod->setDefaultTextFontSize(text_font_size);

        if ( !text_background_color.isEmpty() ){
            //@benes  prod->setPaletteBackgroundColor(QColor(text_background_color));
            //we thought the folliwing fixed the pb , but it didn't.
//            QPalette palette(QPalette(QColor(text_background_color) , QPalette::WindowText));
//            prod->setPalette(palette);
        }

        if (show_text)  prod->setTextInPixmap(true);

        xml->popDomain();

        //connect of all signals
        prepareConnects(prod);

        layout->addWidget(prod, row, col);
        product_list->append(prod);
        product_dict->insert(prod->getProductName(), prod);
    }
    xml->delDomain();
    xml->popDomain();
    return true;
}

bool ProductScreen::resetScreen(){
    setSpecialModeColor(ProductScreen::Normal);
    product_list->clear();
    product_dict->clear();
    delete layout;
    cols = DEFAULT_COLS;
    rows = DEFAULT_ROWS;
    layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);
    return true;
}

QString ProductScreen::screenName(){
    return name;
}

void ProductScreen::setScreenName(const QString& screen_name){
    name = screen_name;
}

void ProductScreen::showAll(){

    for( auto product : *product_list)
        product->show();

}

void ProductScreen::disableAll(){

    for( auto prod : *product_list)
        prod->disableProduct();

}

void ProductScreen::defaultValuesAll(){

    for(auto prod : *product_list )
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
        prod = product_dict->operator [](list[i]);
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
        connect(selected_product,SIGNAL(combineWith(QStringList )),
                this,SLOT(combineWithSlot(QStringList *)));
        connect(selected_product,SIGNAL(productDefinition(XmlConfig *)),
                this,SLOT(productDefinitionSlot(XmlConfig *)));
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
    case ProductScreen::Product_anulation:
        setPalette(QPalette(Colors::PRODUCT_SCREEN_ANULATION_COLOR));
        break;
    case ProductScreen::Default_offer:
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

    ProductScreenStack * stack = qobject_cast<ProductScreenStack *>( parent() );
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
