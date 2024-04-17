/***************************************************************************
                          naddofferwidget.cpp  -  description
                             -------------------
    begin                : lun abr 19 2004
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "offereditionwidget.h"
#include "menusystem/utils.h"

#include <wposgui/keyboard/numkeyboard.h>
#include <wposgui/keyboard/floatkeyboard.h>
#include <libbslxml/xmlconfig.h>

#include <QDir>

#include <iostream>
using std::cerr;
using std::endl;

static const int  ICON_SIZE = 32;
static const QString& OFFERS_LIST_DTD  {"/etc/ntpv_backoffice/dtds/products_offerslist.dtd"};

static const QString& ICON_PATH = "controls:offers/";
static const QString& ADVERTISE_FIXED = QObject::tr("Enter the fixed price of this offer");
static const QString& ADVERTISE_PERCENT = QObject::tr("Enter a percentage (between 0 and 100)");

OfferEditionWidget::OfferEditionWidget(QWidget *parent, const QString& name):
   QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    QVBoxLayout *vlayout;

    float_kb = new FloatKeyboard(box);
    float_kb->setObjectName("float_keyboard_offer");
    if( !( vlayout = qobject_cast<QVBoxLayout *> (box->layout())))
        vlayout = new QVBoxLayout(box);
    vlayout->addWidget(float_kb);

    num_kb = new NumKeyboard(box);
    num_kb->setObjectName("float_keyboard_offer");
    if( ! (vlayout = qobject_cast<QVBoxLayout *> (box->layout())))
        vlayout = new QVBoxLayout(box);
    vlayout->addWidget(num_kb);

    clear();

    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    right_button->setIcon(QPixmap("controls48:forward.png"));
    left_button->setIcon(QPixmap("controls48:back.png"));

    connect(fixed_radiobutton, &QRadioButton::clicked, this, &OfferEditionWidget::radioButtonSlot);
    connect(percent_radiobutton, &QRadioButton::clicked, this, &OfferEditionWidget::radioButtonSlot);
    connect(logo_view, &QListWidget::itemClicked, this, &OfferEditionWidget::logoItemClickedSlot);

    connect(right_button, &QPushButton::clicked, this, &OfferEditionWidget::rightButtonSlot);
    connect(left_button, &QPushButton::clicked, this, &OfferEditionWidget::leftButtonSlot);
    connect(ok_button, &QPushButton::released, this, &OfferEditionWidget::acceptSlot);
}

OfferEditionWidget::~OfferEditionWidget(){
    deleteLater();
}

void OfferEditionWidget::clear(){
    logo_view->clear();
    offer_combobox->clear();
    fixed_radiobutton->setChecked(true);
    group_box->setChecked(false);
    float_kb->show();
    float_kb->clear();
    num_kb->clear();
    num_kb->hide();
    advertise_label->setText(ADVERTISE_FIXED);
    button_group->setEnabled(true);
    ok_button->setEnabled(false);
}

void OfferEditionWidget::radioButtonSlot(){
    if (fixed_radiobutton->isChecked()){
        float_kb->clear();
        float_kb->show();
        num_kb->hide();
        advertise_label->setText(ADVERTISE_FIXED);
    }
    else if (percent_radiobutton->isChecked()){
        num_kb->clear();
        float_kb->hide();
        num_kb->show();
        advertise_label->setText(ADVERTISE_PERCENT);
    }
}

void OfferEditionWidget::logoItemClickedSlot(QListWidgetItem *icon){
    ok_button->setEnabled(hasAllValues());
}

void OfferEditionWidget::rightButtonSlot(){
    auto item = logo_view->currentItem();
    auto previous = item;
    if( !item ) return;

    item = logo_view->itemAt( (logo_view->row(item) + 1) % logo_view->count(), 0 );
    logo_view->setCurrentItem(item);
    logo_view->scrollToItem(item);
    previous->setSelected(false);
    item->setSelected(true);
}

void OfferEditionWidget::leftButtonSlot(){
    auto item = logo_view->currentItem();
    auto previous = item;
    if( !item ) return;

    item = logo_view->itemAt( (logo_view->count() + logo_view->row(item) - 1) % logo_view->count(), 0 );
    logo_view->setCurrentItem(item);
    logo_view->scrollToItem(item);
    previous->setSelected(false);
    item->setSelected(true);
}

void OfferEditionWidget::loadIcons(){
//@benes    logo_view->setMaxItemTextLength(80);
    logo_view->clear();
    QStringList files = QDir(ICON_PATH).entryList(QStringList("*.png"), QDir::Readable | QDir::Files, QDir::Name);
    logo_view->setGridSize(QSize(80,80));

    for(const auto& filename : files){
        auto abs_file_name = ICON_PATH + filename;
        auto item = new QListWidgetItem(cropedIcon(abs_file_name, ICON_SIZE), filename, logo_view);
//@benes        item->setDragEnabled(false);
    }
//@benes    logo_view->arrangeItemsInGrid(); replaced by the following
    logo_view->setViewMode(QListView::IconMode);
    logo_view->setWrapping(true);
}

void OfferEditionWidget::getOfferTypes(){
    XmlConfig xml;
    if(!xml.readXmlFromString(mod.getOfferTypes())){
        cerr << "can not conver string into xml " << __PRETTY_FUNCTION__ << ": " <<__LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
        cerr << "file does not confirm xml " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    QStringList types;
    xml.setDomain("offers");
    for(auto i = 0; i < xml.howManyTags("offer"); i++)
        types << xml.readString("offer[" + QString::number(i) + "].offer_type");

    offer_combobox->clear();
    offer_combobox->addItems(types);
    offer_combobox->setCurrentText("");
}

void OfferEditionWidget::showEvent(QShowEvent *e){
    clear();
    getOfferTypes();
    loadIcons();
    QWidget::showEvent(e);
}
