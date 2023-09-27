/***************************************************************************
                          bslddbbwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#ifndef DATABASECONFIGWIDGET_H
#define DATABASECONFIGWIDGET_H

#include "ui_ddbbwidgetbase.h"
#include <QWidget>
#include <QProcess>

class QString;
class QStringList;
class DatabaseModule;
class QTimer;

class DatabaseConfigWidget :
        public QWidget ,
        private Ui::DDBBWidgetBase  {

    Q_OBJECT
public:

    explicit DatabaseConfigWidget(
            DatabaseModule *dbmod,
             QWidget *parent=0, const QString& name= QString());

    ~DatabaseConfigWidget() = default;

public slots:
    void clear();
    void cancel();
    void accept();

    void userNameChanged(const QString& text);
    void findDatabases();
    void dbNameChanged();

protected slots:
    void findServers();
    void setProgressBar();
    void endFindServers(int exitCode, QProcess::ExitStatus exitStatus);
    void endFindDatabases(int exitCode, QProcess::ExitStatus exitStatus);
    void scanButtonPressed();

    void dbConfigChanged();

protected:
    void showValues(const QStringList& captions, QListWidget* list_wgt, QPixmap icon);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

    bool getHostConfig();
    bool saveReportmanDeff(const QString& host, const QString& db_name, const QString& user,const QString& passwd);
    void setEnableCommands(bool enabled);

private:
    DatabaseModule *db_mod{};
    QProcess *worker{};
    QTimer *timer{};
    int prog_int{};
};

#endif
