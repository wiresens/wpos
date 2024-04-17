/***************************************************************************
                          bsladdproductswidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "basicproductcreationwidget.h"

#include "productoptioncreationwidget.h"
#include "productoffercreationwidget.h"

#include "menusystem/utils.h"

#include "productsmodule/productmodule.h"
#include "productsmodule/optionsmodule/productoptionmodule.h"
#include "productsmodule/offersmodule/productoffermodule.h"
#include "database/productoptionsmoduledb.h"
#include "database/productoffersmoduledb.h"
#include "database/productsmoduledb.h"

#include <wposcore/config.h>
#include <wposgui/common/dragobjects.h>
#include <wposgui/keyboard/floatkeyboard.h>

#include "productsmodule/productmodule.h"

#include <libbslxml/xmlconfig.h>

// #include <QDataStream>
// #include <QLineEdit>
// #include <QComboBox>
// #include <QGroupBox>
#include <QMessageBox>
// #include <QFile>
// #include <QString>
// #include <QStringList>
// #include <QPixmap>
// #include <QLabel>
#include <QButtonGroup>
// #include <QListWidget>
// #include <QListWidgetItem>
// #include <QColor>
// #include <QTableWidget>
// #include <QStackedWidget>
// #include <QCheckBox>
// #include <QRadioButton>
// #include <QTextEdit>
// #include <QLayout>
// #include <QFileDialog>
// #include <QPushButton>
// #include <QTabWidget>
#include <QMenu>
#include <QDir>
// #include <QLocale>
// #include <QImage>
// #include <QEvent>
// #include <QHeaderView>

#include <iostream>
using  std::cerr;
using std::endl;

static const QColor COLOR_POP_UP = QColor(255, 222, 115);
static const QColor COLOR_POP_DOWN = QColor(238, 238, 230);

static const QColor BUTTON_OFF = QColor(238, 238, 230);
static const QColor BUTTON_ON = QColor(107, 181, 251);

static const QString& TAXES_DTD = "/etc/ntpv_backoffice/dtds/products_taxeslist.dtd";
static const QString& FAMILIES_LIST_DTD = "/etc/ntpv_backoffice/dtds/products_familieslist.dtd";
static const QString& PRODUCTS_LIST_DTD = "/etc/ntpv_backoffice/dtds/products_productslist.dtd";
static const QString& PRODUCT_COMPOSITION_DTD = "/etc/ntpv_backoffice/dtds/products_composition.dtd";


static const QString TAX_DEFAULT = "iva16";
static const QString& DEFAULT_QUANTITY = "1";

static const QString XML_PRODUCTS_PATH = "/etc/ntpv/bar.xml";
static const QString ICON_PATH = Files::ProductsDir;

static const QString LOGO_BUTTON = "Logo";
static const QString OPTION_BUTTON = "Option";
static const QString OFFER_BUTTON = "Offer";

static const double ICON_SIZE = 32.00;
static const double ICON_BUTTON_SIZE = 50.00;

BasicProductCreationWidget::BasicProductCreationWidget(ProductModule *_product_mod,
        int _mode_product,
        QWidget *parent, const QString& name):
    QWidget(parent),
    product_mod{_product_mod}
{
    setupUi(this);
    setObjectName(name);
    sub_main_frame->setPalette(QPalette(COLOR_POP_DOWN));
    setModeProduct((ProductMode)_mode_product);
    product_mod = _product_mod;

    tax_button_group = new QButtonGroup(tax_group_box);
    tax_button_group->setExclusive(true);
    //     initTaxes();

    table = new BslDDTable(ddtable_frame, "table");
    table->setColumnCount(3);

    QStringList col_names;
    col_names.append(tr("Logo"));
    col_names.append(tr("Product"));
    col_names.append(tr("Quantiy"));
    table->setHorizontalHeaderLabels(col_names);

    QHBoxLayout *layout = 0;
    if(!(layout = qobject_cast<QHBoxLayout *> (ddtable_frame->layout())))
        layout = new QHBoxLayout(ddtable_frame);
    layout->addWidget(table);

    //compose and fill the iconview frame
    icon_view = new BslDDIconView(ddiconview_frame,"iconview");
    if (!(layout = qobject_cast<QHBoxLayout *> (ddiconview_frame->layout())))
        layout = new QHBoxLayout(ddiconview_frame);
    layout->addWidget(icon_view);

    icon_view->setGridSize(QSize(70,70));

    float_keyboard_product = new FloatKeyboard(numblock_product_frame);
    float_keyboard_product->setObjectName("float_keyboard_product");
    if( !( layout = qobject_cast<QHBoxLayout *> (numblock_product_frame->layout())))
        layout = new QHBoxLayout(numblock_product_frame);
    layout->addWidget(float_keyboard_product);

    //@benes    tax_group_box->setOrientation(Qt::Horizontal);

    //pop logo
    pop_logo_menu = new QMenu(this);
    pop_logo_menu->setObjectName("logo");
    pop_logo_menu->setPalette(QPalette(COLOR_POP_UP));
    logo_view = new BslDDIconView(pop_logo_menu);
    logo_view->setObjectName("logo_view");

    if ( !( layout =qobject_cast<QHBoxLayout *> (pop_logo_menu->layout()))){
        layout = new QHBoxLayout(pop_logo_menu);
    }
    layout->addWidget(logo_view);
    pop_logo_menu->setContentsMargins(10,10,10,10);

    logo_view->setGridSize(QSize(60,60));
    //@benes    logo_view->setFixedHeight(475);

    connect(logo_view, &QListWidget::itemChanged, this, QOverload<QListWidgetItem*>::of(&BasicProductCreationWidget::getLogo));
    //@benes    pop_logo->insertItem(logo_view, 0);
//@benes    pop_logo_menu->addAction(QString(), logo_view, logo_view->objectName());

    //pop option
    pop_option_menu = new QMenu(this);
    pop_option_menu->setObjectName( "option");
    option_widget = new ProductOptionCreationWidget(0.00, pop_option_menu, "option_widget");
    option_widget->initOptionTypes();
    option_widget->setEditable(true);
    option_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout = 0;
    if( ! (layout = qobject_cast<QHBoxLayout *> (pop_option_menu->layout())))
        layout = new QHBoxLayout(pop_option_menu);

    layout->addWidget(option_widget);
    //@benes    pop_option_menu->insertItem(option_widget);
//@benes    pop_option_menu->addAction(QString(), option_widget, option_widget->objectName());
    //@benes    pop_option_menu->setLineWidth(0);

    //pop offer
    pop_offer_menu = new QMenu(this);
    pop_offer_menu->setObjectName("offer");
    offer_widget = new ProductOfferCreationWidget(0.00, pop_offer_menu, "offer_widget");
    offer_widget->initOfferTypes();
    offer_widget->setEditable(true);
    offer_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout = 0;
    if(!(layout = (QHBoxLayout *) pop_offer_menu->layout())){
        layout = new QHBoxLayout(pop_offer_menu);
    }
    layout->addWidget(offer_widget);
    //@benes    pop_offer_menu->insertItem(offer_widget);
    //@benes    pop_offer_menu->addAction(QString(), offer_widget, offer_widget->objectName());
    //offer_widget    pop_offer_menu->setLineWidth(0);

    //prepare the connections
    connect(accept_button,   &QPushButton::released, this, &BasicProductCreationWidget::acceptSlot);
    connect(cancel_button,  &QPushButton::released, this, &BasicProductCreationWidget::cancelSlot);
    connect(previous_button,  &QPushButton::released, this, &BasicProductCreationWidget::previousSlot);
    connect(next_button,  &QPushButton::released, this, &BasicProductCreationWidget::nextSlot);

    //product
    connect(product_name, &QLineEdit::textChanged, this, &BasicProductCreationWidget::productNameChanged);
    connect(logo_button, &QPushButton::clicked, this, &BasicProductCreationWidget::logoButtonClicked);
    connect(float_keyboard_product, &FloatKeyboard::valueChanged, this, &BasicProductCreationWidget::productPriceChanged);

    connect(table, QOverload<int, int, const QString&>::of(&BslDDTable::textEnter), this, &BasicProductCreationWidget::draggedText);

    connect(search_button,  &QPushButton::clicked, this, &BasicProductCreationWidget::searchButtonClicked);
    connect(search_edit, &QLineEdit::textChanged, this, &BasicProductCreationWidget::searchEditChanged);

    connect(up_button,  &QPushButton::clicked, this, &BasicProductCreationWidget::upButtonClicked);
    connect(down_button,  &QPushButton::clicked, this, &BasicProductCreationWidget::downButtonClicked);
    connect(delete_button,  &QPushButton::clicked, this, &BasicProductCreationWidget::deleteButtonClicked);

    connect(new_option_button,  &QPushButton::clicked, this, &BasicProductCreationWidget::newOptionClicked);
    connect(new_offer_button,  &QPushButton::clicked, this, &BasicProductCreationWidget::newOfferClicked);

    connect(pop_option_menu, &QMenu::aboutToShow, this, &BasicProductCreationWidget::showPopOption);
    connect(pop_option_menu, &QMenu::aboutToHide, this, &BasicProductCreationWidget::hidePopOption);

    connect(pop_offer_menu,  &QMenu::aboutToShow, this, &BasicProductCreationWidget::showPopOffer);
    connect(pop_offer_menu, &QMenu::aboutToHide, this, &BasicProductCreationWidget::hidePopOffer);

    connect(pop_logo_menu,  &QMenu::aboutToShow, this, &BasicProductCreationWidget::showPopLogo);
    connect(pop_logo_menu, &QMenu::aboutToHide, this, &BasicProductCreationWidget::hidePopLogo);
}

void BasicProductCreationWidget::init(){

    clear();
    product_stack->setCurrentWidget(product_details_page);
    switch(product_mode){
    case UnitaryProduct:
        previous_button->hide();
        next_button->hide();
        accept_button->setEnabled(false);
        accept_button->show();
        cancel_button->show();
        break;

    case CompositionProduct:
        previous_button->hide();
        next_button->setEnabled(false);
        next_button->show();
        accept_button->hide();
        cancel_button->show();
        break;
    }
}

void BasicProductCreationWidget::clear(){
    initTaxes();
    table->setRowCount(0);
    product_name->clear();
    description_text->clear();
    float_keyboard_product->clear();

    logo.clear();
    logo_button->setIcon(QIcon());
    logo_button->setText(LOGO_BUTTON);

    if(product){
        product = 0;
        delete product;
    }

    switch(product_mode){
    case UnitaryProduct:
        accept_button->setEnabled(false);
        accept_button->show();
        cancel_button->setEnabled(true);
        cancel_button->show();
        next_button->hide();
        previous_button->hide();
        break;
    case CompositionProduct:
        accept_button->hide();
        cancel_button->setEnabled(true);
        cancel_button->show();
        next_button->show();
        previous_button->hide();
        break;
    }
}

void BasicProductCreationWidget::initTaxes(){

    clearTaxes();
    QLayout *layout = tax_group_box->layout();
    if(!layout)  return;

    XmlConfig xml;
    if(!xml.readXmlFromString(product_mod->getTaxes())) return;

    if(!xml.validateXmlWithDTD(TAXES_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.setDomain("taxes[0]");
    for( auto i=0; i < xml.howManyTags("tax"); i++){
        auto tmp_str = xml.readString("tax[" + QString::number(i) + "]");
        auto button = new QPushButton(tmp_str, tax_group_box);
        button->setObjectName(tmp_str);
        button->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
        button->setCheckable(true);
        button->setBackgroundRole(QPalette::Button);
        button->setPalette(QPalette(BUTTON_OFF));
        button->setDown(true);
        button->show();
        button_list.append(button);
        tax_button_group->addButton(button);

        if(tmp_str == TAX_DEFAULT){
            button->setPalette(QPalette(BUTTON_ON));
            button->setDown(false);
        }

        connect(button, &QPushButton::clicked, this, &BasicProductCreationWidget::taxChanged);
        layout->addWidget(button);
    }
}

void BasicProductCreationWidget::productNameChanged(const QString &text){
    QString tmp_str = text;
    static QRegularExpression re(" ");
    switch(product_mode){
    case UnitaryProduct:
        tmp_str = tmp_str.replace(re,"_");
        if(tmp_str.isEmpty()){
            accept_button->setEnabled(false);
            next_button->setEnabled(false);
        }
        else{
            accept_button->setEnabled(true);
            next_button->setEnabled(true);
        }
        break;

    case CompositionProduct:
        activeButtons();
        break;
    }
}

void BasicProductCreationWidget::taxChanged(){

    for( auto* button : button_list){
        if( !button->isDown()) button->setPalette(QPalette(BUTTON_ON));
        else button->setPalette(QPalette(BUTTON_OFF));
    }
}

void BasicProductCreationWidget::initLogos(BslDDIconView *icon_view){

    QString absolute_file_path;
    icon_view->clear();
    QStringList files = QDir(ICON_PATH).entryList(QStringList("*.png"), QDir::Files, QDir::Name);
    for(const auto& file : files){
        absolute_file_path = ICON_PATH + file;
        auto item = new QListWidgetItem( cropedIcon(absolute_file_path, ICON_SIZE) , file, icon_view);
        icon_view->addItem(item);;
    }
    //set also the no lo go icon
    icon_view->addItem(new QListWidgetItem(tr("WITHOUT ICON") , icon_view));
    files.clear();
}

void BasicProductCreationWidget::acceptSlot(){
    offer_widget->checkDefaultOffers();
    option_widget->checkDefaultOptions();

    switch(product_mode){
    case UnitaryProduct:
        if(!insertNewProduct())
            return;
        break;
    case CompositionProduct:
        if(!insertNewComposition())
            return;
        break;
    }

    if(!option_widget->saveOptionsProduct(product->code)){
        cerr << "Can not save product options " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!offer_widget->saveOffersProduct(product->code)){
        cerr << "Can not save product offers " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    clearCodeList();

    clear();
    product_stack->setCurrentWidget(product_details_page);
}

void BasicProductCreationWidget::activeButtons(){
    QString aux;
    int rows;

    aux = product_name->text();
    rows = table->rowCount();
    if(aux.isEmpty()){
        accept_button->setEnabled(false);
        next_button->setEnabled(false);
    }
    else{
        next_button->setEnabled(true);
        if(rows > 1)
            accept_button->setEnabled(true);
        else
            accept_button->setEnabled(false);
    }
}

bool BasicProductCreationWidget::insertNewProduct(){

    clearCodeList();
    auto ingredient = new IngredientData;

    auto code_aux = product_name->text();
    code_aux.replace(" ", "_");
    code_aux = code_aux.toLower();
    ingredient->ingredient_code = code_aux;
    ingredient->quantity = 1.00;
    code_list.append(ingredient);

    if(!product)  product = new ProductData;

    product->code = code_aux;
    product->name = product_name->text();
    product->price = float_keyboard_product->value();
    product->tax = getTax();
    product->description = description_text->text();

    XmlConfig xml;
    xml.createElementSetDomain("products");
    xml.createElementSetDomain("product");

    xml.createElement("code", product->code);
    xml.createElement("name", product->name);
    xml.createElement("price", QString::number(product->price));
    xml.createElement("tax", product->tax);
    xml.createElement("logo", logo);
    xml.createElement("description", product->description);

    xml.releaseDomain("product", false);
    xml.releaseDomain("products", true);

    if(!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }

    if(product_mod->insertProduct(xml.toString())){
        if(!insertProductComposition(code_aux)){
            QMessageBox::warning(this, tr("Inserting product"), tr("An error occurred while inserting the product,\n Try again later."));
            return false;
        }
    }
    else  return false;
    return true;
}

QString BasicProductCreationWidget::getTax(){
    for(const auto& button : button_list)
        if(! button->isDown()) return button->text();
    return QString();
}

bool BasicProductCreationWidget::insertProductComposition(const QString& code){

    XmlConfig xml;
    xml.createElementSetDomain("composition");
    xml.createElement("composition_code", code);

    for (auto* ingredient : code_list){

        if(ingredient->ingredient_code.isEmpty())  continue;

        xml.createElementSetDomain("ingredient");
        xml.createElement("ingredient_code", ingredient->ingredient_code);

        if(ingredient->quantity == 0.00){
            QMessageBox::warning(this, tr("Creating Composition"),
                                 tr("Quantity of each product must be greater than 0"));
            cerr << "Problems with product cuantity " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            return false;
        }

        xml.createElement("quantity", QString::number(ingredient->quantity));
        xml.releaseDomain("ingredient", false);
    }
    xml.releaseDomain("composition", true);

    if(!xml.validateXmlWithDTD(PRODUCT_COMPOSITION_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }

    return product_mod->insertProductComposition(xml.toString());
}

QString BasicProductCreationWidget::getCode(){

    clearCodeList();
    code_list = getComposition();

    auto prod_code = product_name->text();
    prod_code.replace(" ", "_");
    prod_code = prod_code.toLower();


    if(code_list.isEmpty()){
        auto ingredient = new IngredientData;
        ingredient->ingredient_code = prod_code;
        ingredient->quantity = 1.00;
        code_list.append(ingredient);
        return prod_code;
    }

    QStringList codes;
    for(const auto* ingredient : code_list)
        codes.append(ingredient->ingredient_code);

    codes.sort();
    prod_code.clear();

    for(const auto& code : codes)
        prod_code += " " + code;

    return prod_code.toLower().trimmed();
}

int BasicProductCreationWidget::productPosition(const QString& name, XmlConfig *xml){
    //if the xml haven't this product's name, return a negative number
    int position = -1;
    int i, count;
    QString auxiliar;

    count = xml->howManyTags("product");
    for(i = 0; i < count; i++){
        //Read the product's name
        auxiliar = xml->readString("product["+QString::number(i)+"].name");

        if (auxiliar == name){
            position = i;
            break;
        }
    }

    return position;
}

void BasicProductCreationWidget::clearTaxes(){
    clearButtonList();
}

void BasicProductCreationWidget::showWidgetAction(){
    showInsertProduct();
    if(product_mode == UnitaryProduct){
        accept_button->hide();
        next_button->show();
        previous_button->hide();
        cancel_button->show();
    }
}

void BasicProductCreationWidget::showInsertProduct(){
    product_stack->setCurrentWidget(product_details_page);
    clear();

    next_button->hide();
    accept_button->show();
    previous_button->hide();
    cancel_button->show();
}

void BasicProductCreationWidget::cancelSlot(){
    clearCodeList();
    clear();
}

void BasicProductCreationWidget::setTax(const QString& tax){

    for(auto* button : button_list){
        if (button->text() == tax){
            button->setDown(false);
            button->setPalette(QPalette(BUTTON_ON));
        }
        else{
            button->setDown(true);
            button->setPalette(QPalette(BUTTON_OFF));
        }
    }
}

bool BasicProductCreationWidget::insertNewComposition(){

    QString product_code = getCode();
    if(product)  delete product;

    product = new ProductData;
    product->code = product_code;
    product->name = product_name->text();
    product->price = float_keyboard_product->value();
    product->tax = getTax();

    QListWidgetItem *item = logo_view->currentItem();
    if(item)  product->logo = item->text();
    else product->logo.clear();

    product->description = description_text->text();

    QWidget *page = product_stack->currentWidget();

    if( page == product_details_page)
    {
        insertItems(icon_view, true);
        product_stack->setCurrentWidget(product_composition_page);
        return false;
    }
    else if(page == product_composition_page)
    {
        XmlConfig xml;
        xml.createElementSetDomain("products");
        xml.createElementSetDomain("product");
        xml.createElement("code", product->code);
        xml.createElement("name", product->name);
        xml.createElement("price", QString::number(product->price));
        xml.createElement("tax", product->tax);
        xml.createElement("logo", product->logo);
        xml.createElement("description", product->description);
        xml.releaseDomain("product", true);
        xml.releaseDomain("products", true);

        if(!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
            cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
            xml.debug();
            return false;
        }

        if( ! product_mod->insertProduct( xml.toString()) )  return false;
        if( !insertProductComposition(product_code) ) return false;
    }
    return true;
}

void BasicProductCreationWidget::insertItems(BslDDIconView *icon_view, bool unitary){

    if(!icon_view) return;
    icon_view->clear();

    QString xml_string;
    if(unitary) xml_string = product_mod->getUnitaryProducts();
    else xml_string = product_mod->getCompositions();

    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "can not convert xml into string " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
    xml.setDomain("products[0]");
    for(auto i=0; i < xml.howManyTags("product"); i++){
        xml.setDomain("product[" + QString::number(i) + "]");
        auto iconfile = ICON_PATH + xml.readString("logo");
        auto item = new QListWidgetItem(cropedIcon(iconfile, ICON_SIZE) , xml.readString("name"), icon_view );
        icon_view->addItem(item);
        xml.releaseDomain("product", false);
    }
    xml.releaseDomain("products", true);
}

void BasicProductCreationWidget::nextSlot(){
    product_stack->setCurrentWidget(product_composition_page);

    next_button->hide();
    accept_button->show();
    previous_button->show();
    cancel_button->show();
    icon_view->clear();

    insertItems(icon_view, true);
}

void BasicProductCreationWidget::previousSlot(){
    product_stack->setCurrentWidget(product_details_page);

    switch(product_mode){
    case UnitaryProduct:
        next_button->hide();
        accept_button->show();
        previous_button->hide();
        cancel_button->show();
        return;
        break;

    case CompositionProduct:
        next_button->show();
        accept_button->hide();
        previous_button->hide();
        cancel_button->show();
        return;
        break;
    }
}

void BasicProductCreationWidget::draggedText(int x, int y, const QString& text){
    (void )x;
    (void) y;

    auto row = table->rowCount();
    auto item = new QTableWidgetItem(text);
    table->insertRow(row);
    table->setItem(row, 1, item);
    item = new QTableWidgetItem(DEFAULT_QUANTITY);
    table->setItem(row, 2, item);
    activeButtons();
}

void BasicProductCreationWidget::searchButtonClicked(){
    QString name = product_mod->getProductLike(search_edit->text());
    if(!name.isEmpty()) selectProduct(name, icon_view);
}

void BasicProductCreationWidget::searchEditChanged(const QString &text){
    QString name = product_mod->getProductLike(text);
    if(!name.isEmpty()) selectProduct(name, icon_view);
}

void BasicProductCreationWidget::selectProduct(const QString& product, BslDDIconView *icon_view){
    auto items = icon_view->findItems(product, Qt::MatchExactly);
    if(!items.isEmpty()){
        icon_view->setCurrentItem(items.first());
//        icon_view->ensureItemVisible(item);
        items.first()->setHidden(false);
    }
}

void BasicProductCreationWidget::showEvent(QShowEvent *e){
    initLogos(logo_view);
    init();
    QWidget::showEvent(e);
}

void BasicProductCreationWidget::hideEvent(QHideEvent *e){
    clear();
    clearCodeList();
    clearButtonList();
    QWidget::hideEvent(e);
}

QWidget* BasicProductCreationWidget::getIdVisibleWidget(){
    return product_stack->currentWidget();
}

void BasicProductCreationWidget::setModeProduct(ProductMode _mode_product){
    product_mode = _mode_product;
    switch (product_mode){
    case UnitaryProduct:
        logo_button->show();
        break;
    case CompositionProduct:
        logo_button->hide();
        break;
    }
}

QList<IngredientData*> BasicProductCreationWidget::getComposition(){
    int index, count;
    IngredientData *ingredient = 0;
    QList<IngredientData*> ingredient_list;

    count = table->rowCount();
    for(index = 0; index < count; index++){
        ingredient = 0;
        ingredient = new IngredientData;
        ingredient->ingredient_code = product_mod->getProductCodeByProductName(table->item(index, 1)->text());
        ingredient->quantity = table->item(index, 2)->text().toDouble();
        ingredient_list.append(ingredient);
    }

    return ingredient_list;
}

void BasicProductCreationWidget::upButtonClicked(){
//@benes
//    int row, index, count;

//    row = table->currentRow();
//    if(!row)
//        return;

//    table->swapRows(row, row - 1, false);

//    count = table->columnCount();
//    for(index = 0; index < count; index++){
//        table->updateCell(row - 1, index);
//        table->updateCell(row, index);
//    }
}

void BasicProductCreationWidget::downButtonClicked(){

//@benes
//    int row, index, count;

//    row = table->currentRow();
//    if(row == table->rowCount() - 1)  return;

//    table->swapRows(row, row + 1, false);

//    count = table->columnCount();
//    for(index = 0; index < count; index++){
//        table->updateCell(row + 1, index);
//        table->updateCell(row, index);
//    }
}

void BasicProductCreationWidget::deleteButtonClicked(){
    table->removeRow(table->currentRow());
}

void BasicProductCreationWidget::logoButtonClicked(){
    pop_logo_menu->setFixedSize(300, 300);
    int x = logo_frame->mapToGlobal(QPoint(0,0)).x() + logo_frame->height();
    int y = logo_frame->mapToGlobal(QPoint(0,0)).y();

    pop_logo_menu->exec(QPoint(x,y));
}

void BasicProductCreationWidget::getLogo(QListWidgetItem* item){

    logo.clear();
    if(item){
        logo = item->text();
        if (logo == tr("WITHOUT ICON"))
            logo.clear();
    }

    pop_logo_menu->hide();
    if (logo.isEmpty()){
        logo_button->setText(tr("Logo"));
        return;
    }

    auto iconfile = ICON_PATH + logo;
    logo_button->setIcon(cropedIcon(iconfile, ICON_BUTTON_SIZE));
}

void BasicProductCreationWidget::getLogo(const QString& file_path){
    logo_button->setIcon(cropedIcon(file_path, ICON_BUTTON_SIZE));
}

void BasicProductCreationWidget::newOptionClicked(){
    int x = new_option_frame->mapToGlobal(QPoint(0, 0)).x() + new_option_frame->width();
    int y = new_option_frame->mapToGlobal(QPoint(0, 0)).y() + new_option_frame->height() - option_widget->height();

    pop_option_menu->exec(QPoint(x,y));
}

void BasicProductCreationWidget::newOfferClicked(){
    int x = new_offer_frame->mapToGlobal(QPoint(0, 0)).x() + new_offer_frame->width();
    int y = new_offer_frame->mapToGlobal(QPoint(0, 0)).y() + new_offer_frame->height() - offer_widget->height();

    pop_offer_menu->exec(QPoint(x,y));
}

void BasicProductCreationWidget::productPriceChanged(double price){
    option_widget->setProductPrice(price);
    offer_widget->setProductPrice(price);
}

void BasicProductCreationWidget::showPopOption(){
    new_option_frame->setPalette(COLOR_POP_UP);
    setEnabled(false, OPTION_BUTTON);
}

void BasicProductCreationWidget::hidePopOption(){
    new_option_frame->setPalette(QPalette(COLOR_POP_DOWN));
    setEnabled(true, OPTION_BUTTON);
}

void BasicProductCreationWidget::showPopOffer(){
    new_offer_frame->setPalette(QPalette(COLOR_POP_UP));
    setEnabled(false, OFFER_BUTTON);
}

void BasicProductCreationWidget::hidePopOffer(){
    new_offer_frame->setPalette(QPalette(COLOR_POP_DOWN));
    setEnabled(true, OFFER_BUTTON);
}

void BasicProductCreationWidget::showPopLogo(){
    logo_frame->setPalette(QPalette(COLOR_POP_UP));
    setEnabled(false, LOGO_BUTTON);
}

void BasicProductCreationWidget::hidePopLogo(){
    logo_frame->setPalette(QPalette(COLOR_POP_DOWN));
    setEnabled(true, LOGO_BUTTON);
}

void BasicProductCreationWidget::setEnabled(bool enabled, const QString& button_pressed){
    title_pixmap_label1->setEnabled(enabled);
    title_pixmap_label_text1->setEnabled(enabled);
    name_label->setEnabled(enabled);
    product_name->setEnabled(enabled);
    description_label->setEnabled(enabled);
    description_text->setEnabled(enabled);

    title_pixmap_label2->setEnabled(enabled);
    title_pixmap_label_text2->setEnabled(enabled);
    tax_group_box->setEnabled(enabled);
    numblock_product_frame->setEnabled(enabled);

    title_pixmap_label3->setEnabled(enabled);
    title_pixmap_label_text3->setEnabled(enabled);

    accept_button->setEnabled(enabled);
    cancel_button->setEnabled(enabled);
    previous_button->setEnabled(enabled);
    next_button->setEnabled(enabled);

    if(button_pressed == LOGO_BUTTON){
        new_option_button->setEnabled(enabled);
        new_offer_button->setEnabled(enabled);
    }

    if(button_pressed == OPTION_BUTTON){
        logo_button->setEnabled(enabled);
        new_offer_button->setEnabled(enabled);
    }

    if(button_pressed == OFFER_BUTTON){
        logo_button->setEnabled(enabled);
        new_option_button->setEnabled(enabled);
    }
}

void BasicProductCreationWidget::clearButtonList(){
    for(auto* button : button_list){
        tax_button_group->removeButton(button);
        button->deleteLater();
    }
    button_list.clear();
}

void BasicProductCreationWidget::clearCodeList(){
    for(auto* code : code_list) delete code;
    code_list.clear();
}
