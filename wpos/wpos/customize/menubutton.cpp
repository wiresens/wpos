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
#include <wposgui/common/global.h>
#include <wposcore/genericsignalmanager.h>

#include <QMenu>
#include <QFrame>
#include <QChildEvent>
#include <QLayout>
#include <QButtonGroup>
#include <QColor>

#include <QMetaObject>

static const int MAX_ALIGNED_BUTTONS {7};

MenuButton::MenuButton(
    Qt::Orientation orientation,
    QWidget* parent,
    const QString& name):
    QPushButton(parent),
    m_frame{ new QFrame() },
    m_grid_layout  {new QGridLayout(m_frame)},
    m_button_group { new QButtonGroup},
    m_popup_orientation{orientation}
{
    setObjectName(name);
    setText(name);
    setCheckable(true);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericSignal(GSIGNAL::DISABLE_MAINSTACK, this);
    gsm->publishGenericSignal(GSIGNAL::ENABLE_MAINSTACK, this);

    m_frame->setWindowFlags(Qt::Popup);
    m_frame->setBackgroundRole(QPalette::Window);
    m_frame->setPalette(QPalette(QColor(Colors::SPECIAL_BUTTON_POPUP_BG_COLOR)));

    // QLayout* mf_layout = m_frame->layout();
    // if( mf_layout){
    //     mf_layout->deleteLater();
    //     qDebug() << "@benes Attempting to add QLayout to QFrame" ;
    //     if (m_popup_orientation == Qt::Horizontal)
    //         m_frame_layout = new QHBoxLayout(m_frame);
    //     else
    //         m_frame_layout = new QVBoxLayout(m_frame);
    // }@benes

    m_frame->installEventFilter(this);
    installEventFilter(this);

    connect(this, &MenuButton::clicked, this, &MenuButton::popUp);
    connect(m_button_group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonPressed),
        this, &MenuButton::toggleMenuButtonColor);
}

MenuButton::~MenuButton(){
    delete m_frame;
    delete m_button_group;
}

// void MenuButton::setOrientation(Qt::Orientation orientation)
// {
//     if (m_popup_orientation == orientation) return;

//     m_popup_orientation = orientation;

//     if ( m_popup_orientation == Qt::Horizontal &&
//         m_buttons.count() > MAX_ALIGNED_BUTTONS )
//     {
//         alignInHGrid();
//         return;
//     }

//     delete m_frame_layout;
//     if (m_popup_orientation == Qt::Horizontal)
//         m_frame_layout = new QHBoxLayout(m_frame);
//     else
//         m_frame_layout = new QVBoxLayout(m_frame);

//     m_frame_layout->setContentsMargins(2,2,2,2);

//     for (auto button : m_buttons){
//         m_frame_layout->addWidget(button);
//         button->show();
//     }
// }

// void MenuButton::alignInHGrid(){

//     if (m_popup_orientation != Qt::Horizontal) return;

//     delete m_frame_layout;
//     if(m_grid_layout){
//         delete m_grid_layout;
//         m_grid_layout = new QGridLayout(m_frame);
//     }

//     auto cols = MAX_ALIGNED_BUTTONS;
//     if ( m_buttons.count() < MAX_ALIGNED_BUTTONS)
//         cols = m_buttons.count();

//     auto i = 0;
//     for(auto* button : m_buttons){
//         auto row = i / cols ;
//         auto col = i % cols;
//         m_grid_layout->addWidget(button, row, col);
//         button->show();
//         i++;
//     }

// }

//void MenuButton::alignInHGrid(){
//    auto total = grid_layout->count();
//    grid_layout->addWidget(button, total / 2, total % 2);
//}

void MenuButton::addMenuButton(
    QAbstractButton* button,
    QGridLayout *layout )
{
    //We assume a 4x2 GridLayout. We will change that in the future
    // to support dynamically sized layouts
    auto n = layout->count();
    button->setCheckable(true);
    layout->addWidget(button, n / 2, n % 2); // Add row by row
    //    layout->addWidget(button, n % 4, n / 4); //Add column by column
    m_button_group->addButton(button);
}

Qt::Orientation MenuButton::orientation(){
    return m_popup_orientation;
}

void MenuButton::popUp(){
    auto base = parentWidget()->parentWidget();
    auto dx = base->mapToGlobal(QPoint(0,0)).x();
    auto dy = base->mapToGlobal(QPoint(0,0)).y();
    m_frame->setGeometry( dx, dy, base->width(), base->height());
    m_frame->show();
}

void MenuButton::popDown(){
    setDown(true);
    for ( auto button : m_buttons ){
        if ( button->isCheckable() && !button->isDown())
            button->toggle();
    }
    m_frame->close();
    emit popupClosed();
}

void MenuButton::toggleMenuButtonColor(QAbstractButton* button){

    button->setBackgroundRole(QPalette::Button);
    for( auto others : m_button_group->buttons())
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
        m_buttons.remove(QString::number((ulong)button));
        break;

    default:
        //        if ( ! button->inherits("MenuButton") )
        //            connect( button, &QAbstractButton::released, this, &MenuButton::closePopUp);
        //        else
        //            connect( qobject_cast<MenuButton*>(button), &MenuButton::closePopUpSignal,
        //                     this, &MenuButton::closePopUp);

        m_buttons.append( button, QString::number( (ulong)button) );
        addMenuButton(button, m_grid_layout);
        break;
    }
    QPushButton::childEvent(event);
}

//used to monitor the events of the m_frame (only the show and hide or close event)
bool MenuButton::eventFilter(QObject *watched, QEvent *event){

    QObject *tmp_qobject { qobject_cast<QObject*>( parentWidget() ) };
    if ( watched == m_frame ){
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
            m_frame->hide();
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
    for (auto button : m_buttons)
        button->setEnabled(enabled);
    QWidget::setEnabled(enabled);
}
