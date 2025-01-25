/***************************************************************************
                          bslscreeninvitationsdesignwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "invitationscreendesignwidget.h"
#include "addinvitationscreenwidget.h"
#include "deleteinvitationscreenwidget.h"
#include "deleteofferwidget.h"

#include "menusystem/utils.h"

#include "menusystem/products/productoffercreationwidget.h"

#include <wposgui/common/dragobjects.h>

#include <QButtonGroup>
#include <QCheckBox>
#include <QColor>
#include <QComboBox>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QFont>
#include <QIconDragEvent>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QLocale>
#include <QMenu>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QRect>
#include <QScrollArea>
#include <QStackedWidget>
#include <QTimer>
#include <QToolBox>
#include <QVBoxLayout>

const QString& INVITATION_OFFERS_DTD = "dtddocs:products_offerslist.dtd";
const QString& INVITATION_OFFERS_XML = "xmldocs:invitations_description.xml";

const QColor COLOR_POP_UP = QColor(255, 222, 115);
const QColor COLOR_POP_DOWN = QColor(238, 238, 230);
const QColor COLOR_POP_DELETE = QColor(255, 0, 0);

constexpr QSize ICON_VIEW_SIZE = QSize(270, 480);
constexpr QSize ICON_VIEW_GRID_SIZE = QSize(70, 50);

const double ICON_SIZE = 30.00;
const QString ICON_PATH = "controls:offers/";

const int OFFER_GROUP = 0;
const int SCREEN_GROUP = 1;

static const uint TIME_OUT { 10 };

#include <iostream>
using namespace std;

InvitationScreenDesignWidget::InvitationScreenDesignWidget(QWidget* parent, const QString& name)
    : QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    QFont font;
    QHBoxLayout* hlayout = 0;
    QVBoxLayout* vlayout = 0;

    /* ToolBox which shows the offers*/
    offers_box = new QToolBox(offers_frame);
    offers_box->setObjectName("offers_box");
    offers_box->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    offers_box->setMinimumHeight(450);
    if (!(vlayout = qobject_cast<QVBoxLayout*>(offers_frame->layout())))
        vlayout = new QVBoxLayout(offers_frame);
    vlayout->insertWidget(0, offers_box);
    font = offers_box->font();
    font.setPointSize(16);
    offers_box->setFont(font);

    /* QListWidget which shows the offer at the current screen */
    screen_selected = new BslDDIconView(frame_screens, "screen_selected");
    if (!(hlayout = qobject_cast<QHBoxLayout*>(frame_screens->layout())))
        hlayout = new QHBoxLayout(frame_screens);
    hlayout->addWidget(screen_selected);
    screen_selected->setGridSize(QSize(90, 60));

    /* QListWidget which shows all the screens*/
    scroll_screen = new QScrollArea(frame_scroll_screens);
    scroll_screen->setObjectName("scroll_screen");
    scroll_screen->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_screen->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_screen->setFixedSize(450, 95);
    screens_view = new QListWidget(scroll_screen);
    screens_view->setViewMode(QListView::IconMode);
    screens_view->setObjectName("screens_view");

    screens_view->setGridSize(QSize(80, 60));
    //@benes    screens_view->setArrangement(QIconView::LeftToRight);
    screens_view->setFixedSize(100, 95);
    //@benes    screens_view->setWordWrapIconText(false);
    screens_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    screens_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if (!(hlayout = qobject_cast<QHBoxLayout*>(scroll_screen->viewport()->layout())))
        hlayout = new QHBoxLayout(scroll_screen->viewport());
    hlayout->addWidget(screens_view);
    scroll_screen->setWidget(screens_view);

    left_button->setIcon(QPixmap("controls32:back.png"));
    right_button->setIcon(QPixmap("controls32:next.png"));
    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    cancel_button->setIcon(QPixmap("controls48:reload.png"));

    /*  Popups */
    /* Add new screen */
    invitations_menu = new QMenu(this);
    invitations_menu->setObjectName("invitations");
    invitations_menu_item = new AddInvitationScreenWidget(invitations_menu, "invitations_widget");
    invitations_menu_item->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (!(hlayout = qobject_cast<QHBoxLayout*>(invitations_menu->layout())))
        hlayout = new QHBoxLayout(invitations_menu);
    hlayout->addWidget(invitations_menu_item);
    invitations_menu->addMenu(invitations_menu_item);
    //@benes    invitations_menu->setLineWidth(0);

    /* Update a screen */
    update_invitations_menu = new QMenu(this);
    update_invitations_menu->setObjectName("update_invitations");
    update_invitations_menu_item = new AddInvitationScreenWidget(update_invitations_menu, "update_invitations_widget");
    update_invitations_menu_item->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (!(hlayout = qobject_cast<QHBoxLayout*>(update_invitations_menu->layout())))
        hlayout = new QHBoxLayout(update_invitations_menu);
    hlayout->addWidget(update_invitations_menu_item);
    update_invitations_menu->addMenu(update_invitations_menu_item);
    //@benes    update_invitations_menu->setLineWidth(0);

    /* Delete a screen */
    delete_invitations_menu = new QMenu(this);
    delete_invitations_menu->setObjectName("delete_invitations");
    delete_invitations_widget = new DeleteInvitationScreenWidget(delete_invitations_menu, "delete_invitations_widget");
    if (!(hlayout = qobject_cast<QHBoxLayout*>(delete_invitations_menu->layout())))
        hlayout = new QHBoxLayout(delete_invitations_menu);
    hlayout->addWidget(delete_invitations_widget);
    delete_invitations_menu->addMenu(delete_invitations_widget);
    //@benes    delete_invitations_menu->setLineWidth(0);

    left_button->setAutoRepeat(true);
    right_button->setAutoRepeat(true);

    /* Connections*/
    connect(ok_button, &QPushButton::released, this, &InvitationScreenDesignWidget::acceptSlot);
    connect(cancel_button, &QPushButton::released, this, &InvitationScreenDesignWidget::cancelSlot);

    //@benes    connect(screen_selected, SIGNAL(dropped(QDropEvent *, const QList<QIconDragItem>&)), this, SLOT(offerDropped(QDropEvent *, const  QList<QIconDragItem>&)));
    connect(screens_view, &QListWidget::currentItemChanged,
        this, &InvitationScreenDesignWidget::screenChanged);
    connect(this, &InvitationScreenDesignWidget::offerReaded, &InvitationScreenDesignWidget::offerReadedSlot);

    connect(left_button, &QPushButton::released, this, &InvitationScreenDesignWidget::leftButtonClicked);
    connect(right_button, &QPushButton::released, this, &InvitationScreenDesignWidget::rightButtonClicked);
    connect(delete_offer_from_screen, &QPushButton::released, this, &InvitationScreenDesignWidget::deleteOfferFromScreen);
    connect(delete_screen_button, &QPushButton::released, this, &InvitationScreenDesignWidget::deleteScreenClicked);

    connect(delete_invitations_widget, &DeleteInvitationScreenWidget::screenDeleted, this, &InvitationScreenDesignWidget::screenDeletedSlot);
    connect(delete_invitations_menu, &QMenu::aboutToShow, this, &InvitationScreenDesignWidget::showPopDeleteScreen);
    connect(delete_invitations_menu, &QMenu::aboutToHide, this, &InvitationScreenDesignWidget::hidePopDeleteScreen);

    connect(new_screen_button, &QPushButton::released, this, &InvitationScreenDesignWidget::newScreenClicked);
    connect(invitations_menu_item, &AddInvitationScreenWidget::screenCreated, this, &InvitationScreenDesignWidget::screenCreatedSlot);
    connect(invitations_menu, &QMenu::aboutToShow, this, &InvitationScreenDesignWidget::showPopNewScreen);
    connect(invitations_menu, &QMenu::aboutToHide, this, &InvitationScreenDesignWidget::hidePopNewScreen);

    connect(update_screen_button, &QPushButton::released, this, &InvitationScreenDesignWidget::updateScreenClicked);
    connect(update_invitations_menu_item, &AddInvitationScreenWidget::screenUpdated, this, &InvitationScreenDesignWidget::screenUpdatedSlot);
    connect(update_invitations_menu, &QMenu::aboutToShow, this, &InvitationScreenDesignWidget::showPopUpdateScreen);
    connect(update_invitations_menu, &QMenu::aboutToHide, this, &InvitationScreenDesignWidget::hidePopUpdateScreen);

    screen_stack->setCurrentWidget(reading_page);
}

