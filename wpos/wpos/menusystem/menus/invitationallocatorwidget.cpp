/***************************************************************************
                          bslinvitationallocatorwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include "salesscreen.h"

#include "invitationallocatorwidget.h"

#include <QLayout>
#include <QFrame>
#include <QPushButton>
#include <QStackedWidget>
#include <QFont>
#include <QPixmap>

#include <libbslxml/xmlconfig.h>

#include <iostream>
using namespace std;

InvitationAllocatorWidget::InvitationAllocatorWidget(QWidget *parent, const QString& name) :
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);

    // Initialize all images
    cancel_button->setIcon(QPixmap("controls:button_cancel.png"));
    back_button->setIcon(QPixmap("controls:back_48.png"));
    back_button->hide();
    forward_button->setIcon(QPixmap("controls:forward_48.png"));
    forward_button->hide();

    // Default profile for order is obtained from the description XML. <PART 2> extracting values from XML
    parseXmlDescription();

    // Register generic signal
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::XCORE_SET_PRODUCT_OFFER,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::PRODSELECT_COLOR_MODE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED,this);
    gsm->subscribeToGenericSignal(GSIGNAL::LOAD_OFFERS,this);

    connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancelSlot()));
}

InvitationAllocatorWidget::~InvitationAllocatorWidget(){}

void InvitationAllocatorWidget::parseXmlDescription(){
    int num_items = 0;
    int num_menus = 0;
    QString aux_name,aux_text, aux_pixmap, aux_item ;
    QFrame *frame {};
    QVBoxLayout *frame_layout{};
    QListWidget *iconview = 0;
    QListWidgetItem *item = 0;

    iconview_menu->clear();
    iconviews.clear();
    menus.clear();
    data_items.clear();
    menu_items.clear();

    //borrar todos los frames de el widgetstack
    while (pages.count()){
        menus_widgetstack->removeWidget(pages.first());
        pages.removeFirst();
    }

    auto configFile = Files::configFilePath("invitations");
    if ( !QFile::exists(configFile) ){
        cerr << "Can't find " << configFile.toStdString() << " file. [CRITICAL ERROR] Can't initialize order in proper way.";
        return;
    }

    XmlConfig xml(configFile);
    if (!xml.isValid()){
        cerr << "Can't parse " << configFile.toStdString() << "file. [CRITICAL ERROR] Can't initialize order in proper way.";
        return;
    }

    xml.setDomain("menuentries");
    num_menus = xml.howManyTags("menuentry");

    for(int i=0; i< num_menus; i++){
        aux_name = xml.readString("menuentry["+QString::number(i)+"].entry.name");
        aux_text = xml.readString("menuentry["+QString::number(i)+"].entry.text");
        aux_pixmap = xml.readString("menuentry["+QString::number(i)+"].entry.pixmap");
        item = new QListWidgetItem(iconview_menu);
        menu_items.append(item,aux_name);
        item->setText(aux_text);
        item->setIcon(QPixmap(aux_pixmap));
        frame = new QFrame(menus_widgetstack);
        frame->setObjectName("FRAME_"+QString::number(i));
        menus.insert(item->text(), frame);
        frame->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
        frame->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
        frame_layout = new QVBoxLayout(frame);
        frame_layout->setSpacing(0);
        frame_layout->setMargin(0);
        iconview = new QListWidget(frame);
        iconview->setObjectName("ICONVIEW_"+QString::number(i));
        iconviews.append(iconview);
        iconview->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
        iconview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        iconview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        xml.pushDomain();
        xml.setDomain("menuentry["+QString::number(i)+"].menu.items");
        num_items = xml.howManyTags("item");
        for(int j=0; j<num_items; j++){
            ItemOfferData *data = new ItemOfferData();
            aux_text = xml.readString("item["+QString::number(j)+"].text");
            aux_pixmap = xml.readString("item["+QString::number(j)+"].pixmap");
            data->type = xml.readString("item["+QString::number(j)+"].offer.type");
            data->name = xml.readString("item["+QString::number(j)+"].offer.name");
            QListWidgetItem *iconitem = new QListWidgetItem(iconview);
            iconitem->setIcon(QPixmap(aux_pixmap));
            iconitem->setText(aux_text);
            data_items.insert(iconitem->text(), data);
        }
        xml.popDomain();
        frame_layout->addWidget(iconview);
        menus_widgetstack->addWidget(frame);
        pages.append(frame);
        connect(iconview_menu, &QListWidget::itemPressed, this, &InvitationAllocatorWidget::raiseMenu);
        connect(iconview, &QAbstractItemView::pressed, this, &InvitationAllocatorWidget::invitationSelected);
    }

    // Select the default entry and menu
    xml.delDomain();
    aux_item = xml.readString("defaultentry");
    default_item = menu_items.find(aux_item);
    if(default_item != 0){
        iconview_menu->setCurrentItem(default_item);
        raiseMenu(default_item);
    }
}

void InvitationAllocatorWidget::raiseMenu(QListWidgetItem *item){
    QWidget *menu = *(menus.find(item->text()));
    if(!menu)  return;
    menus_widgetstack->setCurrentWidget(menu);
}

void InvitationAllocatorWidget::showEvent(QShowEvent *e){
    if (default_item){
        iconview_menu->setCurrentItem(default_item);
        raiseMenu(default_item);
        for( auto view : iconviews){
            view->clearSelection();
        }
    }
    QWidget::showEvent(e);
}

void InvitationAllocatorWidget::invitationSelected(const QModelIndex &index){

    ItemOfferData *data = *(data_items.find(index.data().toString()));
    if(!data) return;

    {
        XmlConfig xml ;
        xml.createElement("type", data->type);
        xml.createElement("name", data->name);
        emit genericDataSignal(GDATASIGNAL::XCORE_SET_PRODUCT_OFFER, &xml);
    }

    {
        // Then emmit the signal to back to main products stack
        XmlConfig xml ;
        xml.createElement("name", SalesScreen::PRODUCT_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
    }


    {
        //emit the signal to put the prodselect color to the invitation color.
        XmlConfig xml ;
        xml.createElement("mode","special");
        emit genericDataSignal(GDATASIGNAL::PRODSELECT_COLOR_MODE, &xml);
    }

    XmlConfig xml ;
    xml.createElement("enabled","true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
}

void InvitationAllocatorWidget::cancelSlot(){

    XmlConfig xml;
    xml.createElement("name", SalesScreen::PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
    xml.deleteElement("name");

    xml.createElement("enabled","true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
}

void InvitationAllocatorWidget::genericSignalSlot(const QString& signal_name){
    if (signal_name == GSIGNAL::LOAD_OFFERS )
        parseXmlDescription();
}
