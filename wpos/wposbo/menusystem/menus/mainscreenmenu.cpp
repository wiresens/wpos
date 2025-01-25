/***************************************************************************
                          mainscreenwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

 ***************************************************************************/

#include "mainscreenmenu.h"
#include "genericscreen.h"
#include "instantiator.h"

#include <libbslxml/xmlconfig.h>
#include <wposgui/common/global.h>
#include <wposgui/windows/menupage.h>
#include <wposgui/windows/menustack.h>

#include <QLabel>
#include <QListView>
#include <QListWidget>

#include <iostream>
using std::cerr;

//@benes extern GenericSignalManager *gsignal_manager; it does not use, we need to fugure why
Instantiator* menuInstantiator;

MainScreenMenu::MainScreenMenu(
    MenuPage* parent,
    const QString& name)
    : QWidget(parent)
{
    XmlConfig xml;
    if (!loadConfig(xml))
        throw xml;

    setObjectName(name);
    parent->addWidget(this, name);
    setSizePolicy(parentWidget()->sizePolicy());

    menuInstantiator = new Instantiator(this, "MENUS_INSTANTIATOR");
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    header = new QLabel(this);
    mainLayout->addWidget(header);
    auto pixmapStr = xml.readString("toptitlepixmap");
    if (!pixmapStr.isEmpty())
        header->setPixmap(pixmapStr);

    menuStack = new MenuStack(this);
    mainLayout->addWidget(menuStack);
    menuStack->setSizePolicy(sizePolicy());

    footer = new QLabel(this);
    mainLayout->addWidget(footer);
    pixmapStr = xml.readString("downtitlepixmap");
    if (!pixmapStr.isEmpty())
        footer->setPixmap(pixmapStr);

    menuItems = buildMenus(menuStack, xml);
    connect(menuItems, &QListWidget::itemClicked,
        this, QOverload<QListWidgetItem*>::of(&MainScreenMenu::setCurrentMenu));
}

bool MainScreenMenu::loadConfig(XmlConfig& xml) const
{
    // Default profile for order is obtained from the description XML
    const QString& fileName { "xmldocs:mainscreen_description.xml" };

    if (!QFile::exists(fileName)) {
        cerr << "Confing file not found  : " << fileName.toStdString() << '\n';
        return false;
    }

    xml.readXmlFromString(XmlConfig(fileName).toString());
    if (!xml.wellFormed()) {
        cerr << "Invalid XML file " << fileName.toStdString() << '\n';
        return false;
    }

    // Delete any domain and go to root node
    xml.delDomain();
    return true;
}

QListWidget* MainScreenMenu::buildMenus(
    MenuStack* menuStack,
    XmlConfig& xml)
{
    auto menuPage = new MenuPage(menuStack, Menus::MAIN_MENU);
    menuPage->setSizePolicy(menuStack->sizePolicy());
    menuPage->setLayoutType(MenuPage::LayoutType::VBOX);
    menuStack->addPage(menuPage, menuPage->objectName());

    auto menuItems = new QListWidget(menuPage);
    menuItems->setWordWrap(true);
    menuItems->setViewMode(QListView::IconMode);
    menuItems->setMovement(QListView::Static);
    menuItems->setFlow(QListView::LeftToRight);
    menuItems->setLayoutMode(QListView::Batched);
    menuItems->setSizePolicy(menuPage->sizePolicy());
    menuItems->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    menuItems->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    xml.setDomain("menus");
    setStyles(*menuItems, xml);
    menuPage->addWidget(menuItems, Menus::MAIN_MENU_ITEMS);

    // watch out for that list of char pointers...
    for (auto i = 0; i < xml.howManyTags("menu"); i++) {
        // Read name, text and pixmap to create a menu entries
        auto menuName = xml.readString("menu[" + QString::number(i) + "].name");
        auto menuText = xml.readString("menu[" + QString::number(i) + "].text");
        auto menuPixmap = xml.readString("menu[" + QString::number(i) + "].pixmap");

        // Create a page menu and add the generic screen widget
        menuPage = new MenuPage(menuStack, menuName);
        menuPage->setLayoutType(MenuPage::LayoutType::VBOX);
        auto screen = new GenericScreen(
            xml,
            "menus.menu[" + QString::number(i) + "].submenu",
            menuPage,
            menuName);

        connect(screen, &GenericScreen::footerRequested,
            this, &MainScreenMenu::setFooterVisible);
        connect(screen, &GenericScreen::menuRequested,
            this, QOverload<const QString&>::of(&MainScreenMenu::setCurrentMenu));

        menuPage->addWidget(screen, "SCREEN_" + QString::number(i));
        menuStack->addPage(menuPage, menuName);

        auto item = new QListWidgetItem(menuItems); // Add a representation in the iconview
        item->setText(menuText);
        item->setIcon(QPixmap(menuPixmap));
        itemRelantions.insert(item, menuName); // Save menuName to show on item click
    }
    menuStack->setCurrentPage(Menus::MAIN_MENU);
    return menuItems;
}

