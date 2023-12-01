/***************************************************************************
                          memstickwidget.h  -  description
                             -------------------
    begin                : lun jul 26 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/

#ifndef MEMSTICKWIDGET_H
#define MEMSTICKWIDGET_H

#include <QtCore/QObject>
#include <QWidget>
#include "memstickinterface.h"

namespace Ui{
    class MemStickWidgetBase;
} // namespace Ui

class QTimer;
class QTreeWidgetItem;
class MemStickWidgetBase;

class MemStickWidget :
    public QWidget,
    public MemStickInterface
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.MemStick")

public:
    static const QString DBusObject;

    explicit MemStickWidget(QWidget *parent=0, const QString& name=QString());
    ~MemStickWidget();

public slots:
    Q_SCRIPTABLE void setOperationResult(bool status) override;
    void saveIconsToStick();
    void loadIconsFromStick();
    void saveDatabaseToStick();
    void saveDatabaseCSVToStick();
    void getDatabaseFromStick();

    void accept();
    void cancel();
    void proceed();

    void dbSelection();
    void dbName(const QString& name);

protected slots:
    void startTimer();
    void timerSlot();
    void stopTimer();

protected:
    void showEvent(QShowEvent *event) override;
    virtual bool wposStckicRunning();
    void init();   
    bool stickModuleAvailable();
    bool showErrorScreen();

    void clearDatabasePage();
    void checkAllDB();

private:
    void errorMessage(const QString& caption, const QString& msg , std::ostream& s);
    int pos;
    QTimer *timer{};
    Ui::MemStickWidgetBase* ui{};
};

#endif
