/***************************************************************************
                          persticketwidget.h  -  description
                             -------------------
    begin                : mar may 11 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PERSTICKETWIDGET_H
#define PERSTICKETWIDGET_H

#include "ui_persticketswidgetbase.h"
#include <wposcore/config.h>
#include "database/historicdb.h"

class QDate;
class HistoricDB;
class QPixmap;
class OrderView;
class QTreeWidgetItem;
class XmlConfig;
class QTimer;

class PersTicketWidget :
        public QWidget , private Ui::PersTicketsWidgetBase {
    Q_OBJECT
public:
    explicit PersTicketWidget(QWidget *parent=0, const QString& name=QString());
    ~PersTicketWidget();

public slots:
    void clear();
    void clearOrder();

    void upTicketSlot();
    void downTicketSlot();

    void firstTicketSlot();
    void lastTicketSlot();

    void tableClickedSlot();


    void ticketSelectedSlot(QTreeWidgetItem* item);

    void timerDone();

signals:
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);

protected:
    void showEvent(QShowEvent *e);
    void getPersTickets();

    bool checkAllValues();

    void startGetingZ();
    void stopGettingZ();

private:
    void layoutItems();

    QPixmap null_pixmap{"controls:view_text.png"};
    QPixmap error_pixmap{"controls:stop.png"};
    QPixmap anulation_pixmap{"controls:view_text_red.png"};

    HistoricDB database{"historicTicketConnection", Files::configFilePath("database")};
    OrderView *order{};
    QTimer *timer{};
    int progress_step{};
};

#endif
