/***************************************************************************
                          fx2000configwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
***************************************************************************/

#ifndef FX2000CONFIGWIDGET_H
#define FX2000CONFIGWIDGET_H

#include <QWidget>
#include "ui_fx2000configwidgetbase.h"
#include "memstickinterface.h"

class PosLabel;
class FX2000EnrollWidget;
class UserModule;
class QTimer;

class FX2000ConfigWidget :
        public QWidget,
        virtual public MemStickInterface,
        private Ui::FX2000ConfigWidgetBase

{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.FingerPrintReader")

public:
    static const QString DBusObjectPath;

    FX2000ConfigWidget(UserModule *model, QWidget *parent=0, const QString& name = QString());
    ~FX2000ConfigWidget() = default;

public slots:
    virtual void operationResultSlot(bool status) override; //ASYNC

protected slots:
    void redToggle(bool on);
    void greenToggle(bool on);
    void internalToggle(bool on);
    void stateToggle(int id);

    void qualityChanged(int quality);
    void similarityChanged(int similarity);
    void presenceChanged(int presence);

    void readStatus();
    void readRed();
    void readGreen();
    void readInternal();
    void readQuality();
    void readSimilarity();
    void readPresence();
    void readState();

    void initFX2000Slot();
    void lightAdjustSlot();
    void testSlot();

    void importFromStickButton();
    void exportToStickButton();
    void okButtonClicked();

    void startTimer();
    void timerSlot();
    void stopTimer();

    bool checkStickModule();

    void idChanged(const QString&);
    void deviceClickedAt(QString name);

protected:
    void showEvent(QShowEvent *e);
    void connectAll();
    void disconnectAll();

    virtual bool isFingerPrintModuleAvailable() const;
    virtual bool isMemstickModuleAvailable() const;
    virtual void connectDBusSignal() const;
    virtual bool checkFingerPrint(const QString& cmd);
    virtual bool saveFingerPrintToStick();
    virtual bool getFingerPrintFromStick();

private:
    void checkLeds(const QString& cmd, QCheckBox* button = nullptr);
    void checkSensibility(const QString& cmd, QSlider* sld, QProgressBar* pb, uint coef = 100);
private:
    UserModule *model{};
    PosLabel *device_label{};
    FX2000EnrollWidget *enroll_widget{};
    QTimer *timer;

    int pos;
};

#endif
