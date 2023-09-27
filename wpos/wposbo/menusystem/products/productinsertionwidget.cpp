/***************************************************************************
                          naddproductwidget.cpp  -  description
                             -------------------
    begin                : jue abr 29 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productinsertionwidget.h"
#include "menusystem/utils.h"

#include "productsmodule/productmodule.h"
#include "productsmodule/optionsmodule/productoptionmodule.h"
#include "productsmodule/offersmodule/productoffermodule.h"

#include "database/productoptionsmoduledb.h"
#include "database/productoffersmoduledb.h"
#include "database/productsmoduledb.h"

#include <libbslxml/xmlconfig.h>
#include <wposwidget/toolkit.h>
#include <wposwidget/dragobjects.h>
#include <wposwidget/floatkeyboardbox.h>
#include <wposwidget/numkeyboardbox.h>

#include <QDataStream>
#include <QLineEdit>
#include <QComboBox>
#include <QApplication>
#include <QGroupBox>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QPixmap>
#include <QLabel>
#include <QButtonGroup>
#include <QCheckBox>
#include <QListWidget>
#include <QColor>
#include <QTableWidget>
#include <QLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QMenu>
#include <QImage>
#include <QMatrix>
#include <QHeaderView>
#include <QTimer>
#include <QStackedWidget>
#include <QProgressBar>
#include <QComboBox>
#include <QTreeWidget>
#include <QHeaderView>
#include <QTimer>
#include <QLocale>

#include <iostream>
using namespace std;

static const QString& PRODUCTS_LIST_DTD {"/etc/ntpv_backoffice/dtds/products_productslist.dtd"};
static const QString& PRODUCT_COMPOSITION_DTD {"/etc/ntpv_backoffice/dtds/products_composition.dtd"};
static const QString& TAXES_DTD {"/etc/ntpv_backoffice/dtds/products_taxeslist.dtd"};
static const QString& OPTIONS_LIST_DTD {"/etc/ntpv_backoffice/dtds/products_optionslist.dtd"};
static const QString& OFFERS_LIST_DTD  {"/etc/ntpv_backoffice/dtds/products_offerslist.dtd"};

static const QString& UNITARY_PRODUCT_TITLE {"Creacion de productos unitarios"};
static const QString& COMPOSED_PRODUCTS_TITLE {"Creacion de productos compuestos"};

//static const QPixmap NULL_LOGO_PIXMAP = QPixmap();
static const QString& NULL_LOGO_TEXT {"SIN LOGO"};

static const QString& IS_DEFAULT_OPTION_LOGO {"/usr/share/ntpv_backoffice/apps/32x32/button_ok.png"};
static const QString& NO_DEFAULT_OPTION_LOGO {"/usr/share/ntpv_backoffice/apps/32x32/cancel.png"};

static const QColor BUTTON_OFF = QColor(238, 238, 230);
static const QColor BUTTON_ON = QColor(107, 181, 251);
static const QString& DEFAULT_TAX {"iva16"};

static const QString& DEFAULT_QUANTITY {"1.0"};
static const QString& ICON_PATH {"/usr/share/ntpv/logos/"};
static const double ICON_BUTTON_SIZE  = 50.00;
static const double ICON_SIZE = 32.00;

ProductInsertionWidget::ProductInsertionWidget(
        ProductModule *product_module ,
        int _mode,
        QWidget *parent,
        const QString& name ):
    QWidget(parent),
    product_model {product_module},
    options_list  {new HList<NOTypes>},
    offers_list  {new HList<NOTypes>}

{   
    setupUi(this);
    setObjectName(name);
    mode = (ProductMode)_mode;
    timer = new QTimer(this);

    tax_button_group = new QButtonGroup(tax_group);
    option_type_listview->header()->hide();
    offer_type_listview->header()->hide();
    option_listview->header()->hide();
    offer_listview->header()->hide();

    option_type_listview->setSelectionMode(QAbstractItemView::SingleSelection);
    offer_type_listview->setSelectionMode(QAbstractItemView::SingleSelection);
    option_listview->setSelectionMode(QAbstractItemView::SingleSelection);
    offer_listview->setSelectionMode(QAbstractItemView::SingleSelection);

    option_type_listview->setAllColumnsShowFocus(true);
    offer_type_listview->setAllColumnsShowFocus(true);
    option_listview->setAllColumnsShowFocus(true);
    offer_listview->setAllColumnsShowFocus(true);

    option_type_listview->header()->setDefaultAlignment(Qt::AlignCenter);
    offer_type_listview->header()->setDefaultAlignment(Qt::AlignCenter);
    option_listview->header()->setDefaultAlignment(Qt::AlignCenter);
    offer_listview->header()->setDefaultAlignment(Qt::AlignCenter);

    //@benes    option_type_listview->setColumnAlignment(0,Qt::AlignCenter);
    //    offer_type_listview->setColumnAlignment(0,Qt::AlignCenter);

    //    option_listview->setColumnAlignment(0,Qt::AlignCenter);
    //    option_listview->setColumnAlignment(1,Qt::AlignCenter);
    //    option_listview->setColumnAlignment(2,Qt::AlignCenter);

    //    offer_listview->setColumnAlignment(0,Qt::AlignCenter);
    //    offer_listview->setColumnAlignment(1,Qt::AlignCenter);

    offer_type_listview->setColumnWidth(0,320);
    option_type_listview->setColumnWidth(0,320);

    option_listview->setColumnWidth(0,40);
    option_listview->setColumnWidth(1,220);
    option_listview->setColumnWidth(2,60);

    offer_listview->setColumnWidth(0,260);
    offer_listview->setColumnWidth(1,60);

    //        pix_label1->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/package.png"));
    pix_label2->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/xcalc.png"));

    ok_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_ok_48.png"));
    cancel_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_cancel.png"));

    up_table_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/up.png"));
    down_table_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/down.png"));
    up_unitary_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/up.png"));
    down_unitary_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/down.png"));
    delete_table_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/edittrash.png"));


    add_option_type_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/filesave.png"));
    del_option_type_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/edittrash.png"));
    add_option_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/filesave.png"));
    del_option_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/edittrash.png"));
    option_apply_price_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/filesave.png"));

    add_offer_type_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/filesave.png"));
    del_offer_type_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/edittrash.png"));
    add_offer_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/filesave.png"));
    del_offer_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/edittrash.png"));
    offer_apply_price_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/filesave.png"));

    up_option_type_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/up.png"));
    down_option_type_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/down.png"));
    up_option_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/up.png"));
    down_option_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/down.png"));

    up_offer_type_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/up.png"));
    down_offer_type_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/down.png"));
    up_offer_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/up.png"));
    down_offer_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/down.png"));

    QHBoxLayout *hlayout;
    if(!(hlayout =(QHBoxLayout *) ddtable_frame->layout()))
        hlayout = new QHBoxLayout(ddtable_frame);

    table = new BslDDTable(ddtable_frame, "table");
    table->setColumnCount(2);

    QStringList col_names;
    col_names << tr("Product");
    col_names << tr("Quantity");
    //        (table->verticalHeader())->hide();
    table->setColumnWidth(0, 260);
    table->setColumnWidth(1, 100);
    table->setHorizontalHeaderLabels(col_names);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    QFont font;
    font = this->font();
    font.setPointSize(16);
    table->setFont(font);
    hlayout->addWidget(table);

    if (!(hlayout =(QHBoxLayout *) ddiconview_frame->layout()))
        hlayout = new QHBoxLayout(ddiconview_frame);

    icon_view = new BslDDIconView(ddiconview_frame,"iconview");
    icon_view->setGridSize(QSize(70,70));
    icon_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    hlayout->addWidget(icon_view);

    if(!(hlayout = (QHBoxLayout *) numblock_product_frame->layout()))
        hlayout = new QHBoxLayout(numblock_product_frame);

    float_kb = new FloatKeyboardBox(numblock_product_frame);
    float_kb->hideNumber(true);
    hlayout->addWidget(float_kb);

    QVBoxLayout *vlayout;
    if(!(vlayout = (QVBoxLayout *) box_frame_offers->layout()))
        vlayout = new QVBoxLayout(box_frame_offers);

    float_kb_offers = new FloatKeyboardBox(box_frame_offers);
    float_kb_offers->hideNumber(true);
    vlayout->addWidget(float_kb_offers);
    num_kb_offers = new NumKeyboardBox(box_frame_offers);
    num_kb_offers->hideNumber(true);
    vlayout->addWidget(num_kb_offers);
    num_kb_offers->hide();
    price_label_offers_percent->hide();


    if(!(hlayout = (QHBoxLayout *) box_frame_options->layout())){
        hlayout = new QHBoxLayout(box_frame_options);
    }
    float_kb_options = new FloatKeyboardBox(box_frame_options);
    float_kb_options->hideNumber(true);
    hlayout->addWidget(float_kb_options);

    pop_logo = new QMenu(this);
    pop_logo->setContentsMargins(10,10,10,10);
    if (!(hlayout =(QHBoxLayout *) pop_logo->layout())){
        hlayout = new QHBoxLayout(pop_logo);
    }
    logo_view = new BslDDIconView(pop_logo,"logo_view");
    logo_view->setGridSize(QSize(60,60));
    //@benes    logo_view->setFixedHeight(475);
    //@benes    logo_view->setFixedWidth(310);
    hlayout->addWidget(logo_view);
    //@benes    pop_logo->insertItem(logo_view, 0);

    setMode(mode);
    getTaxesButtons();
    getLogos();

    kitchen_checkbox_button->setChecked(false);

    connect(product_name_lineedit, SIGNAL(textChanged(const QString &)),this, SLOT(nameChangedSlot(const QString &)));
    connect(this, SIGNAL(progressSignal(int, const QString&)),this,SLOT(setProgressSlot(int, const QString&)));
    connect(tab, &QTabWidget::currentChanged, this, &ProductInsertionWidget::tabChangedSlot);

    connect(float_kb,SIGNAL(numChanged(double)),this,SLOT(numkeyChangedSlot(double)));
    connect(float_kb_options,SIGNAL(numChanged(double)),this,SLOT(optionNumkeyChangedSlot(double)));
    connect(float_kb_offers,SIGNAL(numChanged(double)),this,SLOT(offerNumkeyChangedSlot(double)));
    connect(num_kb_offers,SIGNAL(numChanged(int)),this,SLOT(offerNumkeyChangedSlot(int)));

    connect(logo_button, SIGNAL(clicked()), this, SLOT(logoButtonClicked()));
    connect(pop_logo, SIGNAL(aboutToShow()), this, SLOT(showPopLogo()));
    connect(pop_logo, SIGNAL(aboutToHide()), this, SLOT(hidePopLogo()));
    connect(logo_view, &BslDDIconView::itemClicked, this, &ProductInsertionWidget::logoClickedSlot);

    connect(search_button, SIGNAL(clicked()), this, SLOT(searchButtonClickedSlot()));
    connect(search_lineedit, SIGNAL(textChanged(const QString &)),this, SLOT(searchEditChangedSlot(const QString &)));
    connect(up_unitary_button,SIGNAL(clicked()),this,SLOT(upScrollUnitaryViewSlot()));
    connect(down_unitary_button,SIGNAL(clicked()),this,SLOT(downScrollUnitaryViewSlot()));
    connect(table,SIGNAL(textEnter(int, int, const QString&)),this,SLOT(draggedText(int, int, const QString&)));
    connect(up_table_button, SIGNAL(clicked()), this, SLOT(upTableButtonClicked()));
    connect(down_table_button, SIGNAL(clicked()), this, SLOT(downTableButtonClicked()));
    connect(delete_table_button, SIGNAL(clicked()), this, SLOT(deleteTableButtonClicked()));


    connect(up_option_type_button,SIGNAL(clicked()),this,SLOT(upOptionTypeSlot()));
    connect(down_option_type_button,SIGNAL(clicked()),this,SLOT(downOptionTypeSlot()));
    connect(up_option_button,SIGNAL(clicked()),this,SLOT(upOptionSlot()));
    connect(down_option_button,SIGNAL(clicked()),this,SLOT(downOptionSlot()));
    connect(up_offer_type_button,SIGNAL(clicked()),this,SLOT(upOfferTypeSlot()));
    connect(down_offer_type_button,SIGNAL(clicked()),this,SLOT(downOfferTypeSlot()));
    connect(up_offer_button,SIGNAL(clicked()),this,SLOT(upOfferSlot()));
    connect(down_offer_button,SIGNAL(clicked()),this,SLOT(downOfferSlot()));

    connect(add_option_type_button,SIGNAL(clicked()),this,SLOT(addOptionTypeSlot()));
    connect(del_option_type_button,SIGNAL(clicked()),this,SLOT(delOptionTypeSlot()));
    connect(add_offer_type_button,SIGNAL(clicked()),this,SLOT(addOfferTypeSlot()));
    connect(del_offer_type_button,SIGNAL(clicked()),this,SLOT(delOfferTypeSlot()));

    connect(add_option_button,SIGNAL(clicked()),this,SLOT(addOptionSlot()));
    connect(del_option_button,SIGNAL(clicked()),this,SLOT(delOptionSlot()));
    connect(add_offer_button,SIGNAL(clicked()),this,SLOT(addOfferSlot()));
    connect(del_offer_button,SIGNAL(clicked()),this,SLOT(delOfferSlot()));

    connect(option_type_listview, &QTreeWidget::itemSelectionChanged,
            this, &ProductInsertionWidget::optionTypeSelectionChangedSlot);

    connect(offer_type_listview, &QTreeWidget::itemSelectionChanged,
            this, &ProductInsertionWidget::offerTypeSelectionChangedSlot);

    connect(option_listview, &QTreeWidget::itemClicked,
            this, &ProductInsertionWidget::clickedOptionSlot);

    connect(offer_listview, &QTreeWidget::itemSelectionChanged,
            this, &ProductInsertionWidget::offerSelectionChangedSlot);

    connect(option_apply_price_button,SIGNAL(clicked()),this,SLOT(applyOptionPriceSlot()));
    connect(offer_apply_price_button,SIGNAL(clicked()),this,SLOT(applyOfferPriceSlot()));

    connect(ok_button,SIGNAL(clicked()),this,SLOT(acceptSlot()));
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancelSlot()));

}

ProductInsertionWidget::~ProductInsertionWidget(){
    clearOptions();
    clearOffers();
    delete float_kb;
    delete float_kb_options;
    delete float_kb_offers;
    delete num_kb_offers;
}

void ProductInsertionWidget::showEvent(QShowEvent *e){
    main_stack->setCurrentWidget(progress);
    clear();
    if (mode == ProductInsertionWidget::Composed)
        QTimer::singleShot(10,this,SLOT(showAction()));
    else
        main_stack->setCurrentWidget(main);
    QWidget::showEvent(e);
}

void ProductInsertionWidget::showAction(){
    progress_label->setText("");
    main_stack->setCurrentWidget(progress);
    getUnitaryLogos();
    main_stack->setCurrentWidget(main);
}

void ProductInsertionWidget::getLogos(){
    logo_view->clear();
    for(const auto& file : QDir(ICON_PATH).entryList(QStringList("*.png"), QDir::Files, QDir::Name)){
        auto absolute_file_path = ICON_PATH + file;
        auto item = new QListWidgetItem(cropedIcon(absolute_file_path, ICON_SIZE),file, logo_view);
        logo_view->addItem(item);
    }
    //set also the null logo icon
    logo_view->addItem(new QListWidgetItem(tr("WITHOUT LOGO"),logo_view));
}

void ProductInsertionWidget::getUnitaryLogos(){

    icon_view->clear();
    XmlConfig xml;
    if(!xml.readXmlFromString(product_model->getUnitaryProducts())){
        cerr << "Cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "Xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
    xml.setDomain("products");
    auto count = xml.howManyTags("product");
    progress_bar->setValue(count);
    for(auto  i=0; i < count; i++){
        xml.setDomain("product[" + QString::number(i) + "]");
        auto icon = ICON_PATH + xml.readString("logo");
        auto name = xml.readString("name");
        auto item = new QListWidgetItem(cropedIcon(icon,ICON_SIZE), name, icon_view);
        icon_view->addItem(item);
        emit progressSignal(i, name);
        xml.releaseDomain("product", false);
    }

}

void ProductInsertionWidget::setProgressSlot(int num, const QString& product){
    progress_bar->setValue(num);
    progress_label->setText(product);
    qApp->processEvents();
}

void ProductInsertionWidget::setMode(int _mode){
    if ( _mode != ProductInsertionWidget::Unitary && _mode != ProductInsertionWidget::Composed ) return;

    mode = (ProductMode) _mode;
    switch (mode){
    case ProductInsertionWidget::Unitary :
        tab->setTabEnabled(tab->indexOf(composition),false);
        title_label->setText(UNITARY_PRODUCT_TITLE);
        this->clearComposedTab();
        logo_button->show();
        break;
    case ProductInsertionWidget::Composed :
        tab->setTabEnabled(tab->indexOf(composition), true);
        title_label->setText(COMPOSED_PRODUCTS_TITLE);
        logo_button->hide();
        getUnitaryLogos();
        break;
    }
}

int ProductInsertionWidget::getMode(){
    return mode;
}

void ProductInsertionWidget::clear(){
    logo.clear();
    product_name_lineedit->clear();
    logo_button->setIcon(QPixmap());
    logo_button->setText(NULL_LOGO_TEXT);
    tab->setCurrentWidget(general);
    clearMainTab();
    clearComposedTab();
    clearOptionTab();
    clearOfferTab();
    ok_button->setEnabled(checkAllValues());
    clearOptions();
    clearOffers();
}

void ProductInsertionWidget::clearMainTab(){
    float_kb->clear();
}

void ProductInsertionWidget::clearComposedTab(){
    table->setRowCount(0);
    search_lineedit->clear();
    icon_view->clear();
}

void ProductInsertionWidget::clearOptionTab(){
    //@benes    option_type_listview->clear();
    option_type_combobox->clear();
    //@benes        option_listview->clear();
    option_combobox->clear();
    float_kb_options->clear();
    option_group_box_price->setEnabled(false);
}

void ProductInsertionWidget::clearOfferTab(){
    offer_type_listview->clear();
    offer_type_combobox->clear();
    offer_listview->clear();
    offer_combobox->clear();
    float_kb_offers->clear();
    num_kb_offers->clear();
    offer_group_box_price->setEnabled(false);
}

void ProductInsertionWidget::getTaxesButtons(){
    QVBoxLayout *vlayout = 0;
    if (!(vlayout =(QVBoxLayout *) tax_group->layout()))
        vlayout = new QVBoxLayout(tax_group);

    XmlConfig xml;
    if(!xml.readXmlFromString(product_model->getTaxes())){
        return;
    }
    if (!xml.validateXmlWithDTD(TAXES_DTD, true)){
        cerr << "Xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }
    xml.setDomain("taxes");
    for(auto i=0; i< xml.howManyTags("tax"); i++){
        auto tpm_str = xml.readString("tax["+ QString::number(i)+"]");
        auto button = new QPushButton(tpm_str, tax_group);
        button->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
        button->setFixedHeight(60);
        button->setCheckable(true);
        if (tpm_str == DEFAULT_TAX){
            button->setPalette(QPalette(BUTTON_ON));
            button->setChecked(true);
        }
        else{
            button->setPalette(QPalette(BUTTON_OFF));
            button->setChecked(false);
        }
        button->setVisible(true);
        connect(button, SIGNAL(clicked()),this,SLOT(taxChanged()));
        vlayout->addWidget(button);
        tax_button_group->addButton(button);
    }
}

void ProductInsertionWidget::clearTaxesButtons(){
    //@benes    while (tax_group->count())
    //@benes        tax_group->remove(tax_group->find(0));

    for(auto* button : tax_button_group->buttons())
        tax_button_group->removeButton(button);
}

void ProductInsertionWidget::taxChanged(){
    for(auto* button : tax_button_group->buttons())
        if(!button->isDown()) button->setPalette(QPalette(BUTTON_ON));
        else button->setPalette(QPalette(BUTTON_OFF));
}

QString ProductInsertionWidget::getActualTax(){
    //@benes    QString ret;
    //    QButton *button = 0;
    //    button  = tax_group->selected();
    //    if (button)
    //        ret = button->name();
    //    return ret;

    auto button = tax_button_group->checkedButton();
    if(button) return button->objectName();
    return QString();
}

void ProductInsertionWidget::numkeyChangedSlot(double num){
    price_label->setText(QString::number(num,'f',2) + tr(" Euros"));
}

void ProductInsertionWidget::optionNumkeyChangedSlot(double num){
    price_label_options->setText(QString::number(num,'f',2)+ tr(" Euros"));
}

void ProductInsertionWidget::offerNumkeyChangedSlot(double num){
    price_label_offers_fixed->setText(QString::number(num,'f',2)+ tr("Euros"));
}

void ProductInsertionWidget::offerNumkeyChangedSlot(int num){
    price_label_offers_percent->setText(QString::number(num)+ " %");
}

void ProductInsertionWidget::logoButtonClicked(){
    pop_logo->setFixedSize(300, 300);
    auto x = frame_logo->mapToGlobal(QPoint(0,0)).x() + frame_logo->height();
    auto y = frame_logo->mapToGlobal(QPoint(0,0)).y();
    pop_logo->exec(QPoint(x,y));
}

void ProductInsertionWidget::showPopLogo(){
    tab->setEnabled(false);
}

void ProductInsertionWidget::hidePopLogo(){
    tab->setEnabled(true);
}

void ProductInsertionWidget::logoClickedSlot(QListWidgetItem* item){

    logo.clear();
    if( item && item->text() != tr("WITHOUT LOGO"))
        logo = item->text();

    pop_logo->hide();
    if (logo.isEmpty()){
        logo_button->setText("Logo");
        return;
    }

    logo_button->setIcon(cropedIcon(ICON_PATH + logo,ICON_BUTTON_SIZE));
}

void ProductInsertionWidget::searchButtonClickedSlot(){
    QString name, code;
    selectProduct(search_lineedit->text());
}

void ProductInsertionWidget::searchEditChangedSlot(const QString &text){
    selectProduct(text);
}

void ProductInsertionWidget::selectProduct(const QString& product){

    auto items = icon_view->findItems(product, Qt::MatchContains);
    if(!items.isEmpty()){
        icon_view->setCurrentItem(items.first());
        items.first()->setHidden(false);
    }
}

void ProductInsertionWidget::upScrollUnitaryViewSlot(){
    icon_view->scroll(0,-70);
}

void ProductInsertionWidget::downScrollUnitaryViewSlot(){
    icon_view->scroll(0,70);
}

void ProductInsertionWidget::draggedText(int x, int y, const QString& text){
    auto row = table->rowCount();
    auto item = new QTableWidgetItem(text);
    table->insertRow(row);
    table->setItem(row, 0, item);
    item = new QTableWidgetItem(DEFAULT_QUANTITY);
    item->setFlags(Qt::ItemIsEditable);
    table->setItem(row, 1, item);
    ok_button->setEnabled(checkAllValues());
}

void ProductInsertionWidget::upTableButtonClicked(){
    auto row = table->currentRow();
    if(!row)  return;
    table->swapRows(row, row - 1); //@ does swap, must be implemented
    for(auto col = 0; col < table->columnCount(); col++){
        table->update(table->model()->index(row - 1, col));
        table->update(table->model()->index(row, col));
    }
}

void ProductInsertionWidget::downTableButtonClicked(){
    auto row = table->currentRow();
    if(row == table->rowCount() - 1) return;
    table->swapRows(row, row + 1); //@ does swap, must be implemented
    for(auto col = 0; col < table->columnCount(); col++){
        table->update(table->model()->index(row + 1, col));
        table->update(table->model()->index(row, col));
    }
}

void ProductInsertionWidget::deleteTableButtonClicked(){
    table->removeRow(table->currentRow());
}

bool ProductInsertionWidget::checkAllValues(){

    bool ret = false;
    auto name = product_name_lineedit->text();
    if ( mode == ProductInsertionWidget::Unitary){
        if ( !name.isEmpty() ) ret =  true;
    }
    else if ( mode == ProductInsertionWidget::Composed){
        if ( !name.isEmpty() && table->rowCount() > 1 )
            ret = true;
    }
    return ret;
}

void ProductInsertionWidget::nameChangedSlot(const QString& text){
    ok_button->setEnabled(checkAllValues());
}

void ProductInsertionWidget::upOptionTypeSlot(){
    selectTreeItemUp(option_type_listview);
}

void ProductInsertionWidget::downOptionTypeSlot(){
    selectTreeItemDown(option_type_listview);
}

void ProductInsertionWidget::upOptionSlot(){
    selectTreeItemUp(option_listview);
}

void ProductInsertionWidget::downOptionSlot(){
    selectTreeItemDown(option_listview);
}

void ProductInsertionWidget::upOfferTypeSlot(){
    selectTreeItemUp(offer_type_listview);
}

void ProductInsertionWidget::downOfferTypeSlot(){
    selectTreeItemDown(offer_type_listview);
}

void ProductInsertionWidget::upOfferSlot(){
    selectTreeItemUp(offer_listview);
}

void ProductInsertionWidget::downOfferSlot(){
    selectTreeItemDown(offer_listview);
}

void ProductInsertionWidget::tabChangedSlot(int index){
    auto w = tab->widget(index);
    if (w == opciones){
        this->clearOptionTab();
        option_type_listview->clearSelection();
        option_listview->clearSelection();
        this->getOptionTypes();
        showOptionTypes();
    }
    else if (w == ofertas){
        clearOfferTab();
        offer_type_listview->clearSelection();
        offer_listview->clearSelection();
        this->getOfferTypes();
        showOfferTypes();
    }
}

//void NAddProductWidget::getOptionTypes(){

//    XmlConfig xml;
//    if(!xml.readXmlFromString(optionModel.getOptionTypes())){
//        cerr << "Can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " <<__LINE__ << endl;
//        return;
//    }

//    if(!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)){
//        cerr << "Xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
//        xml.debug();
//        return;
//    }

//    xml.setDomain("options");
//    QStringList option_types;
//    for(auto i = 0; i < xml.howManyTags("option"); i++)
//        option_types << xml.readString("option[" + QString::number(i) + "].option_type");

//    option_type_combobox->clear();
//    option_type_combobox->addItems(option_types);
//}

//void NAddProductWidget::getOptionsFromType(const QString& type){

////@benes     option_combobox->clear(); moved below
//    XmlConfig xml;
//    if(!xml.readXmlFromString(optionModel.getOptions(type))){
//        cerr << "Can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
//        return;
//    }

//    if(!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)){
//        cerr << "Xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
//        xml.debug();
//        return;
//    }

//    xml.delDomain();
//    xml.setDomain("options");
//    QStringList option_names;
//    for(auto i = 0; i < xml.howManyTags("option"); i++)
//        option_names << xml.readString("option[" + QString::number(i) + "].option_name");

//    option_combobox->clear();
//    option_combobox->addItems(option_names);
//}

void ProductInsertionWidget::getOptionTypes(){
    fillOptions("options","option_type","option", option_type_combobox, product_option_model.getOptionTypes());
}

void ProductInsertionWidget::getOptionsFromType(const QString& type){
    fillOptions("options","option_name","option", option_combobox, product_option_model.getOptions(type));
}

void ProductInsertionWidget::getOfferTypes(){
    fillOptions("offers","offer_type","offer", offer_type_combobox, ProductOfferModule().getOfferTypes());
}

void ProductInsertionWidget::getOffersFromType(const QString& type){
    fillOptions("offers","offer_name","offer", offer_combobox, ProductOfferModule().getOffers(type));
}

void ProductInsertionWidget::showOptionTypes(){
    option_type_listview->clear();
    option_listview->clear();

    for (const auto* op_type : *options_list ){
        auto item = new QTreeWidgetItem(option_type_listview);
        item->setText(0, op_type->type);
    }

    option_group_box_price->setEnabled(isOptionSelected());
    float_kb_options->clear();
}

void ProductInsertionWidget::showOfferTypes(){
    offer_type_listview->clear();
    offer_listview->clear();

    for (const auto* op_type : *offers_list){
        auto item = new QTreeWidgetItem(offer_type_listview);
        item->setText(0, op_type->type);
    }

    offer_group_box_price->setEnabled(isOfferSelected());
    float_kb_offers->clear();

    num_kb_offers->clear();
}

void ProductInsertionWidget::addOptionTypeSlot(){
    auto type = option_type_combobox->currentText();
    if (type.isEmpty()) return;

    NOTypes *op_type = options_list->find(type);
    if ( op_type ) return;
    op_type = new NOTypes;
    op_type->type = type;
    options_list->append(op_type,type);

    showOptionTypes();
}

void ProductInsertionWidget::addOfferTypeSlot(){
    auto type = offer_type_combobox->currentText();
    if (type.isEmpty()) return;

    NOTypes *op_type = offers_list->find(type);
    if (op_type) return;
    op_type = new NOTypes;
    op_type->type = type;
    offers_list->append(op_type,type);

    showOfferTypes();
    option_listview->clear();
}

void ProductInsertionWidget::delOptionTypeSlot(){
    auto items = option_type_listview->selectedItems();
    if (items.isEmpty()) return;
    auto type = items.first()->text(0);
    options_list->remove(type);

    showOptionTypes();
    option_listview->clear();
    option_combobox->clear();
}

void ProductInsertionWidget::delOfferTypeSlot(){
    auto items = offer_type_listview->selectedItems();
    if ( items.isEmpty() ) return;
    auto type = items.first()->text(0);
    offers_list->remove(type);

    showOfferTypes();
    offer_listview->clear();
    offer_combobox->clear();
}

void ProductInsertionWidget::showOptionsFromType(const QString& type){
    option_listview->clear();
    NOTypes *no_type = options_list->find(type);
    if ( !no_type ) return;

    NOData *no_data = getDefaultFromOptionType(no_type->type);
    if ( !no_data && no_type->list.count() ){
        no_data = no_type->list.getFirst();
        no_data->is_default = true;
    }

    for (const auto* no_data : no_type->list){
        if (no_data->state == NOData::Deleted) continue;
        auto aux_double = no_data->value.toDouble();
        auto item = new QTreeWidgetItem(option_listview);

        if (no_data->is_default)
            item->setIcon(0, QPixmap(IS_DEFAULT_OPTION_LOGO));
        else
            item->setIcon(0, QPixmap(NO_DEFAULT_OPTION_LOGO));

        item->setText(1, no_data->name);
        item->setText(2, QString::number(aux_double,'f',2));

    }
    option_group_box_price->setEnabled(isOptionSelected());
    float_kb_options->clear();
}

void ProductInsertionWidget::showOffersFromType(const QString& type){

    offer_listview->clear();
    NOTypes *no_type = offers_list->find(type);
    if (!no_type) return;
    QString cpp = getCurrentCppOperator();

    for (const auto* no_data : no_type->list){
        if (no_data->state == NOData::Deleted) continue;
        auto item = new QTreeWidgetItem(offer_listview);
        item->setText(0, no_data->name);

        if (cpp == "="){
            auto aux_double = no_data->value.toDouble();
            item->setText( 1, QString::number(aux_double,'f',2));
        }
        else if (cpp == "x")
            item->setText( 1, no_data->value+ " %");
    }

    offer_group_box_price->setEnabled(isOfferSelected());
    float_kb_offers->clear();
    num_kb_offers->clear();
}

void ProductInsertionWidget::optionTypeSelectionChangedSlot(){
    auto item = selectedItem();
    if( !item) return;

    auto type = item->text(0);
    getOptionsFromType(type);
    showOptionsFromType(type);
    option_group_box_price->setEnabled(isOptionSelected());
    float_kb_options->clear();
}

void ProductInsertionWidget::offerTypeSelectionChangedSlot(){
    auto item = selectedItem();
    if( !item) return;

    auto type = item->text(0);
    getOffersFromType(type);
    showOffersFromType(type);
    offer_group_box_price->setEnabled(isOfferSelected());
    float_kb_offers->clear();
    num_kb_offers->clear();
}

void ProductInsertionWidget::addOptionSlot(){

    auto items = option_type_listview->selectedItems();
    if (items.isEmpty()) return;
    auto item = items.first();

    NOTypes *no_type = options_list->find(item->text(0));
    if (!no_type) return;

    auto name = option_combobox->currentText();
    if (name.isEmpty()) return;

    NOData *no_data = no_type->list.find(name);
    if (no_data) return;

    no_data = new NOData();
    no_data->name = option_combobox->currentText();
    no_data->value = QString::number(float_kb->getNumber(),'f',2);
    no_data->state = NOData::New;
    no_data->is_default = false;
    no_type->list.append(no_data,no_data->name);
    showOptionsFromType(no_type->type);
    //        item = option_listview->findItem(no_data->name,0);
    //        if (item){
    //                offer_listview->setSelected(item,true);
    //                offer_listview->ensureItemVisible(item);
    //        }
}

void ProductInsertionWidget::delOptionSlot(){
    auto items = option_type_listview->selectedItems();
    if (items.isEmpty()) return;
    auto item = items.first();

    NOTypes *no_type = options_list->find(item->text(0));
    if (!no_type) return;

    items = option_listview->selectedItems();
    if (items.isEmpty()) return;
    item = items.first();

    NOData *no_data = no_type->list.find(item->text(1));
    if (!no_data) return;

    if ( no_data->state != NOData::New){
        no_data->state = NOData::Deleted;
        no_data->is_default = false;
    }
    else{
        no_data = nullptr;
        no_type->list.remove(item->text(1));
    }
    showOptionsFromType(no_type->type);
}

void ProductInsertionWidget::addOfferSlot(){

    auto items = offer_type_listview->selectedItems();
    if (items.isEmpty()) return;
    auto item = items.first();

    NOTypes *no_type = offers_list->find(item->text(0));
    if (!no_type)  return;

    auto name = offer_combobox->currentText();
    if (name.isEmpty()) return;

    NOData *no_data = no_type->list.find(name);
    if ( no_data )  return;

    QString cpp = getCurrentCppOperator();

    no_data = new NOData();
    no_data->name = offer_combobox->currentText();
    if (cpp == "=") no_data->value = "0.00";
    else if (cpp == "x") no_data->value = "0";

    no_data->state = NOData::New;
    no_type->list.append(no_data,no_data->name);

    showOffersFromType(no_type->type);
    //        item = offer_listview->findItem(no_data->name,0);
    //        if (item){
    //                offer_listview->setSelected(item,true);
    //                offer_listview->ensureItemVisible(item);
    //        }
}

void ProductInsertionWidget::delOfferSlot(){

    auto items = offer_type_listview->selectedItems();
    if (items.isEmpty()) return;
    auto item = items.first();

    NOTypes *no_type = offers_list->find(item->text(0));
    if (!no_type)  return;

    items = offer_listview->selectedItems();
    if (items.isEmpty()) return;
    item = items.first();

    NOData *no_data = no_type->list.find(item->text(0));
    if (!no_data) return;

    if (no_data->state !=NOData::New)
        no_data->state = NOData::Deleted;
    else
    {
        no_data = 0;
        no_type->list.remove(item->text(0));
    }

    showOffersFromType(no_type->type);
}

NOData* ProductInsertionWidget::getDefaultFromOptionType(const QString& type){
    NOTypes *no_type  =  options_list->find(type);
    if (!no_type)  return nullptr;

    for (auto * no_data : no_type->list)
        if ( no_data->is_default ) return no_data;
    return nullptr;
}


void ProductInsertionWidget::clickedOptionSlot(QTreeWidgetItem * item, int column ){

    if (! item)  return;
    if (column == 0){
        auto items = option_type_listview->selectedItems();
        if ( items.isEmpty() )  return;
        auto type_item = items.first();

        NOTypes *no_type = options_list->find(type_item->text(0));
        NOData *no_data = no_type->list.find(item->text(1));

        if (!no_data) return;
        if (! no_data->is_default )
        {
            NOData *tmp_no_data = getDefaultFromOptionType(no_type->type);
            if (tmp_no_data){
                if ( tmp_no_data->state == NOData::Original ) tmp_no_data->state = NOData::Modified;
                tmp_no_data->is_default = false;
            }

            if ( no_data->state == NOData::Original) no_data->state = NOData::Modified;
            no_data->is_default = true;
        }
        showOptionsFromType(no_type->type);
        return;
    }
    option_group_box_price->setEnabled(isOptionSelected());
    float_kb_options->clear();
}

void ProductInsertionWidget::offerSelectionChangedSlot(){
    auto item = selectedItem();

    QString cpp_operator = getCurrentCppOperator();
    if ( !item  || cpp_operator.isEmpty() ) return;

    if (cpp_operator == "x"){
        advertise_offer_label->setText(tr("Oferta a tipo porcentual\nintroduzca porcentaje"));
        num_kb_offers->show();
        float_kb_offers->hide();
        price_label_offers_percent->show();
        price_label_offers_fixed->hide();
    }
    else if (cpp_operator == "="){
        advertise_offer_label->setText(tr("Oferta a precio fijo\nintroduzca el precio"));
        num_kb_offers->hide();
        float_kb_offers->show();
        price_label_offers_percent->hide();
        price_label_offers_fixed->show();
    }
    offer_group_box_price->setEnabled(isOfferSelected());
    float_kb_offers->clear();
    num_kb_offers->clear();
}

bool ProductInsertionWidget::isOptionSelected(){
    return !option_type_listview->selectedItems().isEmpty() && !option_listview->selectedItems().isEmpty();
}

bool ProductInsertionWidget::isOfferSelected(){

    return !offer_type_listview->selectedItems().isEmpty() && !offer_listview->selectedItems().isEmpty();
}

void ProductInsertionWidget::clearOptions(){
    for( auto* item : *options_list)
        delete item;
    options_list->clear();
}

 void ProductInsertionWidget::clearOffers(){
     for( auto* item : *offers_list)
         delete item;
     offers_list->clear();
 }

void ProductInsertionWidget::applyOptionPriceSlot(){

    if( !isOptionSelected() ) return;

    NOTypes *no_type = options_list->find( option_type_listview->selectedItems().first()->text(0) );
    if (!no_type) return;

    NOData *no_data = no_type->list.find( option_listview->selectedItems().first()->text(1));
    if (!no_data) return;

    if (no_data->state == NOData::Original)
        no_data->state = NOData::Modified;
    no_data->value = QString::number(float_kb_options->getNumber(),'f',2);
    showOptionsFromType(no_type->type);
}

void ProductInsertionWidget::applyOfferPriceSlot(){

    QString cpp_operator;
    if( !isOfferSelected() ) return;

    NOTypes *no_type = offers_list->find(offer_type_listview->selectedItems().first()->text(0));
    if (!no_type) return;

    NOData *no_data = no_type->list.find(offer_listview->selectedItems().first()->text(0));
    if (!no_data)  return;

    if (no_data->state == NOData::Original) no_data->state = NOData::Modified;
    cpp_operator = getCurrentCppOperator();

    if (cpp_operator == "x")  no_data->value = QString::number(num_kb_offers->getNumber());
    else if (cpp_operator == "=") no_data->value = QString::number(float_kb_offers->getNumber(),'f', 2);
    showOffersFromType(no_type->type);
}

QString ProductInsertionWidget::getCurrentCppOperator(){

    auto items = offer_type_listview->selectedItems();
    if ( items.isEmpty())
        return QString();
    auto type_item = items.first();

    NOTypes *no_type = offers_list->find(type_item->text(0));
    if (!no_type) return QString();

    return ProductOfferModule().getCppOperator(no_type->type);
}

void ProductInsertionWidget::save(){
    if ( !saveProduct() ) return;
    if (!saveProductComposition()) return;
    saveOptions();
    saveOffers();
    product_model->setProductAtPrinter( product_name_lineedit->text(), "kitchen", kitchen_checkbox_button->isChecked());
}

bool ProductInsertionWidget::saveProduct(){

    auto product_code = getCurrentProductCode();
    if (product_code.isEmpty()){
        QString text = tr("There is no product code. Maybe you have not entered the name\n or the composition of the product");
        QMessageBox::information(this, tr("Can't insert product"), text, QMessageBox::Ok);
        return false;
    }

    QString p_name = product_model->getProductName(product_code);
    if ( !p_name.isEmpty() ){
        //the product exists
        if ( !mode ){
            auto text = tr("This product cannot be stored due to existing \n unitary product called : %1 with the same product code.\n Try to change the name").arg(p_name);
            QMessageBox::information(this, tr("Can't save product"), text, QMessageBox::Ok);
            return false;
        }
        else{
            auto msg = tr("There is already a product with this composition with the name : %1\n Do you want to update the product : %2 ?").arg(p_name).arg(p_name);
            if( QMessageBox::question( this, tr("Update product"), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No ) return false;
            product_model->deleteProduct(product_code);
        }
    }

    XmlConfig xml;
    xml.createElementSetDomain("products");
    xml.createElementSetDomain("product");
    xml.createElement("code", product_code);
    xml.createElement("name",product_name_lineedit->text());
    xml.createElement("price", QString::number(float_kb->getNumber(),'f',2));
    xml.createElement("tax", getActualTax());
    xml.createElement("logo", logo);
    if (!mode)
        xml.createElement("description", product_name_lineedit->text()+"_unitary_product");
    else
        xml.createElement("description", product_name_lineedit->text()+"_composed_product");
    xml.releaseDomain("products");

    return product_model->insertProduct(xml.toString());
}

bool ProductInsertionWidget::saveProductComposition(){

    auto product_code = getCurrentProductCode();
    if (product_code.isEmpty()) return false;

    XmlConfig xml;
    xml.createElementSetDomain("composition");
    xml.createElement("composition_code", product_code);
    if ( !mode ){ //unitary
        xml.createElementSetDomain("ingredient");
        xml.createElement("ingredient_code", product_code);
        xml.createElement("quantity", "1.0");
        xml.releaseDomain("composition");
    }
    else{  //composed
        for ( auto i=0; i< table->rowCount(); i++) {
            auto p_name = table->item(i,0)->text();
            auto p_quantity = table->item(i,1)->text();
            auto p_code = product_model->getProductCodeByProductName(p_name);
            xml.createElementSetDomain("ingredient");
            xml.createElement("ingredient_code",p_code);
            xml.createElement("quantity", p_quantity);
            xml.releaseDomain("ingredient");
        }
        xml.releaseDomain("composition");
    }

    return product_model->insertProductComposition(xml.toString());
}

bool ProductInsertionWidget::saveOptions(){
    double aux_value=0.0;
    bool ret=true;

    auto product_code = getCurrentProductCode();
    if (product_code.isEmpty())  return false;
    ProductOptionModule option_module;

    for (auto* no_type : *options_list){
        for (const auto* no_data : no_type->list){
            switch (no_data->state){
            case NOData::Original:
                if (!mode) continue;
                break;
            case NOData::New :
            {
                XmlConfig xml;
                xml.createElement("product_code", product_code);
                xml.createElementSetDomain("options.option");
                xml.createElement("option_type", no_type->type);
                xml.createElement("description_type", no_type->type+"_option_type");
                xml.createElement("option_name", no_data->name);
                xml.createElement("description_option", no_data->name+"option");
                aux_value = (double) ((no_data->value).toDouble() -  float_kb->getNumber());
                xml.createElement("value", QString::number(aux_value,'f',2));

                if(no_data->is_default) xml.createElement("default", "t");
                else xml.createElement("default", "f");
                xml.releaseDomain("options");
                auto xml_string = xml.toString();
                if (!option_module.insertOptionsToProduct(xml_string))
                {
                    cerr << "Failed while trying to save new option "<< no_type->type.toStdString() << " " << no_data->name.toStdString() << endl;
                    ret = false;
                }
            }

                break;
            case NOData::Modified :
            {
                XmlConfig xml;
                xml.createElement("product_code", product_code);
                xml.createElementSetDomain("options.option");
                xml.createElement("option_type", no_type->type);
                xml.createElement("description_type", no_type->type+"_option_type");
                xml.createElement("option_name", no_data->name);
                xml.createElement("description_option", no_data->name+"option");
                aux_value = (double) ((no_data->value).toDouble() -  float_kb->getNumber());
                xml.createElement("value", QString::number(aux_value,'f',2));

                if(no_data->is_default)
                    xml.createElement("default", "t");
                else
                    xml.createElement("default", "f");
                xml.releaseDomain("options");
                auto xml_string = xml.toString();
                xml.debug();

                if ( !option_module.updateOptionsToProduct(xml_string)){
                    cerr << "Failed while trying to update the option "  << no_type->type.toStdString() << " " << no_data->name.toStdString() << endl;
                    ret = false;
                }
            }

                break;
            case NOData::Deleted:
                if ( !option_module.deleteOptionsToProduct(product_code,no_type->type,no_data->name) ){
                    cerr << "fail while trying to del the option " << no_type->type .toStdString() << " " << no_data->name.toStdString() << endl;
                    ret = false;
                }
                break;
            }
        }
    }
    return ret;
}

bool ProductInsertionWidget::saveOffers(){
    double aux_value=0.0;
    bool ret=true;
    int types_count;
    QString aux_cpp;

    QString xml_string;


    auto product_code = getCurrentProductCode();
    if (product_code.isEmpty())  return false;
    ProductOfferModule offer_module;

    for (auto *no_type : *offers_list){
        for (const auto* no_data : no_type->list){
            switch (no_data->state){
            case NOData::Original:
                if (!mode) continue;
            break;

            case NOData::New:
            {
                XmlConfig xml;
                xml.createElement("product_code", product_code);
                xml.createElementSetDomain("offers.offer");
                xml.createElement("offer_type", no_type->type);
                xml.createElement("offer_name", no_data->name);
                auto aux_cpp = offer_module.getCppOperator(no_type->type);

                if (aux_cpp == "x"){
                    auto aux_value = (double) (no_data->value).toDouble() / 100;
                    xml.createElement("value", QString::number(aux_value,'f', 2));
                }
                else if (aux_cpp == "=")
                    xml.createElement("value", no_data->value);

                xml.releaseDomain("offers");
                auto xml_string = xml.toString();
                if (!offer_module.insertOffersToProduct(xml_string)){
                    cerr << "fail while trying to save the offer "  <<(no_type->type).toStdString()<< " "<<(no_data->name).toStdString()<< endl;
                    ret = false;
                }
            }

                break;

            case NOData::Modified:
            {
                XmlConfig xml;
                xml.createElement("product_code", product_code);
                xml.createElementSetDomain("offers.offer");
                xml.createElement("offer_type", no_type->type);
                xml.createElement("offer_name", no_data->name);
                auto aux_cpp = offer_module.getCppOperator(no_type->type);

                if (aux_cpp == "x"){
                    auto aux_value = (double) (no_data->value).toDouble() / 100;
                    xml.createElement("value", QString::number(aux_value,'f',2));
                }
                else if (aux_cpp == "=")
                    xml.createElement("value", no_data->value);

                xml.releaseDomain("offers");
                auto xml_string = xml.toString();

                if (!offer_module.updateOffersToProduct(xml_string)){
                    cerr << "fail while trying to update the offer "  <<(no_type->type).toStdString()<< " "<<(no_data->name).toStdString()<< endl;
                    ret = false;
                }
            }

                break;

            case NOData::Deleted:
                if ( !offer_module.deleteOffersToProduct(product_code,no_type->type, no_data->name) ){
                    cerr << "Failed while trying to del the offer "  << no_type->type.toStdString() << " " << no_data->name.toStdString() << endl;
                    ret = false;
                }
                break;
            }
        }
    }
    return ret;
}

QString ProductInsertionWidget::getCurrentProductCode(){

    QString product_name;
    QString product_code;
    QStringList product_codes;
    if (mode == ProductInsertionWidget::Unitary){
        product_name = product_name_lineedit->text();
        product_code = product_model->getProductCodeByProductName(product_name);
        if (product_code.isEmpty()){    //create new product_code
            product_code = product_name_lineedit->text();
            product_code = product_code.toLower().simplified().replace(" ", "_");
        }
    }
    else if (mode == ProductInsertionWidget::Composed){
        auto count = table->rowCount();
        if ( count <= 1) return product_code;
        for (auto i=0; i < count; i++ ){
            product_name = table->item(i,0)->text();
            product_code = product_model->getProductCodeByProductName(product_name);

            if ( product_code.isEmpty() ) return QString();
            product_codes << product_code;
        }

        product_codes.sort();
        product_code = product_codes.join(" ");
    }
    return product_code;
}

void ProductInsertionWidget::timerSlot(){
    aux_counter++;
    progress_bar->setValue(aux_counter);
    qApp->processEvents();
}

void ProductInsertionWidget::acceptSlot(){
    main_stack->setCurrentWidget(progress);
    progress_title_label->setText(tr("guardando el producto"));
    progress_label->setText(product_name_lineedit->text());
    aux_counter = 0;
    progress_bar->setValue(0);
    qApp->processEvents();
    connect(timer, SIGNAL(timeout()), this,SLOT(timerSlot()));
    timer->start(5);
    save();
    clear();
    timer->stop();
    disconnect(timer, SIGNAL(timeout()), this,SLOT(timerSlot()));
    progress_title_label->setText(tr("Loading logos of unitary products"));

    if (mode == ProductInsertionWidget::Composed)
        QTimer::singleShot(10,this,SLOT(showAction()));
    else
        main_stack->setCurrentWidget(main);
    emit acceptSignal();
}

void ProductInsertionWidget::cancelSlot(){
    clear();
    if (mode == ProductInsertionWidget::Composed)
        QTimer::singleShot(10,this,SLOT(showAction()));
    else
        main_stack->setCurrentWidget(main);
    emit cancelSignal();
}

void ProductInsertionWidget::fillOptions(
        const QString& domain,  const QString& sub_domain,
        const QString& tag, QComboBox* cbx, const QString& data)
{
    XmlConfig xml;
    if(!xml.readXmlFromString(data)){
        cerr << "Can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " <<__LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)){
        cerr << "Xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.setDomain(domain);
    QStringList option_types;
    auto left = tag + "[";
    auto right = "]." + sub_domain;
    for(auto i = 0; i < xml.howManyTags(tag); i++)
        option_types << xml.readString( left + QString::number(i) + right);

    cbx->clear();
    cbx->addItems(option_types);
}
QTreeWidgetItem* ProductInsertionWidget::selectedItem() const{
    auto source = qobject_cast<QTreeWidget*>(sender());

    if( !source || source->selectedItems().isEmpty()) return nullptr;
    return source->selectedItems().first();

}