void MainScreenMenu::setStyles(QListView& listView, XmlConfig& xml)
{
    //@benes    listView.setItemsMovable(false);
    auto tmpStr = xml.readString("backgroundcolor");

    auto m_palette = palette();
    if (!tmpStr.isEmpty())
        m_palette.setColor(backgroundRole(), QColor(tmpStr));
    setPalette(m_palette);

    // Sets background pixmap if there is one
    tmpStr = xml.readString("backgroundpixmap");
    if (!tmpStr.isEmpty())
        /* mainmenu_iconview->setPaletteBackgroundPixmap(QPixmap(tmp_str))*/;

    // Sets the font family
    tmpStr = xml.readString("fontfamily");
    QFont font;
    if (!tmpStr.isEmpty() && tmpStr == "SansSerif")
        font.setFamily("SansSerif");

    // Sets the font size
    tmpStr = xml.readString("fontsize");
    bool ok = false;
    int tmpInt = 0;
    if (!tmpStr.isEmpty()) {
        tmpInt = tmpStr.toInt(&ok);
        if (ok && tmpInt > 0)
            font.setPixelSize(tmpInt);
    }

    tmpStr = xml.readString("bold");
    if (!tmpStr.isEmpty())
        font.setBold(true);

    tmpStr = xml.readString("underline");
    if (!tmpStr.isEmpty())
        font.setUnderline(true);

    tmpStr = xml.readString("italic");
    if (!tmpStr.isEmpty())
        font.setItalic(true);

    tmpStr = xml.readString("strikeout");
    if (!tmpStr.isEmpty())
        font.setStrikeOut(true);

    listView.setFont(font);

    int gridWidth { 0 }, gridHeight { 0 };
    auto tmpNum = xml.readString("ygridsize");
    if (!tmpNum.isEmpty()) {
        tmpInt = tmpNum.toInt(&ok);
        if (ok)
            gridWidth = tmpInt;
    }

    tmpNum = xml.readString("xgridsize");
    if (!tmpNum.isEmpty()) {
        tmpInt = tmpNum.toInt(&ok);
        if (ok)
            gridHeight = tmpInt;
    }

    listView.setGridSize(QSize(gridWidth, gridHeight));

    // Sets grid spacing inside iconview
    auto spacing = xml.readString("gridspacing");
    if (!spacing.isEmpty()) {
        tmpInt = spacing.toInt(&ok);
        if (ok)
            listView.setSpacing(tmpInt);
    }
}

void MainScreenMenu::setHeaderIcon(const QString& file)
{
    header->setPixmap(QPixmap(file));
}

void MainScreenMenu::setFooterIcon(const QString& file)
{
    footer->setPixmap(QPixmap(file));
}

void MainScreenMenu::setHeaderVisible(bool visible)
{
    header->setVisible(visible);
}

void MainScreenMenu::setFooterVisible(bool visible)
{
    footer->setVisible(visible);
}

void MainScreenMenu::setCurrentMenu(QListWidgetItem* item)
{
    QString cmdStr;
    if (itemRelantions.contains(item))
        cmdStr = itemRelantions[item];

    if (cmdStr.isEmpty())
        return;

    if (cmdStr.contains("EXIT")) {
        system("/usr/bin/killall gs >/dev/null 2>&1"); //@linux
        exit(0);
    }

    setFooterVisible(false);
    setCurrentMenu(cmdStr);
}

void MainScreenMenu::setCurrentMenu(const QString& menu)
{
    menuStack->setCurrentPage(menu);
}

void MainScreenMenu::paintEvent(QPaintEvent* event)
{
    menuItems->clearSelection();
    QWidget::paintEvent(event);
}

void MainScreenMenu::resizeEvent(QResizeEvent* event)
{
    menuItems->doItemsLayout();
    QWidget::resizeEvent(event);
}

void MainScreenMenu::showEvent(QShowEvent* event)
{
    menuItems->doItemsLayout();
    QWidget::showEvent(event);
}
