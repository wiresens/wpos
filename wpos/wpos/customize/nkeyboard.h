/***************************************************************************
                          nkeyboard.h  -  description
                             -------------------
    begin                : lun jun 7 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NKEYBOARD_H
#define NKEYBOARD_H

#include "ui_nkeyboardwidget.h"
#include <QWidget>
#include <QObject>
#include <wposcore/hlist.h>
#include <QPushButton>
#include <QStringList>
#include <QRegExp>
#include <QMenu>

class XmlConfig;
class QButtonGroup;
class QRegExp;
class OptionNode;
class QSignalMapper;

struct LoungeData {
    LoungeData(){}
    ~LoungeData(){
        table_buttons->clear();
        if (option_nodes){
            option_nodes->clear();
            delete option_nodes;
        }
        delete table_buttons;
        delete button;
        button_group = 0;
    }

    QPushButton *button{};
    HList<QPushButton> *table_buttons{ new HList<QPushButton> };
    HList<OptionNode> *option_nodes{};
    QButtonGroup *button_group{};
    QRegExp regex;
    QString name;
    uint stack_index{};
    uint tables{};
};

class NKeyboard : public QMenu , private Ui::NKeyboardWidget  {
    Q_OBJECT
public:
    explicit NKeyboard(QWidget *parent=0,
                       const QString& name = QString());

    explicit NKeyboard(const QString& text,
                       QWidget *parent=0,
                       const QString& name = QString());

    ~NKeyboard();

    void init();
    bool isAtTable();
    QString getLounge();
    int getTable();
    HList<OptionNode>* getOptionsFromLounge(const QString& lounge);

public slots:
    void setSendEvents(bool on);
    void setExitWithEnter(bool on);
    void clearSlot();
    void loungeButtonClicked();
    void kbButtonClicked();
    void setText(const QString& text);

    void clearAllStacks(bool clear_enabled = false);
    void clearLoungeStack(const QString &name, bool clear_enabled=false);

    void setUsedTables(QStringList list);
    void setTableButtonUsed(const QString& name);
    void setTableButtonSelected(const QString& name);

protected slots:
    void keyClicked(int num);
    void exitSlot();

    void loungeButtonClicked(bool checked);
    void tableButtonClicked(bool checked);
    void tableButtonClicked(QAbstractButton* button);
    void switchLounge(QAbstractButton* button);

signals:
    void exitClicked();
    void textChanged(const QString& text);
    void enterPressed(const QString &text);
    void loungeSignal(bool on);

protected:
    void loadLounges(XmlConfig *xml);

    HList<QPushButton> *keyboard_buttons{};
    HList<LoungeData> *lounges{};

    QSignalMapper *signal_mapper{};
    QButtonGroup *lounge_buttongroup{};
    QObject *o_parent{};

    const QString state;
    bool has_send_events{false};
    bool caps_lock{false};
    bool exit_with_enter{false};
};

#endif
