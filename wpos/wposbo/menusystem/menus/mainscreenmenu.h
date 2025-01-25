/***************************************************************************
                          bslmainscreenwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#ifndef MAINSCREENMENU_H
#define MAINSCREENMENU_H

#include <QMap>
#include <QString>
#include <QWidget>

class XmlConfig;
class QVBoxLayout;
class QLabel;
class MenuStack;
class MenuPage;
class QListWidget;
class QListWidgetItem;
class QListView;
class QPaintEvent;

class MainScreenMenu : public QWidget {
    Q_OBJECT

public:
    explicit MainScreenMenu(
        MenuPage* parent,
        const QString& name);

    ~MainScreenMenu() = default;

public slots:
    void setCurrentMenu(const QString& menu);
    void setHeaderVisible(bool visible);
    void setFooterVisible(bool visible);
    void setHeaderIcon(const QString& file);
    void setFooterIcon(const QString& file);

protected:
    bool loadConfig(XmlConfig& xml) const;
    QListWidget* buildMenus(MenuStack* menuStack, XmlConfig& xml);
    void setStyles(QListView& listView, XmlConfig& xml);

    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;

    //@benes raiseMenuSlot(QListWidgetItem *item) was public, I made it protected
    // We will examine the global design and see if it's worth while
    void setCurrentMenu(QListWidgetItem* item); // setCurrentWidget

private:
    QVBoxLayout* mainLayout {};
    MenuStack* menuStack {};
    QListWidget* menuItems {};
    QLabel* header {}; // description and pixmap of window header
    QLabel* footer {}; // description and pixmap of window footer

    XmlConfig* xml {}; // will be used to parse the mainscreen XML description file
    QMap<QListWidgetItem*, QString> itemRelantions;
};

#endif
