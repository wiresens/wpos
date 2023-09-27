/***************************************************************************
                          bslmainscreenwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#ifndef MAINSCREENWIDGET_H
#define MAINSCREENWIDGET_H

#include <QWidget>
#include <QMap>
#include <QString>

class XmlConfig;
class QVBoxLayout;
class QLabel;
class MenuStack;
class QListWidget;
class QListWidgetItem;
class QListView;
class QPaintEvent;

class MainScreenWidget : public QWidget  {
    Q_OBJECT
public:
    explicit MainScreenWidget(QWidget *parent=0, const QString& name = QString());
    ~MainScreenWidget() = default;
    
public slots:

    void raiseMenuSlot(const QString &menu); //setCurrentWidget
    void showTopTitleSlot(bool visible); //setHeaderVisible
    void showDownTitleSlot(bool show_it); //setFooterVisible

    void setTopTitlePixmap(QString file); //setHeaderIcon
    void setDownTitlePixmap(QString file); //setFooterIcon

protected:
    virtual void paintEvent(QPaintEvent *e) override;
    void parseXmlDescription();
    void buildMenus(XmlConfig& xml);
    void setStyles(QListView* listView, XmlConfig& xml);
    bool readConfigFile(XmlConfig& xml) const;

    //@benes raiseMenuSlot(QListWidgetItem *item) was public, I made it protected
    //We will examine the global design and see if it's worth while
    void raiseMenuSlot(QListWidgetItem *item); //setCurrentWidget

    QVBoxLayout *main_layout{};
    MenuStack *menus_stack{};
    QListWidget *mainmenu_iconview{};
    QLabel *header{}; //Containts description and pixmap of Window Header
    QLabel *footer{}; //Containts description and pixmap of Window Footer

    XmlConfig *xml{}; //will be used to parse the mainscreen XML description file
    QMap<void*, QString> menu_item_relantionships;
};

#endif
