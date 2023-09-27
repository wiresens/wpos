/***************************************************************************
                          bslkillticketswidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef KILLTICKETSWIDGET_H
#define KILLTICKETSWIDGET_H

#include "ticketshandlingwidget.h"
#include <QEvent>
#include <QListWidget>

class QStringList;
class QPixmap;
class XmlConfig;

class KillTicketsWidget :
        public TicketsHandlingWidget
{
    Q_OBJECT

public:

    enum ResumeSection{
        Icon            = 0,
        TicketId        = 1,
        EmployeeName    = 2,
        Date            = 3,
        Total           = 4,
        Status          = 5
    };

    explicit KillTicketsWidget(QWidget *parent = nullptr,
                         const QString& name = QString());

    ~KillTicketsWidget();

public slots:
    virtual void rejectChange() override;
    virtual void handleTicketSelected() override;
    virtual void refreshList() override;

    virtual void deleteTicket();
    virtual void printTicket();
    virtual void printInvoice();

signals:
    void genericDataSignal(const QString& signal, XmlConfig *xml);

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void setTicketNegative (XmlConfig *xml);

private:
    QAbstractButton* printer_button{printer_or_lounges_button};

    QPixmap invoice_pixmap{"/usr/share/ntpv/apps/invoice.png"};
    QPixmap trash_pixmap {"/usr/share/ntpv/apps/trashcan_empty.png"};
    QPixmap printer_pixmap{"/usr/share/ntpv/apps/printer2.png"};
};

#endif
