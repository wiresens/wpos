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
    explicit TicketsHandlingWidget(
        QWidget* parent = nullptr,
        const QString& name = QString{}
    );

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
    KillTicketsDB *m_ticket_db{};
    OrderView     *m_order_view{};

    QPixmap m_null_pixmap     {"controls:view_text.png"};
    QPixmap m_error_pixmap    {"controls:stop.png"};
    QPixmap m_canceled_pixmap{"controls:view_text_red.png"};
};

#endif // TICKETSHANDLINGWIDGET_H