void InvitationScreenDesignWidget::acceptSlot()
{
    writeXml();
}

void InvitationScreenDesignWidget::cancelSlot()
{
    startShowing();
    emit cancel();
}

void InvitationScreenDesignWidget::leftButtonClicked()
{
    scroll_screen->scroll(-20, 0);
}

void InvitationScreenDesignWidget::rightButtonClicked()
{
    scroll_screen->scroll(20, 0);
}

//@benes
// void BslScreenInvitationsDesignWidget::offerDropped(QDropEvent *e, const QList<QIconDragItem> & lst){
//    QString type, name;
//    ProductOfferData *data = 0;
//    ProductOfferScreenData *screen = 0;

//    if (!screens_view->currentItem())
//        return;

//    screen = screen_data_list.find((screens_view->currentItem())->text());

//    if (!screen)
//        return;

//    if (!QTextDrag::decode(e, name))
//        return;

//    type = offers_box->itemLabel(offers_box->currentIndex());
//    data = screen->list->find(type+"-"+name);

//    if (data)
//        return;

//    data = this->getOffer(type,name);
//    if (!data)
//        return;

//    screen->list->append(data,data->offer_type+"-"+data->offer_name);
//    this->showScreen(screen->text);
//}

void InvitationScreenDesignWidget::screenChanged(
    QListWidgetItem* item, QListWidgetItem* previous)
{
    (void)previous;
    if (!item)
        return;
    showScreen(item->text());
}

