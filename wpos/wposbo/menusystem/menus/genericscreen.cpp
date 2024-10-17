/***************************************************************************
                          genericscreen.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

 ***************************************************************************/

#include "genericscreen.h"
#include "mainscreenmenu.h"
#include "instantiator.h"
#include "menusystem/design/submenuinfoframe.h"

#include <wposgui/common/global.h>
#include <wposgui/windows/menustack.h>
#include <wposgui/windows/menupage.h>
#include <wposgui/common/treeview.h>
#include <libbslxml/xmlconfig.h>

#include <QHeaderView>
#include <QSplitter>
#include <QLayout>
#include <QPixmap>
#include <QEvent>
#include <QColor>

#include <iostream>
using namespace std;
extern Instantiator *menuInstantiator;

GenericScreen::GenericScreen(
    XmlConfig &xml,
    const QString& rootDomain,
    QWidget* parent,
    const QString& name):
    QWidget(parent)
{
    setObjectName(name);

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);

    auto splitter = new QSplitter(this);
    mainLayout->addWidget(splitter);
    splitter->setChildrenCollapsible(false);

    subMenus = new TreeView(this);
    subMenus->setFrameShape(QFrame::StyledPanel);
    subMenus->setObjectName(Menus::SUB_MENU_TREE);
    subMenus->setFocusPolicy(Qt::NoFocus);
    subMenus->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    subMenus->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    subMenus->hideColumn(Name);
    splitter->addWidget(subMenus);

    // Create the stackedwidget which will hold the menu
    menuStack = new MenuStack(this, "GENERIC_SCREEN_STACK_WIDGET");
    splitter->addWidget(menuStack);
    splitter->setStretchFactor(splitter->indexOf(subMenus), 0);
    splitter->setStretchFactor(splitter->indexOf(menuStack), 4);

    // Default profile for order is obtained from the description XML.
    // <PART 2> extracting values from XML
    parseXmlDescription(xml, rootDomain);
    connect(subMenus, &TreeView::itemClicked, this, &GenericScreen::setCurrentMenu);
}

void GenericScreen::parseXmlDescription(
    XmlConfig &xml,
    const QString &rootDomain)
{
    xml.pushDomain();
    xml.delDomain();
    if (!xml.setDomain(rootDomain)){
        xml.popDomain();
        return;
    }

    // First entry in menu
    auto menuName   = xml.readString("name");
    auto menuText   = xml.readString("text");
    auto menuPixmap = xml.readString("pixmap");
    auto menuUrl    = xml.readString("url");

    rootItem = new QTreeWidgetItem(subMenus);
    rootItem->setText(Icon, menuText);
    rootItem->setIcon(Icon, QPixmap(menuPixmap));
    rootItem->setExpanded(true);

    // Add a info menu page
    auto menuPage = new MenuPage(menuStack, menuName);
    menuPage->setLayoutType(MenuPage::LayoutType::VBOX);
    auto menu = menuInstantiator->getMenu(menuPage, menuName);
    if( !menu ){
        menuPage->deleteLater();
        return;
    }

    menuPage->addWidget(menu, "INFO_MENU_WIDGET");
    menuStack->addPage(menuPage, menuName);
    rootItem->setText(Name, menuName);
    rootItem->setSelected(true);

    xml.setDomain("items");
    QTreeWidgetItem *item {};
    for(auto i=0; i < xml.howManyTags("item"); i++){
        menuName = xml.readString("item["+QString::number(i)+"].name");
        menuText = xml.readString("item["+QString::number(i)+"].text");
        menuPixmap = xml.readString("item["+QString::number(i)+"].pixmap");

        if (  !menuInstantiator->hasMenu(menuName) && !menuName.contains("BACK"))
            continue;

        if(i != 0) item = new QTreeWidgetItem(rootItem, item);
        else item = new QTreeWidgetItem(rootItem);

        item->setText(Icon, menuText);
        item->setIcon(Icon, QPixmap(menuPixmap));
        if( !menuName.contains("BACK")){
            menuPage = new MenuPage(menuStack, menuName);
            menuPage->setLayoutType(MenuPage::LayoutType::VBOX);
            menuPage->setBackgroundRole(QPalette::Window);
            menuPage->setPalette(QPalette(QColor(255,221,118)));
            menu = menuInstantiator->getMenu(menuPage, menuName);
            menuPage->setSizePolicy(menuStack->sizePolicy());
            menu->setSizePolicy(menuPage->sizePolicy());

            menuPage->addWidget(menu, menuName + QString::number(i));
            menuStack->addPage(menuPage, menuName);
        }
        item->setText(Name, menuName);
    }
    xml.popDomain();
}

void GenericScreen::setCurrentMenu(QTreeWidgetItem *item){
    if (!item) return;
    auto direction = item->text(Name);
    if (!direction.isEmpty()){
        if( !direction.contains("BACK") ){
            menuStack->setCurrentPage(direction);
            emit menuChanged(direction);
        }
        else{
            emit footerRequested(true);
            emit menuRequested(Menus::MAIN_MENU);
        }
    }
}

void GenericScreen::showEvent(QShowEvent *event){
    rootItem->setSelected(true);
    auto direction = rootItem->text(Name);
    if ( !direction.isEmpty() ) menuStack->setCurrentPage(direction);
    menuStack->resize(menuStack->sizeHint());
    QWidget::showEvent(event);
}
