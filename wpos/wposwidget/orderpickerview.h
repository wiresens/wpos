/***************************************************************************
                          bsladvancedorderwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef ORDER_MANAGER_FRAME_H
#define ORDER_MANAGER_FRAME_H

#include <wposcore/hlist.h>

#include <QtWidgets/QFrame>

class OrderView;
class QGridLayout;
class QBoxLayout;
class XmlConfig;
class QFrame;
class QPushButton;

class OrderPickerView : public QFrame  {
    Q_OBJECT
public:
    explicit OrderPickerView(QWidget *parent, QString name=QString{});

    ~OrderPickerView();
    OrderView *order();

public slots:
    void exitMainWindowSlot();
    void deleteReceiptSlot();
    void dataChangedSlot(XmlConfig *xml);
    void printAtKitchen();
    
signals:
    void dataChanged(XmlConfig *xml);
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);
    void genericSignal(const QString& signal_name);

protected:

    virtual OrderView* createOrder(QWidget* parent, XmlConfig &xml);
    virtual QFrame* createControls(QWidget* parent, XmlConfig &xml);
    void parseXmlDescription();

    HList<QPushButton> m_buttons{};
    QBoxLayout *m_main_blayout{};
    QGridLayout *m_buttons_glayout{};
    OrderView *m_order{};
    QFrame *m_buttons_frame{};
};

#endif
