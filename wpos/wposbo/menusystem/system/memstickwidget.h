/***************************************************************************
                          memstickwidget.h  -  description
                             -------------------
    begin                : lun jul 26 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/
/**
  *@author Carlos Manzanedo
  */


#ifndef MEMSTICKWIDGET_H
#define MEMSTICKWIDGET_H

#include <QWidget>
#include "memstickwidgetbase.h"
#include "memstickinterface.h"

class QTimer;
class QListWidgetItem;

class MemStickWidget :
        public MemStickWidgetBase,
        virtual public MemStickInterface
{
    Q_OBJECT
public:
    MemStickWidget(QWidget *parent=0, const QString& name=QString());
    ~MemStickWidget();

public slots:
    void saveIconsToStickSlot();
    void saveIconsFromStickSlot();
    void saveDatabaseToStickSlot();
    void saveDatabaseCSVToStickSlot();
    void getDatabaseFromStickClickedSlot();

    void operationResultSlot(bool status);
    void acceptClickedSlot();

    void cancelClicked();
    void okClicked();


    void dbSelectionChangedSlot(QListWidgetItem *item);
    void dbNameChangedSlot(const QString& name);

protected slots:
    void startTimer();
    void timerSlot();
    void stopTimer();

protected:
    void init();
    void showEvent(QShowEvent *e);
    bool checkStickModule();
    bool showErrorScreen();

    void clearDatabasePage();
    void checkAllDB();

    int pos;
    QTimer *timer;

};

#endif
