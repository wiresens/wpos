/***************************************************************************
                          ndelofferwidget.cpp  -  description
                             -------------------
    begin                : vie abr 23 2004
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "offerdeletionwidget.h"
#include "productsmodule/offersmodule/productoffermodule.h"
#include <libbslxml/xmlconfig.h>

#include <QMessageBox>

#include <iostream>
using std::cerr;
using std::endl;

static const QString& OFFERS_LIST_DTD  {"/etc/ntpv_backoffice/dtds/products_offerslist.dtd"};
static const QString& ICONS_PATH {"controls:offers/"};

const QString& FIXED = QObject::tr("Offer Fixed Price");
const QString& PERCENT = QObject::tr("Offer Percentage Rate");

OfferDeletionWidget::OfferDeletionWidget(
        QWidget *parent, const QString& name ) :
    QWidget(parent){
    setupUi(this);
    setObjectName(name);

    clear();
    title->setText(tr("Delete Offer"));
    offer_listview->header()->hide();
    offer_listview->setSelectionMode(QAbstractItemView::SingleSelection);
    offer_listbox->setSelectionMode(QAbstractItemView::SingleSelection);

    offer_listview->setColumnWidth( Icon, 80);
    offer_listview->setColumnWidth( Name, 300);
    offer_listview->setAllColumnsShowFocus(true);
    offer_listview->sortByColumn(Name);

    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    pix_label1->setPixmap(QPixmap("controls48:galeon.png"));

    connect(offer_listbox, &QListWidget::itemSelectionChanged, this, &OfferDeletionWidget::handleOfferListSelected);
    connect(offer_listview,  &QTreeWidget::itemSelectionChanged, this, &OfferDeletionWidget::handleOfferTreeSelected);
    connect(up_button, &QPushButton::clicked, this, &OfferDeletionWidget::previousOffer);
    connect(down_button,  &QPushButton::clicked, this, &OfferDeletionWidget::nextOffer);
    connect(up_type_button,  &QPushButton::clicked, this, &OfferDeletionWidget::previousType);
    connect(down_type_button,  &QPushButton::clicked, this, &OfferDeletionWidget::nextType);
    connect(del_type_button,  &QPushButton::released, this, &OfferDeletionWidget::deleteOfferType);
    connect(ok_button,  &QPushButton::released, this, &OfferDeletionWidget::acceptSlot);
}

void OfferDeletionWidget::clear(){
    offer_listbox->clear();
    offer_listview->clear();
    operator_label->clear();
    ok_button->setEnabled(hasAllValues());
}

void OfferDeletionWidget::previousOffer(){
//    QListWidgetItem *item = 0;
//    item = offer_listview->selectedItem();
//    if ((!item)||item == (offer_listview->firstChild()))
//        return;
//    else
//        item = item->itemAbove();
//    offer_listview->setCurrentItem(item);
//    offer_listview->ensureItemVisible(item);
//    offer_listview->setSelected(item,true);
}

void OfferDeletionWidget::nextOffer(){
//    QListWidgetItem *item = 0;
//    item = offer_listview->selectedItem();
//    if (!item)
//        item = offer_listview->firstChild();
//    else if (item == offer_listview->lastItem())
//        return;
//    else
//        item = item->itemBelow();
//    offer_listview->setCurrentItem(item);
//    offer_listview->ensureItemVisible(item);
//    offer_listview->setSelected(item,true);
}

void OfferDeletionWidget::previousType(){
//    int pos = 0;
//    pos = offer_listbox->currentItem();
//    if (pos == -1)
//        return;
//    else if (pos)
//        pos--;
//    offer_listbox->setCurrentItem(pos);
//    offer_listbox->setSelected(pos,true);
//    offer_listbox->ensureCurrentVisible();
}

void OfferDeletionWidget::nextType(){
//    int pos = offer_listbox->currentItem();
//    if (pos == -1)  pos = 0;
//    else if (pos != (int)offer_listbox->count()-1) pos++;

//    offer_listbox->setCurrentItem(pos);
//    offer_listbox->setSelected(pos,true);
//    offer_listbox->ensureCurrentVisible();
}

void OfferDeletionWidget::showEvent(QShowEvent *e){
    clear();
    getOfferTypes();
    QWidget::showEvent(e);
}

void OfferDeletionWidget::getOfferTypes(){

    XmlConfig xml ;
    if( !xml.readXmlFromString(mod.getOfferTypes()) ){
        cerr << "can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " <<__LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
        cerr << "file does not conform the xml " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.setDomain("offers");
    QStringList types;
    for(auto i = 0; i < xml.howManyTags("offer"); i++)
        types << xml.readString("offer[" + QString::number(i) + "].offer_type");

    offer_listbox->clear();
    offer_listbox->addItems(types);
}

void OfferDeletionWidget::handleOfferListSelected(){
    auto source = qobject_cast<QListWidget*>( sender());
    if ( !source || source->selectedItems().isEmpty()) return;

    auto item = source->selectedItems().first();

    showOffers(item->text());
    auto cpp = mod.getCppOperator(source->currentItem()->text());

    if (cpp=="=") operator_label->setText(FIXED);
    else if (cpp=="x")  operator_label->setText(PERCENT);
    else  operator_label->clear();

    ok_button->setEnabled(hasAllValues());
}

void OfferDeletionWidget::handleOfferTreeSelected(){
    auto source = qobject_cast<QTreeWidget*>( sender());
    if ( !source || source->selectedItems().isEmpty()) return;

    ok_button->setEnabled(hasAllValues());
}

void OfferDeletionWidget::showOffers(const QString& offer_type){

    offer_listview->clear();
    QString xml_string = mod.getOffers(offer_type);

    if ( xml_string.isEmpty() ) return;

    XmlConfig xml;
    if( !xml.readXmlFromString(xml_string) ){
        cerr << "can not convert the string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if( !xml.validateXmlWithDTD(OFFERS_LIST_DTD, true) ){
        cerr << "the file does not conform the xml" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
    xml.setDomain("offers");
    for(auto i = 0; i < xml.howManyTags("offer"); i++){
        auto item = new QTreeWidgetItem(offer_listview);
        auto logo_file = ICONS_PATH + xml.readString("offer["+QString::number(i)+"].logo");
        item->setIcon(Icon, QPixmap(logo_file) );
        item->setText(Name, xml.readString("offer["+QString::number(i)+"].offer_name"));
    }
}

bool OfferDeletionWidget::hasAllValues(){
    return !offer_listbox->selectedItems().isEmpty() && !offer_listview->selectedItems().isEmpty() ;
}

void OfferDeletionWidget::deleteOfferType(){
    auto items  = offer_listbox->selectedItems();
    if (items.isEmpty()) return;
    auto item = items.first();
    auto type = item->text();

    auto msg = tr("If you delete this offer type, all offers associated \n with this type of all products will also be deleted. Do you want to delete the offer type  : %1 ?").arg(type);
    if( QMessageBox::question( this, tr("Delete Offer"), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No ) return;

    if ( !mod.deleteOfferType(type) ){
        msg = tr("Could not delete the selected offer type\n and all offers of this type");
        QMessageBox::information(this, tr("Failed to delete offer"), msg, QMessageBox::Ok);
    }
    clear();
    getOfferTypes();
}

void OfferDeletionWidget::acceptSlot(){
    if (!hasAllValues()) return;
    auto *item = offer_listview->selectedItems().first();

    XmlConfig xml;
    xml.createElementSetDomain("offers.offer");
    xml.createElement("offer_type", offer_listbox->currentItem()->text());
    xml.createElement("offer_name", item->text(Name));
    xml.delDomain();

    if ( !mod.deleteOffer(xml.toString()) ){
        QString text = tr("The selected offer could not be deleted\n");
        QMessageBox::information(this, tr("Failed to delete offer"), text, QMessageBox::Ok);
    }

    clear();
    getOfferTypes();
}
