/***************************************************************************
                          nticketdesignwidget.h  -  description
                             -------------------
    begin                : lun abr 19 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NTICKETDESIGNWIDGET_H
#define NTICKETDESIGNWIDGET_H

#include <QWidget>

#include "ui_ticketdesigninterface.h"

class NTicketDesignWidget : public QWidget,
                            private Ui::TicketDesignInterface {

    Q_OBJECT
public:
    explicit NTicketDesignWidget(QWidget* parent = 0, const QString& name = QString {});
    ~NTicketDesignWidget() = default;

public slots:
    void acceptSlot();
    void clear();
    void somethingChanged(const QString& text);

protected:
    virtual void showEvent(QShowEvent* e) override;
    bool readXml();
    void writeXml();
};

#endif