/**
 *       Delete offer Method
 */
void InvitationScreenDesignWidget::deleteOfferFromScreen()
{
    QString key;
    QListWidgetItem* item = 0;
    ProductOfferScreenData* screen = 0;

    item = screens_view->currentItem();
    if (!item)
        return;

    screen = screen_data_list.find(item->text());
    if (!screen)
        return;

    item = screen_selected->currentItem();
    if (!item)
        return;

    key = item->text();
    screen->product_offers.remove(key + "-" + item->text());
    showScreen(screen->text);
}

/**
 *       Delete Screen Methods.
 */
void InvitationScreenDesignWidget::deleteScreenClicked()
{
    QListWidgetItem* item = 0;
    int x, y;
    item = screens_view->currentItem();
    if (item)
        delete_invitations_widget->setScreen(item->text());
    else
        delete_invitations_widget->showSelectScreen();

    x = frame_delete_screen->mapToGlobal(QPoint(0, 0)).x() + frame_delete_screen->width() - delete_invitations_widget->width();
    y = frame_delete_screen->mapToGlobal(QPoint(0, 0)).y() - delete_invitations_widget->height();
    delete_invitations_menu->exec(QPoint(x, y));
}

void InvitationScreenDesignWidget::showPopDeleteScreen()
{
    frame_delete_screen->setPalette(QPalette(COLOR_POP_DELETE));
    setEnabledScreen(false, SCREEN_GROUP);
    delete_invitations_menu->setEnabled(true);
    frame_delete_screen->setEnabled(true);
    delete_screen_button->setEnabled(true);
}

