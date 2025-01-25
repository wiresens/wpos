/***************************************************************************
                          bslscreenproductsdesignwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productscreendesignwidget.h"

#include "menusystem/utils.h"

#include "database/productsmoduledb.h"
#include "productsmodule/productmodule.h"

#include <wposcore/config.h>
#include <wposgui/common/dragobjects.h>

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListView>
#include <QLocale>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTimer>

#include <iostream>
#include <stdio.h>
using namespace std;

static const QString& SCREEN_PRODUCTS_LIST_DTD = "dtddocs:products_productslist.dtd";
static const QString& SCREENS_XML = "xmldocs:bar.xml";
static const QString& ICON_PATH = cfg::PRODUCT_DIR;
static QString DEFAULT_NAME = QObject::tr("Current Screen");

static const double SIZE_CONST = 40.00;
static const int TABLE_SIZE_X = 70;
static const int TABLE_SIZE_Y = 70;
static const uint TIME_OUT { 10 };

struct ProductNode {
    ProductData product;
    int row {};
    int col {};
};

struct ScreenNode {
    HList<ProductNode> product_list;
    QString name;
    QString next_screen;
    int rows { 6 };
    int cols { 6 };
    bool is_default { false };
    bool showtext { false };
};

ProductScreenDesignWidget::ProductScreenDesignWidget(QWidget* parent, const QString& name)
    : QWidget(parent)
    , screen_list { new HList<ScreenNode> }
{
    setupUi(this);
    setObjectName(name);

    pix_label1->setPixmap(QPixmap("controls32:info.png"));
    pix_label2->setPixmap(QPixmap("controls32:ktip.png"));
    pix_label3->setPixmap(QPixmap("controls32:ktip.png"));
    pix_label4->setPixmap(QPixmap("controls32:info.png"));
    pix_label5->setPixmap(QPixmap("controls32:next.png"));
    pix_label6->setPixmap(QPixmap("controls32:info.png"));

    delete_button->setIcon(QPixmap("controls48:trash.png"));
    back_button->setIcon(QPixmap("controls48:back.png"));
    forward_button->setIcon(QPixmap("controls48:forward.png"));
    accept_button->setIcon(QPixmap("controls48:button_ok_48.png"));

    next_screen_button->setIcon(QPixmap("controls48:play_next.png"));
    prev_screen_button->setIcon(QPixmap("controls48:play_prev.png"));
    down_icon_button->setIcon(QPixmap("controls32:down.png"));
    up_icon_button->setIcon(QPixmap("controls32:up.png"));

    // compose and fill the iconview frame
    QHBoxLayout* layout {};
    products = new BslDDIconView(frame_products, "screen");
    if (!(layout = (QHBoxLayout*)frame_products->layout()))
        layout = new QHBoxLayout(frame_products);
    layout->addWidget(products);

    // compose and fill the iconview frame
    screen = new BslDDTable(frame_screen, "screen");
    if (!(layout = (QHBoxLayout*)frame_screen->layout()))
        layout = new QHBoxLayout(frame_screen);
    layout->addWidget(screen);

    screen->setAcceptDrops(true);
    screen->setDragEnabled(true);
    screen->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    screen->setFont(QFont("Helvetica", 8, QFont::Bold));
    screen->setSelectionMode(QTableWidget::NoSelection);

    screen->setContentsMargins(0, 0, 0, 0);
    //@benes    screen->setLeftMargin(0);
    //@benes    screen->setTopMargin(0);
    //@benes    screen->setReadOnly(true);

    //@benes    screen->setRowMovingEnabled(true);
    //@benes    screen->setColumnMovingEnabled(true);

    //@benes    products->setGridX(TABLE_SIZE_X);
    //@benes    products->setGridY(TABLE_SIZE_Y);

    screens->setDuplicatesEnabled(false);
    name_screen->setDuplicatesEnabled(false);

    connect(name_screen, &QComboBox::currentTextChanged, this, &ProductScreenDesignWidget::nameScreenChanged);

    connect(num_rows, QOverload<int>::of(&QSpinBox::valueChanged), this, &ProductScreenDesignWidget::numRowsChanged);
    connect(num_cols, QOverload<int>::of(&QSpinBox::valueChanged), this, &ProductScreenDesignWidget::numColumnsChanged);

    connect(accept_button, &QPushButton::clicked, this, &ProductScreenDesignWidget::acceptReleased);
    connect(forward_button, &QPushButton::clicked, this, &ProductScreenDesignWidget::forwardReleased);
    connect(back_button, &QPushButton::clicked, this, &ProductScreenDesignWidget::backReleased);
    connect(delete_button, &QPushButton::clicked, this, &ProductScreenDesignWidget::deleteSlot);
    connect(next_screen_button, &QPushButton::clicked, this, &ProductScreenDesignWidget::nextScreenSlot);
    connect(prev_screen_button, &QPushButton::clicked, this, &ProductScreenDesignWidget::prevScreenSlot);
    connect(up_icon_button, &QPushButton::clicked, this, &ProductScreenDesignWidget::upSlot);
    connect(down_icon_button, &QPushButton::clicked, this, &ProductScreenDesignWidget::downSlot);

    connect(this, &ProductScreenDesignWidget::productReaded, this, &ProductScreenDesignWidget::productReadedSlot);

    connect(screen, QOverload<int, int, const QString&>::of(&BslDDTable::textEnter), this, &ProductScreenDesignWidget::draggedText);
}

void ProductScreenDesignWidget::clear()
{
    name_screen->clear();
    screens->clear();

    name_screen->setCurrentText("");
    screens->insertItem(0, "");
    screens->setCurrentText("");
    screen_label->setText("");

    products->clearSelection();

    num_rows->setValue(1);
    num_cols->setValue(1);

    clearTable();

    screen_stack->setCurrentWidget(select_rows_cols);

    accept_button->hide();
    back_button->hide();
    forward_button->show();
    forward_button->setEnabled(false);
    screen_default->setChecked(false);
    delete_button->setEnabled(false);
}

void ProductScreenDesignWidget::clearTable()
{

    for (auto i = 0; i < screen->rowCount(); i++)
        screen->removeRow(i);

    screen->setRowCount(0);
    screen->setColumnCount(0);
}

void ProductScreenDesignWidget::forwardReleased()
{
    ScreenNode* screen_node = 0;

    screen_node = screen_list->find(name_screen->currentText());
    if (!screen_node) {
        // create a new entry with the values at the ui
        screen_node = new ScreenNode();
        screen_node->name = name_screen->currentText();
        screen_list->append(screen_node, screen_node->name);
    }

    forward_button->hide();
    back_button->show();
    accept_button->show();
    accept_button->setEnabled(true);
    delete_button->setEnabled(true);

    screen_node->rows = num_rows->value();
    screen_node->cols = num_cols->value();
    screen_node->next_screen = screens->currentText();
    screen_node->is_default = screen_default->isChecked();
    screen_node->showtext = showtext_checkbox->isChecked();

    setProductScreen(name_screen->currentText());
    screen_stack->setCurrentWidget(screen_design);
}

void ProductScreenDesignWidget::backReleased()
{
    screen_stack->setCurrentWidget(select_rows_cols);

    back_button->hide();
    forward_button->show();
    forward_button->setEnabled(true);
    delete_button->setEnabled(false);
    accept_button->hide();

    clear();
    initScreens();
    actual_namestring.clear();
}

void ProductScreenDesignWidget::nextScreenSlot()
{

    if (actual_namestring.isEmpty())
        return;

    auto pos = screen_list->indexOf(actual_namestring);
    if (pos < 0)
        return;

    auto count = screen_list->count() - 1;
    if (pos == count)
        pos = 0;
    else
        pos++;

    auto screen_node = screen_list->at(pos);
    if (!screen_node)
        return;

    setProductScreen(screen_node->name);
}

void ProductScreenDesignWidget::prevScreenSlot()
{

    if (actual_namestring.isEmpty())
        return;

    auto pos = screen_list->indexOf(actual_namestring);
    if (pos < 0)
        return;

    auto count = screen_list->count() - 1;
    if (pos == 0)
        pos = count;
    else
        pos--;

    auto screen_node = screen_list->at(pos);
    if (!screen_node)
        return;
    setProductScreen(screen_node->name);
}

void ProductScreenDesignWidget::acceptReleased()
{
    accept_button->setEnabled(false);
    writeScreensAtXml();
    clear();
    initScreens();
    screen_stack->setCurrentWidget(select_rows_cols);
    accept_button->setEnabled(true);
}

void ProductScreenDesignWidget::initUnitaryProducts()
{
    auto product_mod = new ProductModule();
    auto xml_string = product_mod->getUnitaryProducts();
    delete product_mod;

    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if (!xml.validateXmlWithDTD(SCREEN_PRODUCTS_LIST_DTD)) {
        cerr << "The file does not conform a valid xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    xml.delDomain();
    xml.setDomain("products[0]");
    products->clear();

    BslDDIconViewItem* item {};
    for (auto index = 0; index < xml.howManyTags("product"); index++) {
        xml.setDomain("product[" + QString::number(index) + "]");
        if (xml.readString("logo").isEmpty()) {
            item = new BslDDIconViewItem(products, xml.readString("name"));
            xml.releaseDomain("product", false);
            continue;
        }

        auto iconfile = ICON_PATH + xml.readString("logo");
        item = new BslDDIconViewItem(products, xml.readString("name"), cropedIcon(iconfile, SIZE_CONST));
        products->insertItem(0, item);
        xml.releaseDomain("product", false);
    }

    item = new BslDDIconViewItem(products, "solo");
    products->insertItem(0, item);
    xml.releaseDomain("products", true);
}

void ProductScreenDesignWidget::productReadedSlot(
    int num,
    const QString& screen_name,
    const QString& product_name,
    const QString& logo)
{
    progress->setValue(num);
    screen_name_label->setText(screen_name);
    product_name_label->setText(product_name);
    if (!logo.isEmpty())
        product_logo_label->setPixmap(QPixmap("products:" + logo));
    else
        product_logo_label->clear();
    qApp->processEvents();
}

void ProductScreenDesignWidget::getScreens()
{

    screen_list->clear();
    XmlConfig xml(SCREENS_XML);
    xml.delDomain();

    if (!xml.setDomain("screens"))
        return;
    screen_stack->setCurrentWidget(reading_page);

    QStringList product_list { xml.findNode("product") };
    progress->setValue(product_list.count());
    product_list.clear();

    int pos = 0;
    auto default_screen = xml.readString("defaultscreen");
    for (auto i = 0; i < xml.howManyTags("screen"); i++) {

        if (!isVisible()) {
            screen_list->clear();
            return;
        }

        xml.setDomain("screen[" + QString::number(i) + "]");
        auto screen_node = new ScreenNode();
        screen_node->name = xml.readString("name");
        screen_node->next_screen = xml.readString("nextscreen");
        screen_node->cols = xml.readString("cols").toInt();
        screen_node->rows = xml.readString("rows").toInt();
        screen_node->is_default = false;
        screen_node->showtext = false;

        if (xml.readString("showtext").toLower() == "true")
            screen_node->showtext = true;
        if (screen_node->name == default_screen)
            screen_node->is_default = true;

        // check products
        ProductData* product {};
        for (auto j = 0; j < xml.howManyTags("product"); j++) {
            if (!isVisible()) {
                screen_list->clear();
                return;
            }
            auto tmp_str = xml.readString("product[" + QString::number(j) + "].name");
            if (tmp_str == "solo") {
                product = new ProductData;
                product->code = "solo";
                product->name = "solo";
            } else
                product = getProduct(tmp_str);
            if (!product)
                continue;

            auto product_node = new ProductNode();
            product_node->col = xml.readAttribute("product[" + QString::number(j) + "]", "col").toInt();
            product_node->row = xml.readAttribute("product[" + QString::number(j) + "]", "row").toInt();
            tmp_str = xml.readString("product[" + QString::number(j) + "].name");

            product_node->product = *product;
            delete product;
            product = nullptr;

            tmp_str = QString::number(product_node->row) + "-" + QString::number(product_node->col);
            screen_node->product_list.append(product_node, tmp_str);
            emit productReaded(++pos, screen_node->name, product_node->product.name, product_node->product.logo);
        }
        xml.releaseDomain("screen");
        screen_list->append(screen_node, screen_node->name);
    }
    screen_stack->setCurrentWidget(select_rows_cols);
}

void ProductScreenDesignWidget::writeScreensAtXml()
{

    QString fontsize, textfontsize, fontfamily, textfontfamily, textbackgroundcolor;

    XmlConfig xml(SCREENS_XML);

    if (xml.setDomain("screens")) {
        fontsize = xml.readString("fontsize");
        textfontsize = xml.readString("textfontsize");
        fontfamily = xml.readString("fontfamily");
        textfontfamily = xml.readString("textfontfamily");
        textbackgroundcolor = xml.readString("textbackgroundcolor");
    }

    xml.delDomain();
    while (xml.howManyTags("screens"))
        xml.deleteElement("screens");

    xml.createElementSetDomain("screens");

    if (!fontsize.isEmpty())
        xml.createElement("fontsize", fontsize);

    if (!textfontsize.isEmpty())
        xml.createElement("textfontsize", textfontsize);

    if (!fontfamily.isEmpty())
        xml.createElement("fontfamily", fontfamily);

    if (!textfontfamily.isEmpty())
        xml.createElement("textfontfamily", textfontfamily);

    if (!textbackgroundcolor.isEmpty())
        xml.createElement("textbackgroundcolor", textbackgroundcolor);

    for (auto* screen_node : *screen_list) {

        if (screen_node->is_default)
            xml.doWrite("defaultscreen", screen_node->name);

        xml.createElementSetDomain("screen");
        xml.createElement("name", screen_node->name);
        xml.createElement("cols", QString::number(screen_node->cols));
        xml.createElement("rows", QString::number(screen_node->rows));

        if (!(screen_node->next_screen).isEmpty())
            xml.createElement("nextscreen", screen_node->next_screen);
        if (screen_node->showtext)
            xml.createElement("showtext", "true");
        else
            xml.createElement("showtext", "false");

        for (auto product : screen_node->product_list) {
            if (!product)
                continue;

            xml.createElementSetDomain("product");
            xml.createAttributeHere("row", QString::number(product->row));
            xml.createAttributeHere("col", QString::number(product->col));
            xml.createElement("name", product->product.code);
            xml.releaseDomain("product");
        }
        xml.releaseDomain("screen");
    }
    xml.releaseDomain("screens");
    xml.save();
}

void ProductScreenDesignWidget::nameScreenChanged(const QString& text)
{

    if (text.isEmpty()) {
        forward_button->setEnabled(false);
        return;
    }

    if (num_rows->value() == 0)
        forward_button->setEnabled(false);
    if (num_cols->value() == 0)
        forward_button->setEnabled(false);

    screen_label->setText(text);
    forward_button->setEnabled(true);

    // check if the scren already exists;
    ScreenNode* screen_node = screen_list->find(text);
    if (screen_node) {
        num_cols->setValue(screen_node->cols);
        num_rows->setValue(screen_node->rows);
        screen_default->setChecked(screen_node->is_default);
        showtext_checkbox->setChecked(screen_node->showtext);
        screens->clear();
        screens->setDuplicatesEnabled(false);

        screens->insertItem(0, "");
        for (auto* aux_screen_node : *screen_list)
            if (aux_screen_node->name != screen_node->name)
                screens->insertItem(0, aux_screen_node->name);

        screens->setCurrentText(screen_node->next_screen);
        screens->setDuplicatesEnabled(false);
        delete_button->setEnabled(true);
    } else {
        screens->clear();
        screens->setDuplicatesEnabled(false);
        for (auto* aux_screen_node : *screen_list) {
            screens->insertItem(0, aux_screen_node->name);
            screens->insertItem(0, "");
            screens->setCurrentText("");
        }
    }
}

void ProductScreenDesignWidget::numColumnsChanged(int value)
{
    forward_button->setEnabled(true);
    if (value == 0 || num_rows->value() == 0 || name_screen->currentText().isEmpty())
        forward_button->setEnabled(false);
}

void ProductScreenDesignWidget::numRowsChanged(int value)
{
    forward_button->setEnabled(true);
    if (value == 0 || num_cols->value() == 0 || name_screen->currentText().isEmpty())
        forward_button->setEnabled(false);
}

void ProductScreenDesignWidget::initScreens()
{

    disconnect(name_screen, &QComboBox::currentTextChanged, this, &ProductScreenDesignWidget::nameScreenChanged);
    screens->setDuplicatesEnabled(false);
    name_screen->setDuplicatesEnabled(false);

    name_screen->clear();
    screens->clear();

    for (auto* screen_node : *screen_list)
        name_screen->insertItem(0, screen_node->name);

    name_screen->setCurrentText("");
    screen_label->setText("");
    screens->insertItem(0, "");
    screens->setCurrentText("");
    num_cols->setValue(1);
    num_rows->setValue(1);
    screen_default->setChecked(false);
    showtext_checkbox->setChecked(false);
    connect(name_screen, &QComboBox::currentTextChanged, this, &ProductScreenDesignWidget::nameScreenChanged);
}

void ProductScreenDesignWidget::draggedText(
    int x,
    int y,
    const QString& text)
{

    double size = SIZE_CONST;
    auto header_view = screen->verticalHeader();
    auto pos_x = x - header_view->width();

    header_view = screen->horizontalHeader();
    auto pos_y = y - header_view->height();

    auto row = screen->rowAt(pos_y);
    auto col = screen->columnAt(pos_x);

    if (row < 0 || col < 0) {
        cerr << "Not valid Drop location " << __PRETTY_FUNCTION__ << __LINE__ << endl;
        return;
    }

    if (actual_namestring.isEmpty())
        return;
    size = actual_size;

    ProductData* product {};
    if (text == "solo") {
        product = new ProductData;
        product->code = "solo";
        product->name = "solo";
    } else
        product = getProductWithName(text);

    if (!product)
        return;

    ScreenNode* screen_node = screen_list->find(actual_namestring);
    ProductNode* product_node = screen_node->product_list.find(QString::number(row) + "-" + QString::number(col));
    if (product_node) {
        screen_node->product_list.remove(QString::number(row) + "-" + QString::number(col));
    }
    product_node = new ProductNode();
    product_node->product = *product;
    product_node->col = col;
    product_node->row = row;
    screen_node->product_list.append(product_node, QString::number(row) + "-" + QString::number(col));

    auto item = new QTableWidgetItem(QString {}, 0);
    if ((product->logo).isEmpty()) {
        item->setText(product->name);
        //@benes        item->setWordWrap(true);
    } else {
        auto iconfile = ICON_PATH + product->logo;
        item->setIcon(cropedIcon(iconfile, size));
    }

    screen->setItem(row, col, item);
    //@benes    screen->ensureCellVisible(row, col);
    screen->setCurrentCell(row, col);
    accept_button->setEnabled(true);
}

void ProductScreenDesignWidget::setProductScreen(const QString& screen_name)
{
    int i, count, max;
    QTableWidgetItem* item = 0;
    ProductNode* product_node = 0;

    clearTable();
    double size = ((int)SIZE_CONST) + 4;
    ScreenNode* screen_node = screen_list->find(screen_name);
    if (!screen_node) {
        // create a new entry with the values at the ui
        screen_node = new ScreenNode();
        screen_node->name = screen_name;
        screen_list->append(screen_node, screen_node->name);
    }
    actual_namestring = screen_name;
    actual_namescreen_label->setText(actual_namestring);

    if (screen_node->cols > screen_node->rows)
        max = screen_node->cols;
    else
        max = screen_node->rows;
    actual_size = (int)((screen->width() - 15) / max);
    size = actual_size;

    screen->setRowCount(screen_node->rows);
    screen->setColumnCount(screen_node->cols);

    for (i = 0; i < screen_node->cols; i++)
        screen->setColumnWidth(i, size);

    for (i = 0; i < screen_node->rows; i++)
        screen->setRowHeight(i, size);

    count = screen_node->product_list.count();
    for (i = 0; i < count; i++) {
        product_node = screen_node->product_list.at(i);

        if (product_node->col >= screen_node->cols
            || product_node->row >= screen_node->rows) {
            screen_node->product_list.remove(i);
            count--;
            i--;
        }
    }

    count = screen_node->product_list.count();
    for (i = 0; i < count; i++) {
        product_node = screen_node->product_list.at(i);
        item = new QTableWidgetItem();
        if ((product_node->product.logo).isEmpty()) {
            // set the correct font to this widget
            item->setText(product_node->product.name);
            // @benes           item->setWordWrap(true);
        } else {
            auto iconfile = ICON_PATH + product_node->product.logo;
            item->setIcon(cropedIcon(iconfile, size));
        }
        screen->setItem(product_node->row, product_node->col, item);
    }
}

void ProductScreenDesignWidget::deleteSlot()
{

    ScreenNode* screen_node {};
    if (screen_stack->currentWidget() == select_rows_cols) {
        screen_node = screen_list->find(name_screen->currentText());
        if (screen_node) {
            screen_list->remove(name_screen->currentText());
            initScreens();
        }
    } else if (screen_stack->currentWidget() == screen_design) {
        if (actual_namestring.isEmpty())
            return;

        screen_node = screen_list->find(actual_namestring);
        QString tmp_str = QString::number(screen->currentRow()) + "-" + QString::number(screen->currentColumn());
        ProductNode* product_node = screen_node->product_list.find(tmp_str);
        if (!product_node) {
            //@benes            screen->setText(screen->currentRow(),screen->currentColumn(),"");
            //@benes            screen->setIcon(screen->currentRow(),screen->currentColumn(), QPixmap());
            auto row = screen->currentRow();
            auto col = screen->currentColumn();
            screen->model()->setData(screen->model()->index(row, col), QString {});
            screen->model()->setData(screen->model()->index(row, col), QIcon(), Qt::DecorationRole);
            return;
        }
        screen_node->product_list.remove(tmp_str);
        //@benes        screen->setText(screen->currentRow(), screen->currentColumn(),"");
        //@benes        screen->setPixmap(screen->currentRow(),screen->currentColumn(),QPixmap(""));
        auto row = screen->currentRow();
        auto col = screen->currentColumn();
        screen->model()->setData(screen->model()->index(row, col), QString {});
        screen->model()->setData(screen->model()->index(row, col), QIcon(), Qt::DecorationRole);
    }
}

ProductData* ProductScreenDesignWidget::getProduct(const QString& product_code)
{
    std::unique_ptr<ProductModule> product_mod { new ProductModule() };
    return xmlToProduct(product_mod->getProduct(product_code));
}

ProductData* ProductScreenDesignWidget::getProductWithName(const QString& product_name)
{
    std::unique_ptr<ProductModule> product_mod { new ProductModule() };
    return xmlToProduct(product_mod->getLogo(product_name));
}

void ProductScreenDesignWidget::startShowing()
{
    // disable all buttons
    back_button->hide();
    forward_button->hide();
    forward_button->hide();
    accept_button->hide();
    delete_button->hide();

    clearTable();
    getScreens();
    initScreens();
    initUnitaryProducts();

    accept_button->hide();
    back_button->hide();
    forward_button->show();
    delete_button->show();
    forward_button->setEnabled(false);
    screen_default->setChecked(false);
    delete_button->setEnabled(false);
}

void ProductScreenDesignWidget::showEvent(QShowEvent* e)
{
    QObject* object = this;
    if (!object->inherits("ProductScreenDesignWidget")) {
        cerr << "Other ProductScreenDesignWidget sent the event" << endl;
        QWidget::showEvent(e);
    }

    screen_stack->setCurrentWidget(reading_page);
    QTimer::singleShot(TIME_OUT, this, &ProductScreenDesignWidget::startShowing);
    QWidget::showEvent(e);
}

void ProductScreenDesignWidget::hideEvent(QHideEvent* e)
{
    screen_list->clear();
    QWidget::hideEvent(e);
}

void ProductScreenDesignWidget::upSlot()
{
    products->scroll(0, -100);
}

void ProductScreenDesignWidget::downSlot()
{
    products->scroll(0, 100);
}

ProductData* ProductScreenDesignWidget::xmlToProduct(const QString& xml_str)
{

    XmlConfig xml(SCREENS_XML);
    if (!xml.readXmlFromString(xml_str)) {
        cerr << "cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return nullptr;
    }

    if (!xml.validateXmlWithDTD(SCREEN_PRODUCTS_LIST_DTD)) {
        cerr << "The file does not conform a valid xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return nullptr;
    }

    xml.delDomain();
    xml.setDomain("products.product");

    auto product = new ProductData;
    product->code = xml.readString("code");
    product->name = xml.readString("name");
    product->logo = xml.readString("logo");

    return product;
}
