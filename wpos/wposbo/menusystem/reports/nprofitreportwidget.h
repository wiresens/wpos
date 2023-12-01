/***************************************************************************
                          nprofitreportwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NPROFITREPORTWIDGET_H
#define NPROFITREPORTWIDGET_H

#include "ui_profitreportwidgetbase.h"
#include "reportinterface.h"

#include <QWidget>

class PdfViewer;
class UserModule;
class QTimer;

class NProfitReportWidget :
    public QWidget,
    virtual public ReportInterface,
    Ui::ProfitReportWidgetBase {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.ProfitReport")

public:
    const QString DBusObjectPath{"/wpos/wposbo/ProfitReport"};
    explicit NProfitReportWidget(UserModule *user_module, QWidget *parent=0, const QString& name = QString());
    ~NProfitReportWidget();

signals:
    void reportCreated(const QString& name);

public slots:
    Q_SCRIPTABLE void reportSlot(QString) override; //ASYNC

protected slots: //@benes
    void clear();
    virtual void toggleStates(QAbstractButton* button);

    void upUsersSlot();
    void downUsersSlot();

    void advancedButtonToggleSlot(bool on);
    void usersBoxCheckedSlot(bool on);
    void selectedUserSlot();

    void acceptSlot();
    void timerSlot();

    void saveReportSlot();
    void exitFromViewSlot();

protected:
    void init();
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    virtual bool doIsClosing();
    virtual void doSelectReport(QString& start_date, QString& end_date, const QString& informe = QString());
    virtual void doSetOutputStream(QDataStream& out_stream, const QString& informe, const QString& start_date, const QString& end_date, const QString& employee_id);
    void getUsers();
    void startShowing();

    QString getActualReport();
    bool checkAllValues();
    bool checkReportModule();
    bool checkStickModule();

    UserModule *mod{};
    PdfViewer* viewer{};
    QTimer *timer{};
    QButtonGroup* button_group{};

    QString last_report_file;
    QString last_report_name;

    int pos{};

private:
//    QPushButton* interanual_button{productos_button};
//    QPushButton* anual_button{ofertas_button};
//    QPushButton* mensual_button{opciones_button};
//    QPushButton* semanal_button{prod_unitarios_button};
//    QPushButton* diario_button{ofertas_prod_unitarios_button};
//    QPushButton* cierres_button{precios_button};

};

#endif
