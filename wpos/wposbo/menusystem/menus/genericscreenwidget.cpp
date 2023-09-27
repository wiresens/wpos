/***************************************************************************
                          bslgenericscreenwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "genericscreenwidget.h"

#include "mainscreenwidget.h"
#include "instantiator.h"



#include "menusystem/design/submenuinfoframe.h"

#include <wposwidget/global.h>
#include <wposwidget/menustack.h>
#include <wposwidget/menupage.h>
#include <wposwidget/treeview.h>

#include <QHeaderView>
#include <QSplitter>
#include <QLayout>
#include <QPixmap>
#include <QEvent>
#include <QColor>

#include <libbslxml/xmlconfig.h>

#include <iostream>

using namespace std;

//extern GenericSignalManager *gsignal_manager;
extern Instantiator *menus_instantiator;

GenericScreenWidget::GenericScreenWidget(QWidget* parent, const QString& name) :
    QWidget(parent){
    setObjectName(name);
    initThis();
    connect(actions_listview, &TreeView::itemClicked, this, &GenericScreenWidget::raiseMenuSlot);
}

GenericScreenWidget::GenericScreenWidget(
        XmlConfig *xml, const QString& root_node,
        QWidget* parent, const QString& name) :
    QWidget(parent), xml_description {xml}, root_domain  {root_node}
{
    setObjectName(name);
    initThis();
    connect(actions_listview, &TreeView::itemClicked, this, &GenericScreenWidget::raiseMenuSlot);
}

void GenericScreenWidget::initThis(){
    main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0,0,0,0);
    auto splitter = new QSplitter(this);
    main_layout->addWidget(splitter);
    splitter->setChildrenCollapsible(false);

    actions_listview = new TreeView(this);
    actions_listview->setFrameShape(QFrame::StyledPanel);
    actions_listview->setObjectName("MENU_WIDGET");
    actions_listview->setFocusPolicy(Qt::NoFocus);
    actions_listview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    actions_listview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//@benes     actions_listview->addColumn("");
    actions_listview->hideColumn(1);
    splitter->addWidget(actions_listview);

    // Create the widget which will held the menus (it is a stack widget)
    menus_stack = new MenuStack(this,"STACK_WIDGET");
//    menus_stack->setFrameShape(QFrame::StyledPanel);
    splitter->addWidget(menus_stack);
    splitter->setStretchFactor(splitter->indexOf(actions_listview), 0);
    splitter->setStretchFactor(splitter->indexOf(menus_stack), 4);

    // Default profile for order is obtained from the description XML. <PART 2> extracting values from XML
    parseXmlDescription();
}

void GenericScreenWidget::parseXmlDescription(){

    xml_description->pushDomain();
    xml_description->delDomain();
    if (!xml_description->setDomain(root_domain+".submenu")){
        xml_description->popDomain();
        return;
    }

    // First entry in menu
    auto aux_name = xml_description->readString("name");
    auto aux_text = xml_description->readString("text");
    auto aux_pixmap = xml_description->readString("pixmap");
    auto aux_url = xml_description->readString("url");
    root = new QTreeWidgetItem(actions_listview);
    root->setText(Icon, aux_text);
    root->setIcon(Icon, QPixmap(aux_pixmap));
    root->setExpanded(true);

    // Add a info menu page
    auto page = new MenuPage(menus_stack, aux_name);
    page->setLayoutType(MenuPage::VBOX);
    auto menu = menus_instantiator->getMenu(page, aux_name);
    if( !menu ){
//@benes        delete page;
        page->deleteLater();
        return;
    }

    auto menu_name = menu->objectName();
    page->addWidget(menu,"INFO_MENU_WIDGET");
    menus_stack->addPage(page, aux_name);

    root->setText(Name, aux_name);
    root->setSelected(true);

    xml_description->setDomain("items");
    QTreeWidgetItem *item {};
    for(auto i=0; i < xml_description->howManyTags("item"); i++){
        aux_name = xml_description->readString("item["+QString::number(i)+"].name");
        aux_text = xml_description->readString("item["+QString::number(i)+"].text");
        aux_pixmap = xml_description->readString("item["+QString::number(i)+"].pixmap");

        if (  !menus_instantiator->hasMenu(aux_name) && !aux_name.contains("BACK"))
            continue;

        if(i != 0) item = new QTreeWidgetItem(root,item);
        else item = new QTreeWidgetItem(root);

        item->setText(Icon, aux_text);
        item->setIcon(Icon, QPixmap(aux_pixmap));
        if( !aux_name.contains("BACK") ){
            page = new MenuPage(menus_stack, aux_name);
            page->setLayoutType(MenuPage::VBOX);
            setBackgroundRole(QPalette::Window);
            page->setPalette(QPalette(QColor(255,221,118)));
            menu = menus_instantiator->getMenu(page, aux_name);
            menu->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
            page->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
            // connect(this,SIGNAL(menuIDSignal(const QString &)),menu,SLOT(menuIDSlot(const QString &)));
            page->addWidget(menu, aux_name+QString::number(i));
            menus_stack->addPage(page, aux_name);
        }
        item->setText(Name, aux_name);
    }
    xml_description->popDomain();
}

void GenericScreenWidget::raiseMenuSlot(QTreeWidgetItem *item){

    if (!item) return;
    auto direction = item->text(Name);
    if (!direction.isEmpty())
    {
        if( !direction.contains("BACK") ){
            menus_stack->setCurrentPage(direction);
            emit menuIDSignal(direction);
        }
        else
        {
            showDownTitleSignal(true);
            emit raiseMenuSignal(Menus::MAIN_MENU);
        }
    }
}

void GenericScreenWidget::showEvent(QShowEvent *e){
    root->setSelected(true);
    auto direction = root->text(Name);
    if ( !direction.isEmpty() ) menus_stack->setCurrentPage(direction);
    menus_stack->resize(menus_stack->sizeHint());
    QWidget::showEvent(e);
}
