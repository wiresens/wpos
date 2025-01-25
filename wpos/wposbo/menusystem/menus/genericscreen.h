/***************************************************************************
                          genericscreen.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

 ***************************************************************************/

#ifndef GENERICSCREEN_H
#define GENERICSCREEN_H

#include <QWidget>

class QString;
class XmlConfig;
class QHBoxLayout;
class TreeView;
class MenuStack;
class QTreeWidgetItem;
class QShowEvent;

class GenericScreen : public QWidget {
    Q_OBJECT
public:
    enum Column {
        Icon = 0,
        Name = 1,
        Text = Icon
    };

    GenericScreen(
        XmlConfig& xml,
        const QString& rootDomain,
        QWidget* parent,
        const QString& name);

    ~GenericScreen() = default;
    void showEvent(QShowEvent* event) override;

signals:
    void footerRequested(bool needed);
    void menuRequested(const QString& menu_name);
    void menuChanged(const QString& menu);

public slots:
    void setCurrentMenu(QTreeWidgetItem* item);

private:
    void parseXmlDescription(
        XmlConfig& xml,
        const QString& rootDomain);

private:
    QTreeWidgetItem* rootItem {};
    MenuStack* menuStack {};
    QHBoxLayout* mainLayout {};
    TreeView* subMenus {};
};

#endif
