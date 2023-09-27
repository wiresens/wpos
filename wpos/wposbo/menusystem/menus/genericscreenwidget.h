/***************************************************************************
                          bslgenericscreenwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef GENERICSCREENWIDGET_H
#define GENERICSCREENWIDGET_H

#include <QWidget>

class QString;
class XmlConfig;
class QHBoxLayout;
class TreeView;
class MenuStack;
class QTreeWidgetItem;
class QShowEvent;

class GenericScreenWidget : public QWidget  {
    Q_OBJECT
public:
    enum Column{
        Icon = 0,
        Name = 1,
        Text = Icon
    };

    explicit GenericScreenWidget(QWidget* parent=0, const QString &name=QString());

    GenericScreenWidget(XmlConfig *xml, const QString& root_node,
                           QWidget* parent=0, const QString &name=QString());

    ~GenericScreenWidget() = default;
    void showEvent(QShowEvent *e);

signals:
    void showDownTitleSignal(bool show_it); //@benes footerNeeded(bool needed)
    void raiseMenuSignal(const QString &menu_name); //setCurrentMenu(const QString &name)
    void menuIDSignal(const QString &text_id); //menuNameChanged(const QString &text_it)

public slots:    
    void raiseMenuSlot(QTreeWidgetItem* item);
    
private:
    void initThis();
    void parseXmlDescription();

    XmlConfig *xml_description{};
    QTreeWidgetItem *root{};
    QHBoxLayout *main_layout{};
    TreeView *actions_listview{};
    MenuStack *menus_stack{};

    QString root_domain;
};

#endif
