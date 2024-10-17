/***************************************************************************
                          bslmenubutton.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QCheckBox>
#include <QPushButton>
#include <wposcore/hlist.h>

class QMenu;
class QFrame;
class QBoxLayout;
class QGridLayout;
class GenericMenuButton;

class MenuButton : public QPushButton{
    Q_OBJECT

public:
    MenuButton(
        Qt::Orientation orientation,
        QWidget* parent,
        const QString& name);

    ~MenuButton();

    // void setOrientation(Qt::Orientation orientation);
    Qt::Orientation orientation();

    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void genericSignal(const QString& signal_name);
    void popupClosed();

public slots:
    void popUp();
    void popDown();
    void toggleMenuButtonColor(QAbstractButton* button);

protected:
    // void alignInHGrid();
    void addMenuButton(QAbstractButton* button, QGridLayout *layout );
    void childEvent(QChildEvent *event) override;
    void setEnabled(bool enabled);

    QFrame                  *m_frame{};
    QBoxLayout              *m_frame_layout{};
    QGridLayout             *m_grid_layout{};
    QButtonGroup            *m_button_group{};
    HList<QAbstractButton>  m_buttons;
    Qt::Orientation         m_popup_orientation{};
};

#endif
