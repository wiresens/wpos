/***************************************************************************
                          bslmainscreenwidget.cpp  -  description
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

#include <libbslxml/xmlconfig.h>

#include <wposwidget/menustack.h>
#include <wposwidget/menupage.h>

#include <QListWidget>
#include <QListView>
#include <QLabel>

#include <iostream>
using std::cerr;

//@benes extern GenericSignalManager *gsignal_manager; it does not use, we need to fugure why
Instantiator *menus_instantiator;

MainScreenWidget::MainScreenWidget(
        QWidget *parent, const QString& name) :
    QWidget(parent)
{
    setObjectName(name);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    menus_instantiator = new Instantiator(this, "MENUS_INSTANTIATOR");
    main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(0,0,0,0);
    main_layout->setSpacing(0);

    // Default profile for order is obtained from the description XML.
    // <PART 1> finding and parsing XML file
    XmlConfig xml;
    if( ! readConfigFile(xml) ) throw xml;

    //Create top title label
    header = new QLabel(this);
    main_layout->addWidget(header);
    auto pixmap_str = xml.readString("toptitlepixmap");
    if (!pixmap_str.isEmpty())  header->setPixmap(pixmap_str);

    //Create central menu stackedwidget as bs component
    menus_stack = new MenuStack(this);
    main_layout->addWidget(menus_stack);
//    menus_stack->setFixedHeight(620);  //@benes
    menus_stack->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    //Create top title label
    footer = new QLabel(this);
    main_layout->addWidget(footer);
    pixmap_str = xml.readString("downtitlepixmap");
    if (!pixmap_str.isEmpty())  footer->setPixmap(pixmap_str);

    // Default profile for order is obtained from the description XML. <PART 2> extracting values from XML

    xml.setDomain("menus");
    buildMenus(xml);
    connect(mainmenu_iconview, &QListWidget::itemClicked,
            this, QOverload<QListWidgetItem *>::of(&MainScreenWidget::raiseMenuSlot));
}

void MainScreenWidget::buildMenus(XmlConfig& xml){
    // Create the main screen iconview which holds all access icons
    auto page = new MenuPage(menus_stack, "MAIN_MENU");
    page->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    mainmenu_iconview = new QListWidget(page);
    mainmenu_iconview->setWordWrap(true);
    mainmenu_iconview->setViewMode(QListView::IconMode);
    mainmenu_iconview->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    mainmenu_iconview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainmenu_iconview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    page->setLayoutType(MenuPage::VBOX);
    page->addWidget(mainmenu_iconview, "SCREEN_MAIN");
    menus_stack->addPage(page,"MAIN_MENU");
    setStyles(mainmenu_iconview, xml);

    //watch out for that list of char pointers...
    for(auto i=0; i < xml.howManyTags("menu"); i++){

        // Read name, text and pixmap to create a menu entry
        auto aux_name = xml.readString( "menu[" + QString::number(i) + "].name" );
        auto aux_text = xml.readString( "menu[" + QString::number(i)+ "].text" );
        auto aux_pixmap = xml.readString( "menu[" + QString::number(i) + "].pixmap" );

        // Create a page menu and add the generic menu widget
        page = new MenuPage(menus_stack, aux_name);
        auto screen = new GenericScreenWidget(&xml, "menus.menu["+QString::number(i)+"]", page, aux_name);

        connect(screen, &GenericScreenWidget::showDownTitleSignal,
                this, &MainScreenWidget::showDownTitleSlot);
        connect(screen, &GenericScreenWidget::raiseMenuSignal,
                this, QOverload<const QString&>::of(&MainScreenWidget::raiseMenuSlot));

        page->setLayoutType(MenuPage::VBOX);
        page->addWidget(screen, "SCREEN_"+QString::number(i));
        menus_stack->addPage(page, aux_name);
        // Add a representation in the iconview
        auto item = new QListWidgetItem(mainmenu_iconview);
        //@benes        item->setDragEnabled(false);
        item->setText(aux_text);
        item->setIcon(QPixmap(aux_pixmap));

        // Save the relationship between this item and the menu that must be raised on click
        menu_item_relantionships.insert(item, aux_name);
    }
    menus_stack->setCurrentPage("MAIN_MENU");
}

void MainScreenWidget::setStyles(QListView* listView, XmlConfig& xml){
    //@benes    listView->setItemsMovable(false);
    auto tmp_str = xml.readString("backgroundcolor");

    auto m_palette = palette();
    if (!tmp_str.isEmpty()) m_palette.setColor(backgroundRole(), QColor(tmp_str));
    setPalette(m_palette);

    // Sets background pixmap if there is one
    tmp_str = xml.readString("backgroundpixmap");
    if (!tmp_str.isEmpty())
        /* mainmenu_iconview->setPaletteBackgroundPixmap(QPixmap(tmp_str))*/;

    // Sets the font family
    tmp_str = xml.readString("fontfamily");
    QFont font;
    if ( !tmp_str.isEmpty() && tmp_str == "SansSerif") font.setFamily("SansSerif");

    // Sets the font size
    tmp_str = xml.readString("fontsize");
    bool ok = false;
    int aux_int = 0;
    if (!tmp_str.isEmpty()){
        aux_int = tmp_str.toInt(&ok);
        if (ok)  font.setPixelSize(aux_int);
    }

    // Sets  bold
    tmp_str = xml.readString("bold");
    if (!tmp_str.isEmpty())  font.setBold(true);

    //Sets  underline
    tmp_str = xml.readString("underline");
    if (!tmp_str.isEmpty()) font.setUnderline(true);

    // and italic
    tmp_str = xml.readString("italic");
    if (!tmp_str.isEmpty()) font.setItalic(true);

    // and strikeout
    tmp_str = xml.readString("strikeout");
    if (!tmp_str.isEmpty()) font.setStrikeOut(true);

    listView->setFont(font);

    // Sets grid spacing inside iconview
    auto tmp_num = xml.readString("gridspacing");
    if (!tmp_num.isEmpty()){
        aux_int = tmp_num.toInt(&ok);
        if (ok) listView->setSpacing(aux_int);
    }

    // Sets column width
    tmp_num = xml.readString("xgridsize");
    int gridX{0}, gridY{0};
    if (!tmp_num.isEmpty()){
        aux_int = tmp_num.toInt(&ok);
        if (ok) gridY = aux_int; //@benes mainmenu_iconview->setGridY(aux_int);
    }

    // Sets row heightS
    tmp_num = xml.readString("ygridsize");
    if (!tmp_num.isEmpty()){
        aux_int = tmp_num.toInt(&ok);
        if (ok) gridX = aux_int ; //@benes mainmenu_iconview->setGridX(aux_int);
    }
    listView->setGridSize(QSize(gridX, gridY));
}

