/***************************************************************************
                          ndeloptionwidget.h  -  description
                             -------------------
    begin                : Wed  28 Apr 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef OPTION_DELETION_WIDGET_H
#define OPTION_DELETION_WIDGET_H

#include "optioneditionwidget.h"
#include <QObject>

class OptionDeletionWidget : public OptionEditionWidget {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.OptionDeletionModel")

public:
    static const QString DBusObjectPath;

    explicit OptionDeletionWidget(QWidget* parent = 0, const QString& name = QString {});
    ~OptionDeletionWidget() = default;

signals:
    Q_SCRIPTABLE void optionChanged(); // dbus_signals

protected slots:
    virtual void acceptSlot() override;
    virtual void notify() override;

    void optionClickedSlot();
    void delTypeSlot(); // processType()
    bool delOption(const QString& type, const QString& option); // processOption()

protected:
    virtual void showEvent(QShowEvent* e) override;
    virtual void getTypes() override;
    virtual void getOptionTypes(const QString& type) override;
};

#endif // OPTION_DELETION_WIDGET_H
