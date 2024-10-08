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

#include <wposcore/hlist.h>

#include <QMenu>
#include <QRegExp>
#include <QPushButton>

class ProductExtraInfo;
class LoungeData;
class XmlConfig;

class QSignalMapper;
class QButtonGroup;

namespace Ui {
class NKeyboardWidget;
}

class NKeyboard :
    public QMenu
{
    Q_OBJECT

public:
    explicit NKeyboard(
        QWidget *parent=nullptr,
        const QString& name = QString{}
    );

    ~NKeyboard();

    bool isAtTable();
    QString getLounge();
    int getTable();

    HList<ProductExtraInfo>*
    getOptionsFromLounge(const QString& lounge);

signals:
    void exitClicked();
    void textChanged(const QString& text);
    void enterPressed(const QString& text);
    void loungeRequested(bool on);

public slots:
    void setSendEvents(bool on);
    void setExitWithEnter(bool on);
    void clearSlot();
    void showLounge();
    void showKeyBoard();
    void setText(const QString& text);

    void clearAllStacks(bool clear_enabled = false);
    void clearLoungeStack(
        const QString &name,
        bool clear_enabled=false
    );

    void setUsedTables(QStringList list);
    void setTableButtonUsed(const QString& name);
    void setTableButtonSelected(const QString& name);

protected slots:
    void keyClicked(int key);
    void exitSlot();

    void loungeButtonClicked(bool checked);
    void tableButtonClicked(QAbstractButton* button);
    void switchLounge(QAbstractButton* button);

protected:
    void loadLounges(XmlConfig& xml);

private:
    void build(QSignalMapper* sgmpr);
    void write(const QString& text);
    QString read() const;

private:
    Ui::NKeyboardWidget* ui;
    QObject             *m_obj_parent{};
    HList<LoungeData>   m_lounges{};
    QButtonGroup        *m_lounge_btn_group{};

    bool  m_caps_lock{false};
    bool  m_has_send_events{false};
    bool  m_exit_with_enter{false};
};

#endif
