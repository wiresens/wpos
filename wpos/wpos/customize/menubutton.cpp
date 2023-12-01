/***************************************************************************
                          bslmenubutton.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "menubutton.h"
#include "genericbutton.h"
#include <wposwidget/global.h>
#include <wposcore/genericsignalmanager.h>

#include <QMenu>
#include <QFrame>
#include <QChildEvent>
#include <QLayout>
#include <QButtonGroup>
#include <QColor>

#include <QMetaObject>
#include <iostream>

using namespace std;

#define HEIGHT 80
#define MAX_BUTTONS_ALIGNED 7

MenuButton::MenuButton(
    Qt::Orientation orientation,
    QWidget* parent,
    const QString& name):
    QPushButton(parent),
    frame{ new QFrame() },
    grid_layout  {new QGridLayout(frame)},
    button_group { new QButtonGroup},
    popup_orientation{orientation}
{
    setObjectName(name);
    setText(name);
    setCheckable(true);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericSignal(GSIGNAL::DISABLE_MAINSTACK, this);
    gsm->publishGenericSignal(GSIGNAL::ENABLE_MAINSTACK, this);

    frame->setWindowFlags(Qt::Popup);
    frame->setBackgroundRole(QPalette::Window);
    frame->setPalette(QPalette(QColor(Colors::SPECIAL_BUTTON_POPUP_BG_COLOR)));

    if (popup_orientation == Qt::Horizontal)
        frame_layout = new QHBoxLayout(frame);
    else
        frame_layout = new QVBoxLayout(frame);

    frame->installEventFilter(this);
    installEventFilter(this);

    connect(this, &MenuButton::clicked, this, &MenuButton::openPopUp);
    connect(button_group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonPressed), this, &MenuButton::toggleMenuButtonColor);
}

MenuButton::~MenuButton(){
    delete frame;
    delete button_group;
}

void MenuButton::setOrientation(Qt::Orientation orientation)
{
    if (popup_orientation == orientation) return;

    popup_orientation = orientation;
    delete frame_layout;

    if ( popup_orientation == Qt::Horizontal &&
        buttons.count() > MAX_BUTTONS_ALIGNED )
    {
        alignInHGrid();
        return;
    }

    if (popup_orientation == Qt::Horizontal)
        frame_layout = new QHBoxLayout(frame);
    else
        frame_layout = new QVBoxLayout(frame);

    frame_layout->setMargin(2);

    for (auto button : buttons){
        frame_layout->addWidget(button);
        button->show();
    }
}

void MenuButton::alignInHGrid(){

    if (popup_orientation != Qt::Horizontal) return;

    delete frame_layout;
    if(grid_layout){
        delete grid_layout;
        grid_layout = new QGridLayout(frame);
    }

    auto cols = MAX_BUTTONS_ALIGNED;
    if ( buttons.count() < MAX_BUTTONS_ALIGNED)
        cols = buttons.count();

    auto i = 0;
    for(auto button : buttons){
        auto row = (int) (i / cols ) ;
        auto col = i % cols;
        grid_layout->addWidget(button, row, col);
        button->show();
        i++;
    }

}

//void MenuButton::alignInHGrid(){
//    auto total = grid_layout->count();
//    grid_layout->addWidget(button, total / 2, total % 2);
//}

void MenuButton::addMenuButton(QAbstractButton* button, QGridLayout *layout )
{
    //We assume a 4x2 GridLayout. We will change that in the future
    // to support dynamically sized layouts
    auto n = layout->count();
    button->setCheckable(true);
    layout->addWidget(button, n / 2, n % 2); // Add row by row
    //    layout->addWidget(button, n % 4, n / 4); //Add column by column
    button_group->addButton(button);
}

Qt::Orientation MenuButton::orientation(){
    return popup_orientation;
}

void MenuButton::openPopUp(){
    auto base = parentWidget()->parentWidget();
    auto dx = base->mapToGlobal(QPoint(0,0)).x();
    auto dy = base->mapToGlobal(QPoint(0,0)).y();
    frame->setGeometry( dx, dy, base->width(), base->height());
    frame->show();
}

void MenuButton::closePopUp(){
    setDown(true);
    for ( auto button : buttons ){
        if ( button->isCheckable() && !button->isDown())
            button->toggle();
    }
    frame->close();
    emit closePopUpSignal();
}

void MenuButton::toggleMenuButtonColor(QAbstractButton* button){

    button->setBackgroundRole(QPalette::Button);
    for( auto others : button_group->buttons())
        if(others != button) others->setPalette(button->palette());
    button->setPalette(QPalette(QColor(Colors::MONEY_BUTTON_DWN_COLOR)));
}

void MenuButton::childEvent(QChildEvent *event){
    QAbstractButton *button {};
    if ( event->child()->inherits("QAbstractButton") )
        button = qobject_cast<QAbstractButton*>( event->child() );

    if( ! button ) return QPushButton::childEvent(event);

    switch( event->type() ){

    case QChildEvent::ChildRemoved :
        buttons.remove(QString::number((ulong)button));
        break;

    default:
        //        if ( ! button->inherits("MenuButton") )
        //            connect( button, &QAbstractButton::released, this, &MenuButton::closePopUp);
        //        else
        //            connect( qobject_cast<MenuButton*>(button), &MenuButton::closePopUpSignal,
        //                     this, &MenuButton::closePopUp);

        buttons.append( button, QString::number( (ulong)button) );
        addMenuButton(button, grid_layout);
        break;
    }
    QPushButton::childEvent(event);
}

//used to monitor the events of the frame (only the show and hide or close event)
bool MenuButton::eventFilter(QObject *watched, QEvent *event){

    QObject *tmp_qobject { qobject_cast<QObject*>( parentWidget() ) };
    if ( watched == frame ){
        switch ( event->type() ) {

        case QEvent::Show:
            if ( tmp_qobject->inherits("ButtonSetWidget") )
                emit genericSignal(GSIGNAL::DISABLE_MAINSTACK);
            break;

        case QEvent::Close:
        case QEvent::Leave:
            if (tmp_qobject->inherits("ButtonSetWidget"))
                emit genericSignal(GSIGNAL::ENABLE_MAINSTACK);
            setDown(true);
            frame->hide();
            break;

        case QEvent::Hide:
            if (tmp_qobject->inherits("ButtonSetWidget"))
                emit genericSignal(GSIGNAL::ENABLE_MAINSTACK);
            toggle();
            setDown(true);
            break;
        default: ;
        }
    } else  if ( watched == this ){
        if (event->type() == QEvent::Enter)
        {/*openPopUp()*/;}
    }

    return QPushButton::eventFilter(watched, event);
}

void MenuButton::setEnabled(bool enabled){
    for (auto button : buttons)
        button->setEnabled(enabled);
    QWidget::setEnabled(enabled);
}
