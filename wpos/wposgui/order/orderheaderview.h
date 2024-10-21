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

class OrderHeaderView : public QFrame{
    Q_OBJECT
public:
    OrderHeaderView(XmlConfig &headerXmlDesc, QWidget *parent, QString name);

public slots:
    void showPhoto(bool show_it = true);
    void setStaffInfo(XmlConfig *xml);

protected:
    void parseXmlDescription(XmlConfig &headerXmlDesc);

    QHBoxLayout *labelsLayout{};
    QFrame *textFrame{};
    QBoxLayout *textLayout{};
    QLabel* photoLabel{};
    QLabel* identityLabel{};
    QLabel* descLabel{};

};

#endif