void InvitationScreenDesignWidget::hidePopDeleteScreen()
{
    frame_delete_screen->setPalette(palette());
    setEnabledScreen(true, SCREEN_GROUP);
}

void InvitationScreenDesignWidget::screenDeletedSlot(bool deleted)
{
    QListWidgetItem* item = screens_view->currentItem();
    if (item && deleted) {
        QString text = item->text();
        screen_data_list.remove(text);
        showViews();
    }

    delete_invitations_menu->hide();
}

/**
 *       New Screen Methods
 */
void InvitationScreenDesignWidget::newScreenClicked()
{
    auto x = frame_new_screen->mapToGlobal(QPoint(0, 0)).x();
    auto y = frame_new_screen->mapToGlobal(QPoint(0, 0)).y() - invitations_menu_item->height();
    invitations_menu->exec(QPoint(x, y));
}

void InvitationScreenDesignWidget::showPopNewScreen()
{
    frame_new_screen->setPalette(palette());
    setEnabledScreen(false, SCREEN_GROUP);
    invitations_menu->setEnabled(true);
    frame_new_screen->setEnabled(true);
    new_screen_button->setEnabled(true);
}

void InvitationScreenDesignWidget::hidePopNewScreen()
{
    frame_new_screen->setPalette(palette());
    setEnabledScreen(true, SCREEN_GROUP);
}

void InvitationScreenDesignWidget::screenCreatedSlot(ProductOfferScreenData* new_screen)
{
    ProductOfferScreenData* main_screen = 0;
    if (!new_screen) {
        invitations_menu->hide();
        return;
    }

    if (new_screen->is_default) {
        main_screen = screen_data_list.find(this->getMainWidgetName());
        if (main_screen)
            main_screen->is_default = false;
    }
    screen_data_list.append(new_screen, new_screen->text);
    showViews();
    invitations_menu->hide();
}

/**
 *       Update Screen Slots
 */
void InvitationScreenDesignWidget::updateScreenClicked()
{
    QListWidgetItem* item = screens_view->currentItem();
    if (!item)
        return;

    ProductOfferScreenData* screen = screen_data_list.find(item->text());
    if (!screen)
        return;

    update_invitations_menu_item->setScreen(screen);
    auto x = frame_update_screen->mapToGlobal(QPoint(0, 0)).x() + frame_update_screen->width() - update_invitations_menu_item->width();
    auto y = frame_update_screen->mapToGlobal(QPoint(0, 0)).y() - update_invitations_menu_item->height();
    update_invitations_menu->exec(QPoint(x, y));
}

void InvitationScreenDesignWidget::screenUpdatedSlot(ProductOfferScreenData* new_screen, const QString& old_screen)
{

    ProductOfferScreenData* main_screen = 0;

    if (!new_screen || old_screen.isEmpty()) {
        update_invitations_menu->hide();
        return;
    }

    if (!screen_data_list.remove(old_screen)) {
        update_invitations_menu->hide();
        return;
    }

    ProductOfferScreenData* screen = new ProductOfferScreenData(new_screen);
    if (!screen) {
        update_invitations_menu->hide();
        return;
    }

    if (screen->is_default) {
        main_screen = screen_data_list.find(this->getMainWidgetName());
        if (main_screen)
            main_screen->is_default = false;
    }
    screen_data_list.append(screen, screen->text);
    update_invitations_menu->hide();
    showViews();
}

void InvitationScreenDesignWidget::showPopUpdateScreen()
{
    frame_update_screen->setPalette(palette());
    setEnabledScreen(false, SCREEN_GROUP);
    update_invitations_menu->setEnabled(true);
    frame_update_screen->setEnabled(true);
    update_screen_button->setEnabled(true);
}

void InvitationScreenDesignWidget::hidePopUpdateScreen()
{
    frame_update_screen->setPalette(palette());
    setEnabledScreen(true, SCREEN_GROUP);
}

