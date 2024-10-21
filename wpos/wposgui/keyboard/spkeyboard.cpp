/***************************************************************************
                          spkeyboard.cpp  -  description
                             -------------------
    begin                : dom feb 8 2004
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ui_spkeyboardwidget.h"
#include "spkeyboard.h"

#include <QSignalMapper>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QString>

namespace KeyBoard{
struct Key
{
    QPushButton *key;
    const int val;
};
}

static constexpr int ESC        {4096};
static constexpr int ESC_ASCII  {27};
static constexpr int F1         {4144};
static constexpr int F2         {4145};
static constexpr int F3         {4146};
static constexpr int F4         {4147};
static constexpr int F5         {4148};
static constexpr int F6         {4149};
static constexpr int F7         {4150};
static constexpr int F8         {4151};
static constexpr int F9         {4152};
static constexpr int F10        {4153};
static constexpr int F11        {4154};
static constexpr int F12        {4155};
static constexpr int OA         {65535};
static constexpr int OA_ASCII   {186};
static constexpr int B1         {49};
static constexpr int B2         {50};
static constexpr int B3         {51};
static constexpr int B4         {52};
static constexpr int B5         {53};
static constexpr int B6         {54};
static constexpr int B7         {55};
static constexpr int B8         {56};
static constexpr int B9         {57};
static constexpr int B0         {48};
static constexpr int BINT       {39};
static constexpr int DEL        {4099};
static constexpr int ENTER      {4100};
static constexpr int INTRO      {4101};
static constexpr int BQ         {81};
static constexpr int BW         {87};
static constexpr int BE         {69};
static constexpr int BR         {82};
static constexpr int BT         {84};
static constexpr int BY         {89};
static constexpr int BU         {85};
static constexpr int BI         {73};
static constexpr int BO         {79};
static constexpr int BP         {80};
static constexpr int MAY        {44132};
static constexpr int BA         {65};
static constexpr int BS         {83};
static constexpr int BD         {68};
static constexpr int BF         {70};
static constexpr int BG         {71};
static constexpr int BH         {72};
static constexpr int BJ         {74};
static constexpr int BK         {75};
static constexpr int BL         {76};
static constexpr int BEN        {65535};
static constexpr int BEN_ASCII  {241};
static constexpr int SHIFT      {4128};
static constexpr int CTRL       {4129};
static constexpr int SPACE      {32};
static constexpr int ALT        {4135};
static constexpr int BLOQ       {4133};
static constexpr int BZ         {90};
static constexpr int BX         {88};
static constexpr int BC         {67};
static constexpr int BV         {86};
static constexpr int BB         {66};
static constexpr int BN         {78};
static constexpr int BM         {77};
static constexpr int BCUT       {44};
static constexpr int BDOT       {46};
static constexpr int MIN        {45};
static constexpr int BDIV       {47};
static constexpr int BPROD      {42};
static constexpr int BRPLUS     {43};
static constexpr int BMSG       {60};
static constexpr int BTAB       {0};

SpKeyboard::SpKeyboard(QWidget* parent)
    :QWidget(parent),
    ui{ new Ui::SpKeyboardWidget},
    m_obj_parent{parent}
{
    ui->setupUi(this);
    QSignalMapper *sgmpr = new QSignalMapper(this);
    setFocusPolicy(Qt::NoFocus);

    build(sgmpr, m_keys);
}

SpKeyboard::~SpKeyboard(){
    delete ui;
}

void SpKeyboard::build(
    QSignalMapper* sgmpr,
    HList<QPushButton> &keys)
{
    QVector<KeyBoard::Key> key_map{
        {ui->esc,       ESC},
        {ui->f1,        F1},
        {ui->f2,        F2},
        {ui->f3,        F3},
        {ui->f4,        F4},
        {ui->f5,        F5},
        {ui->f6,        F6},
        {ui->f7,        F7},
        {ui->f8,        F8},
        {ui->f9,        F9},
        {ui->f10,       F10},
        {ui->f11,       F11},
        {ui->f12,       F12},
        {ui->oa,        OA_ASCII},
        {ui->b1,        B1},
        {ui->b2,        B2},
        {ui->b3,        B3},
        {ui->b4,        B4},
        {ui->b5,        B5},
        {ui->b6,        B6},
        {ui->b7,        B7},
        {ui->b8,        B8},
        {ui->b9,        B9},
        {ui->b0,        B0},
        {ui->bint,      BINT},
        {ui->bdel,      DEL},
        {ui->btab,      BTAB},
        {ui->bq,        BQ},
        {ui->bw,        BW},
        {ui->be,        BE},
        {ui->br,        BR},
        {ui->bt,        BT},
        {ui->by,        BY},
        {ui->bu,        BU},
        {ui->bi,        BI},
        {ui->bo,        BO},
        {ui->bp,        BP},
        {ui->benter,    ENTER},
        {ui->bmay,      MAY},
        {ui->ba,        BA},
        {ui->bs,        BS},
        {ui->bd,        BD},
        {ui->bf,        BF},
        {ui->bg,        BG},
        {ui->bh,        BH},
        {ui->bj,        BJ},
        {ui->bk,        BK},
        {ui->bl,        BL},
        {ui->ben,       BEN_ASCII},
        {ui->blsh,      SHIFT},
        {ui->bmsg,      BMSG},
        {ui->bz,        BZ},
        {ui->bx,        BX},
        {ui->bc,        BC},
        {ui->bv,        BV},
        {ui->bb,        BB},
        {ui->bn,        BN},
        {ui->bm,        BM},
        {ui->bcut,      BCUT},
        {ui->bdot,      BDOT},
        {ui->blmi,      MIN},
        {ui->brsh,      SHIFT},
        {ui->blctrl,    CTRL},
        {ui->blalt,     ALT},
        {ui->bspace,    SPACE},
        {ui->bralt,     ALT},
        {ui->brctrl,    CTRL},
        {ui->bbloq,     BLOQ},
        {ui->bdiv,      BDIV},
        {ui->bprod,     BPROD},
        {ui->brmin,     MIN},
        {ui->brplus,    BRPLUS},
        {ui->brenter,   INTRO},
        {ui->brdot,     BDOT},
        {ui->br0,       B0},
        {ui->br1,       B1},
        {ui->br2,       B2},
        {ui->br3,       B3},
        {ui->br4,       B4},
        {ui->br5,       B5},
        {ui->br6,       B6},
        {ui->br7,       B7},
        {ui->br8,       B8},
        {ui->br9,       B9},
    };

    ui->output_line->setFocus();
    for( auto rec : key_map){
        // keys.append(rec.key, QString::number(rec.val));
        sgmpr->setMapping(rec.key, rec.val);
        rec.key->setFocusProxy(this);
        rec.key->setFocusPolicy(Qt::NoFocus);
        connect(rec.key, &QPushButton::released,
                sgmpr, QOverload<>::of(&QSignalMapper::map));
    }
    disconnect(ui->esc, &QPushButton::released,
               sgmpr, QOverload<>::of(&QSignalMapper::map));

    connect(sgmpr, &QSignalMapper::mappedInt, this, &SpKeyboard::keyClicked);
    connect(ui->exit_button, &QPushButton::released, this, &SpKeyboard::exitSlot);
    connect(ui->clear_button, &QPushButton::released, this, &SpKeyboard::clearSlot);
    connect(ui->output_line,  &QLineEdit::textChanged, this, &SpKeyboard::textChanged);
    connect(ui->exit_button,  &QPushButton::released, this, &SpKeyboard::exitClicked);
}

void SpKeyboard::setExitWithEnter(bool on){
    m_exit_with_enter = on;
}

void SpKeyboard::exitSlot(){
    //exits produce an enter event.
    if (m_exit_with_enter)
        emit enterPressed(ui->output_line->text());
    else
        emit exitClicked();
}

void SpKeyboard::clearSlot(){
    ui->output_line->clear();
}

void SpKeyboard::sendEvents(bool on){
    m_has_send_events = on;
}

void SpKeyboard::keyClicked(int num){
    QString input_text;
    //at now it does not support event passing
    switch (num){
    case ESC:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep {nullptr};
            QKeyEvent *ker {nullptr};
            kep = new QKeyEvent(QEvent::KeyPress,   ESC, Qt::MetaModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, ESC, Qt::MetaModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        exitSlot();
        break;
    case F1:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep { nullptr};
            QKeyEvent *ker {nullptr};
            kep = new QKeyEvent(QEvent::KeyPress,   F1, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F1, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F2:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep {nullptr};
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F2, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F2, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F3:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F3, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F3, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F4:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F4, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F4, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F5:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F5, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F5, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F6:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F6, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F6, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F7:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F7, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F7, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F8:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F8, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F8, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F9:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F9, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F9, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F10 :
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F10, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F10, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F11:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F11, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F11, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F12:
        if ( m_has_send_events && m_obj_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,   F12, Qt::NoModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F12, Qt::NoModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case OA_ASCII :
        break;
    case B1:
        ui->output_line->setText(ui->output_line->text()+"1");
        break;
    case B2:
        ui->output_line->setText(ui->output_line->text()+"2");
        break;
    case B3:
        ui->output_line->setText(ui->output_line->text()+"3");
        break;
    case B4:
        ui->output_line->setText(ui->output_line->text()+"4");
        break;
    case B5:
        ui->output_line->setText(ui->output_line->text()+"5");
        break;
    case B6:
        ui->output_line->setText(ui->output_line->text()+"6");
        break;
    case B7:
        ui->output_line->setText(ui->output_line->text()+"7");
        break;
    case B8:
        ui->output_line->setText(ui->output_line->text()+"8");
        break;
    case B9:
        ui->output_line->setText(ui->output_line->text()+"9");
        break;
    case B0:
        ui->output_line->setText(ui->output_line->text()+"0");
        break;
    case BINT:
        ui->output_line->setText(ui->output_line->text()+"?");
        break;
    case DEL :
        input_text = ui->output_line->text();
        input_text.truncate(input_text.length() -1 );
        ui->output_line->setText(input_text);
        break;
    case ENTER :
        emit enterPressed(ui->output_line->text());
        break;
    case  INTRO:
        emit enterPressed(ui->output_line->text());
        break;
    case BQ:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"Q");
        else
            ui->output_line->setText(ui->output_line->text()+"q");
        break;
    case BW:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"W");
        else
            ui->output_line->setText(ui->output_line->text()+"w");
        break;
    case BE :
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"E");
        else
            ui->output_line->setText(ui->output_line->text()+"e");
        break;
    case BR :
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"R");
        else
            ui->output_line->setText(ui->output_line->text()+"r");
        break;
    case BT:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"T");
        else
            ui->output_line->setText(ui->output_line->text()+"t");
        break;
    case BY:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"Y");
        else
            ui->output_line->setText(ui->output_line->text()+"y");
        break;
    case BU:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"U");
        else
            ui->output_line->setText(ui->output_line->text()+"u");
        break;
    case BI:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"I");
        else
            ui->output_line->setText(ui->output_line->text()+"i");
        break;
    case BO :
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"O");
        else
            ui->output_line->setText(ui->output_line->text()+"o");
        break;
    case BP:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"P");
        else
            ui->output_line->setText(ui->output_line->text()+"p");
        break;
    case MAY :
        m_caps_lock = !m_caps_lock;
        break;
    case BA :
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"A");
        else
            ui->output_line->setText(ui->output_line->text()+"a");
        break;
    case BS:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"S");
        else
            ui->output_line->setText(ui->output_line->text()+"s");
        break;
    case BD:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"D");
        else
            ui->output_line->setText(ui->output_line->text()+"d");
        break;
    case BF:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"F");
        else
            ui->output_line->setText(ui->output_line->text()+"f");
        break;
    case BG:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"G");
        else
            ui->output_line->setText(ui->output_line->text()+"g");
        break;
    case BH:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"H");
        else
            ui->output_line->setText(ui->output_line->text()+"h");
        break;
    case BJ:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"J");
        else
            ui->output_line->setText(ui->output_line->text()+"j");
        break;
    case BK:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"K");
        else
            ui->output_line->setText(ui->output_line->text()+"k");
        break;
    case BL:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"L");
        else
            ui->output_line->setText(ui->output_line->text()+"l");
        break;
    case SHIFT :
        break;
    case CTRL :
        break;
    case SPACE :
        ui->output_line->setText(ui->output_line->text()+" ");
        break;
    case ALT :
        break;
    case BLOQ :
        break;
    case BZ :
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"Z");
        else
            ui->output_line->setText(ui->output_line->text()+"z");
        break;
    case BX:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"X");
        else
            ui->output_line->setText(ui->output_line->text()+"x");
        break;
    case BC:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"C");
        else
            ui->output_line->setText(ui->output_line->text()+"c");
        break;
    case BV:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"V");
        else
            ui->output_line->setText(ui->output_line->text()+"v");
        break;
    case BB:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"B");
        else
            ui->output_line->setText(ui->output_line->text()+"b");
        break;
    case BN:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"N");
        else
            ui->output_line->setText(ui->output_line->text()+"n");
        break;
    case BM:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"M");
        else
            ui->output_line->setText(ui->output_line->text()+"m");
        break;
    case BCUT :
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+";");
        else
            ui->output_line->setText(ui->output_line->text()+",");
        break;
    case BDOT:
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+":");
        else
            ui->output_line->setText(ui->output_line->text()+".");
        break;
    case MIN :
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+"_");
        else
            ui->output_line->setText(ui->output_line->text()+"-");
        break;
    case BDIV :
        ui->output_line->setText(ui->output_line->text()+"/");
        break;
    case BPROD :
        ui->output_line->setText(ui->output_line->text()+"*");
        break;
    case BRPLUS :
        ui->output_line->setText(ui->output_line->text()+"+");
        break;
    case BMSG :
        if (m_caps_lock)
            ui->output_line->setText(ui->output_line->text()+">");
        else
            ui->output_line->setText(ui->output_line->text()+"<");
        break;
    case BTAB:
        ui->output_line->setText(ui->output_line->text()+"\t");
        break;
    case 65535:
        break;
    }

}
