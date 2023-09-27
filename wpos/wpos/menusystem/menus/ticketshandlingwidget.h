#ifndef TICKETSHANDLINGWIDGET_H
#define TICKETSHANDLINGWIDGET_H

#include "ui_killticketswidget.h"

class KillTicketsDB;
class OrderView;
class XmlConfig;

class TicketsHandlingWidget :
        public QFrame,
        protected Ui::KillTicketsWidget
{
public:
    explicit TicketsHandlingWidget(QWidget* parent = nullptr,
            const QString& name = QString());

    virtual ~TicketsHandlingWidget();

public slots:
    virtual void handleTicketSelected() = 0;
    virtual void selectDown() final;
    virtual void selectUp() final ;
    virtual void rejectChange(){}
    virtual void refreshList(){}

//signals:
//    void genericDataSignal(const QString& signal, XmlConfig *xml);

protected:
    KillTicketsDB *ticket_db{};
    OrderView *order{};

    QPixmap null_pixmap{"/usr/share/ntpv/apps/view_text.png"};
    QPixmap error_pixmap{"/usr/share/ntpv/apps/stop.png"};
    QPixmap anulation_pixmap{"/usr/share/ntpv/apps/view_text_red.png"};
};

#endif // TICKETSHANDLINGWIDGET_H
