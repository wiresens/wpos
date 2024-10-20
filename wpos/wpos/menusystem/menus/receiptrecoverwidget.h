/***************************************************************************
                          receiptrecoverwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef RECEIPTRECOVERWIDGET_H
#define RECEIPTRECOVERWIDGET_H

#include "ticketshandlingwidget.h"
#include "receiptinterface.h"

#include <QEvent>
#include <QListView>

class QPixmap;
class XmlConfig;
class NPopUpKeyboard;
class DBusReceiptQuerier;
class TicketResumeData;

class ReceiptRecoverWidget :
    public TicketsHandlingWidget,
    virtual public ReceiptInterface
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wpos.ReceiptRecover")

public:
    enum OrderSection{
        Icon            = 0,
        EmployeeName    = 1,
        CommandId       = 2,
        EmployeeId      = 3,
        Date            = 4,
        Status          = 5,
        TableCode       = 6
    };

    static const QString DBusObject;

    explicit ReceiptRecoverWidget(
        QWidget *parent = nullptr,
        const QString& name = QString{}
    );

    ~ReceiptRecoverWidget();

public slots:
    Q_SCRIPTABLE void receiptChanged(
        QString employee_id,
        QString start_time) override;

public slots:
    virtual void rejectChange() override;
    virtual void handleTicketSelected() override;
    virtual void refreshList() override;

    void getReceiptSlot();
    void usersButtonSlot(bool on);
    void clickedSlot(QTreeWidgetItem *item, int column);

    //Keyboard methods
    void kbExitClickedSlot();
    void kbTextChanged(const QString & text);

    void genericDataSignalSlot(const QString& signal_name, XmlConfig *xml);
    void setSelectedSlot();

    void newOrderClickedSlot();
    void openOrderKbExitClickedSlot();

signals:
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);
    void genericSignal(const QString& signal_name);

protected slots:
    void setUseLounge(bool enable);

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override ;
    bool eventFilter(QObject *watched, QEvent *event) override;

    bool refreshAndShow();
    QStringList getUsedTableCodes();

    using TicketResumes = QVector<TicketResumeData>;

    virtual TicketResumes
    employeeReceiptResume(const QString& employee_id);

    virtual TicketResumes
    employeeReceiptResume();

private :
    TicketResumes
    receiptResumeFromXml(XmlConfig xml);
    void showMenu();

private:
    DBusReceiptQuerier  *m_receiptQuerier{};
    NPopUpKeyboard      *m_npopup_keyboard{};
    QAbstractButton     *m_lounge_button {printer_or_lounges_button};
    QAbstractButton     *m_pay_button    {trash_button};

//    QPixmap m_pay_pixmap {"controls:redo.png"};
    QPixmap             m_pay_pixmap        {"payments:mix_mobile_paymodes.png"};
    QPixmap             m_allusers_pixmap   {"controls:kuser.png"};
    QString             m_input_text;
    bool                m_use_lounges       {false};
};
#endif