bool MainScreenWidget::readConfigFile(XmlConfig& xml) const{
    const QString& MAINSCREEN_DESCRIPTION_XML {"/etc/ntpv_backoffice/mainscreen_description.xml"};

    if ( !QFile::exists(MAINSCREEN_DESCRIPTION_XML) ){
        cerr << "Can't find " << MAINSCREEN_DESCRIPTION_XML.toStdString()
             << " file. [CRITICAL ERROR] Can't initialize order in proper way.";
        return false;
    }

    xml.readXmlFromString(XmlConfig(MAINSCREEN_DESCRIPTION_XML).toString());
    if ( !xml.isValid() ){
        cerr << "Can't parse " << MAINSCREEN_DESCRIPTION_XML.toStdString()
             << "file. [CRITICAL ERROR] Can't initialize order in proper way.";
        return false;
    }

    // Delete any domain and go to root node
    xml.delDomain();
    return true;
}

void MainScreenWidget::setTopTitlePixmap(QString file){
    header->setPixmap(QPixmap(file));
}

void MainScreenWidget::setDownTitlePixmap(QString file){
    footer->setPixmap(QPixmap(file));
}

void MainScreenWidget::showTopTitleSlot(bool show_it){
    if(show_it) header->show();
    else header->hide();
}

void MainScreenWidget::showDownTitleSlot(bool show_it){
    if(show_it){
        footer->show();
//        menus_stack->setFixedHeight(620);
    }
    else{
        footer->hide();
//        menus_stack->setFixedHeight(700);
    }
}

void MainScreenWidget::paintEvent(QPaintEvent *e){
    mainmenu_iconview->clearSelection();
    QWidget::paintEvent(e);
}

void MainScreenWidget::raiseMenuSlot(QListWidgetItem *item){
    QString command_str;
    if(menu_item_relantionships.contains(item))
        command_str =  menu_item_relantionships[item];
    if ( command_str.isEmpty()) return;

    if( command_str.contains("EXIT") ){
        system("/usr/bin/killall gs >/dev/null 2>&1"); //@linux
        exit(0);
    }

    showDownTitleSlot(false);
    raiseMenuSlot(command_str);
}

void MainScreenWidget::raiseMenuSlot(const QString &menu){
    menus_stack->setCurrentPage(menu);
}