void InvitationScreenDesignWidget::setEnabledScreen(bool enabled, const int group_pressed)
{
    if (group_pressed == OFFER_GROUP) {
        new_screen_button->setEnabled(enabled);
        update_screen_button->setEnabled(enabled);
        delete_screen_button->setEnabled(enabled);
        delete_offer_from_screen->setEnabled(enabled);
    }

    frame_screens->setEnabled(enabled);
    frame_scroll_screens->setEnabled(enabled);
    offers_box->setEnabled(enabled);
    ok_button->setEnabled(enabled);
    cancel_button->setEnabled(enabled);
}

void InvitationScreenDesignWidget::showConfig()
{
    showToolBox();
    showViews();
}

void InvitationScreenDesignWidget::showToolBox()
{
    int j, offer_count;
    QString xml_string;
    QString offer_name, offer_pixmap;
    QListWidgetItem* item = 0;
    QFont font;

    clearToolBox();
    ProductOfferModule model;
    XmlConfig xml;
    if (!xml.readXmlFromString(model.getOfferTypes())) {
        cerr << "cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if (!xml.validateXmlWithDTD(INVITATION_OFFERS_DTD, true)) {
        cerr << "file does not conform a valid xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    xml.delDomain();
    xml.setDomain("offers");
    for (auto i = 0; i < xml.howManyTags("offer"); i++) {
        xml.setDomain("offer[" + QString::number(i) + "]");
        auto offer_type = xml.readString("offer_type");
        auto icon_view = new BslDDIconView(offers_box, offer_type);
        icon_view->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
        icon_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // to override the toolbox font size
        QFont font = offers_box->font();
        font.setPointSize(11);
        icon_view->setFont(font);
        offers_box->addItem(icon_view, offer_type);
        xml.releaseDomain("offer");
        xml_string = model.getOffers(offer_type);

        XmlConfig xml_offer;
        if (!xml_offer.readXmlFromString(model.getOffers(offer_type))) {
            cerr << "cannot transform the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            return;
        }

        if (!xml_offer.validateXmlWithDTD(INVITATION_OFFERS_DTD, true)) {
            cerr << "the xml does not conform a valid xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml_offer.debug();
            return;
        }

        xml_offer.setDomain("offers");
        offer_count = xml_offer.howManyTags("offer");
        progress->setMinimum(0);
        progress->setMaximum(offer_count);
        for (auto j = 0; j < offer_count; j++) {
            xml_offer.setDomain("offer[" + QString::number(j) + "]");
            offer_name = xml_offer.readString("offer_name");
            offer_pixmap = xml_offer.readString("logo");
            item = new QListWidgetItem(icon_view);
            item->setText(offer_name);
            emit offerReaded(j, tr("Reading all offers from the database"), tr("Type: ") + offer_type + tr("  Name: ") + offer_name);

            if (!offer_pixmap.isEmpty()) {
                auto iconfile = ICON_PATH + offer_pixmap;
                item->setIcon(cropedIcon(iconfile, ICON_SIZE));
            }
            xml_offer.releaseDomain("offer");
        }

        //@benes        icon_view->setArrangement(QIconView::LeftToRight);
        icon_view->setGridSize(ICON_VIEW_GRID_SIZE);
        //@benes         icon_view->setMaxItemTextLength(70);
        icon_view->resize(ICON_VIEW_SIZE);
        icon_view->show();
    }
    offers_box->setCurrentIndex(0);
}

void InvitationScreenDesignWidget::showViews()
{
    clearViews();
    QString main_widget_name = getMainWidgetName();
    if (!main_widget_name.isNull() && !main_widget_name.isEmpty()) {
        ProductOfferScreenData* screen = screen_data_list.find(main_widget_name);
        auto item = new QListWidgetItem(screen->text, screens_view);
        if (!screen->pixmap.isEmpty())
            item->setIcon(QPixmap(screen->pixmap));
    }

    for (const auto* screen : screen_data_list) {
        if (screen->text == main_widget_name)
            continue;
        auto item = new QListWidgetItem(screen->text, screens_view);
        if (!screen->pixmap.isEmpty())
            item->setIcon(QPixmap(screen->pixmap));
    }

    screens_view->setCurrentItem(screens_view->item(0));
    screens_view->scrollToTop();
}

void InvitationScreenDesignWidget::showScreen(const QString& screen_name)
{

    if (screen_name.isEmpty())
        return;
    ProductOfferScreenData* screen = screen_data_list.find(screen_name);

    if (!screen)
        return;
    screen_selected->clear();
    for (const auto* data : screen->product_offers) {
        auto item = new QListWidgetItem(data->offer_name, screen_selected);
        item->setText(data->offer_type);
        if (!data->logo.isEmpty())
            item->setIcon(QPixmap(ICON_PATH + data->logo));
    }
}

void InvitationScreenDesignWidget::clear()
{
    screen_data_list.clear();
    clearToolBox();
    clearViews();
}

void InvitationScreenDesignWidget::clearToolBox()
{
    auto count = offers_box->count();
    for (auto i = 0; i > count; i++) {
        QWidget* w = offers_box->widget(0);
        offers_box->removeItem(0);
        w->deleteLater();
    }
}

void InvitationScreenDesignWidget::clearViews()
{
    screens_view->clear();
    screen_selected->clear();
}

void InvitationScreenDesignWidget::writeXml()
{

    QFile file(INVITATION_OFFERS_XML);
    if (file.exists())
        file.remove();

    XmlConfig xml;
    xml.createElementSetDomain("menuentries");
    for (auto* screen : screen_data_list) {
        xml.createElementSetDomain("menuentry");
        xml.createElement("entry.name", screen->name);
        xml.createElement("entry.text", screen->text);
        xml.createElement("entry.pixmap", screen->pixmap);

        if (screen->visible)
            xml.createElement("entry.visible", "true");
        else
            xml.createElement("entry.visible", "false");

        if (screen->enabled)
            xml.createElement("entry.enabled", "true");
        else
            xml.createElement("entry.enabled", "false");

        if (screen->is_default) {
            xml.pushDomain();
            xml.delDomain();
            xml.doWrite("defaultentry", screen->name);
            xml.popDomain();
        }

        xml.createElementSetDomain("menu");
        xml.createElement("name", screen->name);
        xml.createElementSetDomain("items");
        for (const auto* offer_data : screen->product_offers) {
            xml.createElementSetDomain("item");
            xml.createElement("text", offer_data->offer_name);
            xml.createElement("pixmap", ICON_PATH + offer_data->logo);
            xml.createElement("offer.type", offer_data->offer_type);
            xml.createElement("offer.name", offer_data->offer_name);
            xml.releaseDomain("item");
        }
        xml.releaseDomain("menuentry");
    }
    xml.delDomain();
    xml.save(INVITATION_OFFERS_XML);
}

void InvitationScreenDesignWidget::readXml()
{

    if (!QFile(INVITATION_OFFERS_XML).exists())
        return;

    XmlConfig xml(INVITATION_OFFERS_XML);
    if (!xml.wellFormed())
        return;

    auto default_screen = xml.readString("defaultentry");
    if (!xml.setDomain("menuentries"))
        return;

    progress->setMinimum(0);
    progress->setMaximum(xml.findNode("type").count());
    int pos = 0;
    for (auto i = 0; i < xml.howManyTags("menuentry"); i++) {
        if (!xml.setDomain("menuentry[" + QString::number(i) + "].entry"))
            continue;
        auto screen = new ProductOfferScreenData();
        auto tmp_str = xml.readString("name");

        if (tmp_str == default_screen)
            screen->is_default = true;

        screen->name = tmp_str;
        screen->text = xml.readString("text");
        screen->pixmap = xml.readString("pixmap");
        tmp_str = xml.readString("enabled");
        if (tmp_str.toLower() == "true")
            screen->enabled = true;
        tmp_str = xml.readString("visible");
        if (tmp_str.toLower() == "true")
            screen->visible = true;

        xml.releaseDomain("entry");
        if (!isVisible())
            return;
        screen_data_list.append(screen, screen->text);

        if (!xml.setDomain("menu.items")) {
            xml.releaseDomain("menuentry");
            continue;
        }

        for (auto j = 0; j < xml.howManyTags("item"); j++) {
            if (!xml.setDomain("item[" + QString::number(j) + "].offer"))
                continue;
            auto offer_type = xml.readString("type");
            auto offer_name = xml.readString("name");
            ProductOfferData* offer_data = getOffer(offer_type, offer_name);
            if (offer_data) {
                if (!isVisible())
                    return;
                screen->product_offers.append(offer_data, offer_data->offer_type + "-" + offer_data->offer_name);
                emit offerReaded(++pos, screen->text, "Type: " + offer_data->offer_type + "  Name: " + offer_data->offer_name);
            }
            xml.releaseDomain("item");
        }
        xml.releaseDomain("menuentry");
    }
}

ProductOfferData* InvitationScreenDesignWidget::getOffer(QString offer_type, QString offer_name)
{

    ProductOfferModule model;
    QString xml_string;

    if (offer_type.isEmpty() || offer_name.isEmpty())
        return nullptr;

    // scoped operation for xml object, generate the xml
    {
        XmlConfig xml;
        xml.createElement("offers.offer.offer_type", offer_type);
        xml.createElement("offers.offer.offer_name", offer_name);

        if (!xml.validateXmlWithDTD(INVITATION_OFFERS_DTD, true)) {
            cerr << "The file does not conform a valid xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            return nullptr;
        }

        xml_string = model.getOffer(xml.toString());
    }

    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "The string can not be converted into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return nullptr;
    }

    if (!xml.validateXmlWithDTD(INVITATION_OFFERS_DTD, true)) {
        cerr << "The file does not conform a valid xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return nullptr;
    }

    auto data = new ProductOfferData;
    xml.delDomain();
    xml.setDomain("offers.offer");
    data->offer_type = xml.readString("offer_type");
    data->offer_name = xml.readString("offer_name");
    data->description_offer = xml.readString("description_offer");
    data->logo = xml.readString("logo");
    data->cpp_operator = model.getCppOperator(data->offer_type);
    data->description_type = model.getDescriptionOfferType(data->offer_type);
    data->is_default = false;
    data->value = 0.0;

    return data;
}

QString InvitationScreenDesignWidget::getMainWidgetName()
{

    for (const auto* screen : screen_data_list) {
        if (screen->is_default)
            return screen->text;
    }
    return QString {};
}

void InvitationScreenDesignWidget::showEvent(QShowEvent* e)
{
    if (!e->spontaneous()) {
        if (first_show)
            first_show = false;
        else
            QTimer::singleShot(TIME_OUT, this, &InvitationScreenDesignWidget::startShowing);
    }
    QWidget::showEvent(e);
}

void InvitationScreenDesignWidget::hideEvent(QHideEvent* e)
{
    clear();
    screen_data_list.clear();
    screen_stack->setCurrentWidget(reading_page);
    QWidget::hideEvent(e);
}

void InvitationScreenDesignWidget::startShowing()
{
    screen_stack->setCurrentWidget(reading_page);
    clear();
    readXml();
    showConfig();
    screen_stack->setCurrentWidget(main_page);
}

void InvitationScreenDesignWidget::offerReadedSlot(int num, const QString& screen_name, const QString offer_label)
{
    progress->setValue(num);
    screen_name_label->setText(screen_name);
    offer_name_label->setText(offer_label);
    qApp->processEvents();
}
