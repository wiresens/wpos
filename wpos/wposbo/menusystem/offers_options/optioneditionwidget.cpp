#include "optioneditionwidget.h"

#include "productsmodule/optionsmodule/productoptionmodule.h"

#include <libbslxml/xmlconfig.h>
#include <wposgui/common/toolkit.h>
#include <wposgui/keyboard/floatkeyboard.h>

#include <iostream>
using std::cerr;
using std::endl;

#define OPTIONS_LIST_DTD "dtddocs:products_optionslist.dtd"
#define INTERFACE_NAME "addOptionInterface"

OptionEditionWidget::OptionEditionWidget(QWidget* parent, const QString& name)
    : QWidget(parent)
{
    setupUi(this);
    setObjectName(name);

    // idem
    QVBoxLayout* vlayout {};
    float_kb = new FloatKeyboard(box);
    float_kb->setObjectName("float_keyboard_offer");
    if (!(vlayout = (QVBoxLayout*)box->layout()))
        vlayout = new QVBoxLayout(box);
    vlayout->addWidget(float_kb);

    type_combobox->hide();
    option_combobox->hide();
    del_option_button->hide();

    type_listview->setSelectionMode(QAbstractItemView::SingleSelection);
    option_listview->setSelectionMode(QAbstractItemView::SingleSelection);
    type_listview->header()->hide();
    option_listview->header()->hide();
    type_listview->setColumnWidth(0, 300);
    type_listview->setAllColumnsShowFocus(true);
    option_listview->setAllColumnsShowFocus(true);
    option_listview->setColumnWidth(0, 300);

    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    add_type_button->setIcon(QPixmap("controls32:filesave.png"));
    add_option_button->setIcon(QPixmap("controls32:filesave.png"));
    del_type_button->setIcon(QPixmap("controls32:edittrash.png"));
    del_option_button->setIcon(QPixmap("controls32:edittrash.png"));

    up_type_button->setIcon(QPixmap("controls32:up.png"));
    down_type_button->setIcon(QPixmap("controls32:down.png"));
    up_option_button->setIcon(QPixmap("controls32:up.png"));
    down_option_button->setIcon(QPixmap("controls32:down.png"));

    // connections.
    connect(up_type_button, &QPushButton::clicked, this, &OptionEditionWidget::upTypeSlot);
    connect(down_type_button, &QPushButton::clicked, this, &OptionEditionWidget::downTypeSlot);
    connect(up_option_button, &QPushButton::clicked, this, &OptionEditionWidget::upOptionSlot);
    connect(down_option_button, &QPushButton::clicked, this, &OptionEditionWidget::downOptionSlot);

    connect(type_listview, &QTreeWidget::itemSelectionChanged, this, &OptionEditionWidget::typeClickedSlot);
    connect(ok_button, &QPushButton::clicked, this, &OptionEditionWidget::acceptSlot);
}

OptionEditionWidget::~OptionEditionWidget()
{
    delete float_kb;
}

void OptionEditionWidget::clear()
{
    type_lineedit->clear();
    option_lineedit->clear();

    type_listview->clear();
    option_listview->clear();

    group_box->setChecked(false);
    float_kb->clear();
}

void OptionEditionWidget::upTypeSlot()
{
    //    QListViewItem *item = 0;
    //    item = type_listview->selectedItem();
    //    if (!item)
    //        return;
    //    else if (item != type_listview->firstChild())
    //        item = item->itemAbove();
    //    type_listview->setCurrentItem(item);
    //    type_listview->ensureItemVisible(item);
    //    type_listview->setSelected(item,true);
    selectTreeItemUp(type_listview);
}

void OptionEditionWidget::downTypeSlot()
{
    //    QListViewItem *item = 0;
    //    item = type_listview->selectedItem();
    //    if (!item)
    //        item = type_listview->firstChild();
    //    else if (item != type_listview->lastItem())
    //        item = item->itemBelow();
    //    type_listview->setCurrentItem(item);
    //    type_listview->ensureItemVisible(item);
    //    type_listview->setSelected(item,true);
    selectTreeItemDown(type_listview);
}

void OptionEditionWidget::upOptionSlot()
{
    //    QListViewItem *item = 0;
    //    item = option_listview->selectedItem();
    //    if (!item)
    //        return;
    //    else if (item != option_listview->firstChild())
    //        item = item->itemAbove();
    //    option_listview->setCurrentItem(item);
    //    option_listview->ensureItemVisible(item);
    //    option_listview->setSelected(item,true);
    selectTreeItemUp(option_listview);
}

void OptionEditionWidget::downOptionSlot()
{
    //    QListViewItem *item = 0;
    //    item = option_listview->selectedItem();
    //    if (!item)
    //        item = option_listview->firstChild();
    //    else if (item != type_listview->lastItem())
    //        item = item->itemBelow();
    //    option_listview->setCurrentItem(item);
    //    option_listview->ensureItemVisible(item);
    //    option_listview->setSelected(item,true);
    selectTreeItemDown(option_listview);
}

void OptionEditionWidget::typeClickedSlot()
{
    auto source = qobject_cast<QTreeWidget*>(sender());
    if (!source || source->selectedItems().isEmpty())
        return;

    getOptionTypes(source->selectedItems().first()->text(0));
}

void OptionEditionWidget::getTypes()
{

    type_listview->clear();
    XmlConfig xml;
    if (!xml.readXmlFromString(mod.getOptionTypes())) {
        cerr << "can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)) {
        cerr << "th file does not conform xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.setDomain("options");
    for (auto i = 0; i < xml.howManyTags("option"); i++) {
        auto type = xml.readString("option[" + QString::number(i) + "].option_type");
        auto item = new QTreeWidgetItem(type_listview);
        item->setText(0, type);
    }
}

void OptionEditionWidget::getOptionTypes(const QString& type)
{

    option_listview->clear();
    XmlConfig xml;
    if (!xml.readXmlFromString(mod.getOptions(type))) {
        cerr << "can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)) {
        cerr << "the file does not conform xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
    xml.setDomain("options");
    for (auto i = 0; i < xml.howManyTags("option"); i++) {
        auto name = xml.readString("option[" + QString::number(i) + "].option_name");
        auto item = new QTreeWidgetItem(option_listview);
        item->setText(0, name);
    }
}

void OptionEditionWidget::showEvent(QShowEvent* e)
{
    clear();
    getTypes();
    QWidget::showEvent(e);
}

void OptionEditionWidget::acceptSlot()
{
    clear();
    getTypes();
}

bool OptionEditionWidget::checkAllValues()
{
    return !type_listview->selectedItems().isEmpty() && !option_listview->selectedItems().isEmpty();
}

void OptionEditionWidget::fillOptions(
    const QString& domain, const QString& sub_domain,
    const QString& tag, QComboBox* cbx, const QString& data)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(data)) {
        cerr << "Can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)) {
        cerr << "Xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.setDomain(domain);
    QStringList option_types;
    auto left = tag + "[";
    auto right = "]." + sub_domain;
    for (auto i = 0; i < xml.howManyTags(tag); i++)
        option_types << xml.readString(left + QString::number(i) + right);

    cbx->clear();
    cbx->addItems(option_types);
}
