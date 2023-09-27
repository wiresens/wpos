/***************************************************************************
                          bslorderheaderwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
Modified by Carlos Manzanedo Rueda.

%LICENCIA%
 ***************************************************************************/

#ifndef ORDERHEADERVIEW_H
#define ORDERHEADERVIEW_H

#include <wposcore/hlist.h>

#include <QFrame>

class QPixmap;
class QLabel;
class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;
class XmlConfig;

class OrderHeaderView : public QFrame  {
    Q_OBJECT
public:
    OrderHeaderView(XmlConfig *xml, QWidget *parent, QString name);
    ~OrderHeaderView();

public slots:
    void showPhoto(bool show_it = true);
    void setStaffInfo(XmlConfig *xml);

protected:
    void parseXmlDescription(XmlConfig *xml);

    QHBoxLayout *total_layout {};
    QFrame *labels_frame {};
    QBoxLayout *layout {};
    HList<QLabel> *widgets {};
};

#endif
