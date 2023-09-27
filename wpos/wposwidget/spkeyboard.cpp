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

#include "spkeyboard.h"

#include <QSignalMapper>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QString>

#include <iostream>
using namespace std;

//        f1->
//        f2->
//        f3->
//        f4->
//        f5->
//        f6->
//        f7->
//        f8->
//        f9->
//        f10->
//        f11->
//        f12->
//        oa->
//        b1->
//        b2->
//        b3->
//        b4->
//        b5->
//        b6->
//        b7->
//        b8->
//        b9->
//        b0->
//        bint->
//        bdel->
//        btab->
//        bq->
//        bw->
//        be->
//        br->
//        bt->
//        by->
//        bu->
//        bi->
//        bo->
//        bp->
//        benter->
//        bmay->
//        ba->
//        bs->
//        bd->
//        bf->
//        bg->
//        bh->
//        bj->
//        bk->
//        bl->
//        ben->
//        blsh->
//        bmsg->
//        bz->
//        bx->
//        bc->
//        bv->
//        bb->
//        bn->
//        bm->
//        bcut->
//        bdot->
//        blmi->
//        brsh->
//        blctrl->
//        blalt->
//        bspace->
//        bralt->
//        brctrl->
//        bbloq->
//        bdiv->
//        bprod->
//        brmin->
//        brplus->
//        brenter->
//        brdot->
//        br0->
//        br1->
//        br2->
//        br3->
//        br4->
//        br5->
//        br6->
//        br7->
//        br8->
//        br9->



#define ESC 4096
#define ESC_ASCII 27
#define F1 4144
#define F2 4145
#define F3 4146
#define F4 4147
#define F5 4148
#define F6 4149
#define F7 4150
#define F8 4151
#define F9 4152
#define F10 4153
#define F11 4154
#define F12 4155
#define OA 65535
#define OA_ASCII 186
#define B1 49
#define B2 50
#define B3 51
#define B4 52
#define B5 53
#define B6 54
#define B7 55
#define B8 56
#define B9 57
#define B0 48
#define BINT 39
#define DEL 4099
#define ENTER 4100
#define INTRO 4101
#define BQ 81
#define BW 87
#define BE 69
#define BR 82
#define BT 84
#define BY 89
#define BU 85
#define BI 73
#define BO 79
#define BP 80
#define MAY 44132
#define BA 65
#define BS 83
#define BD 68
#define BF 70
#define BG 71
#define BH 72
#define BJ 74
#define BK 75
#define BL 76
#define BEN 65535
#define BEN_ASCII 241
#define SHIFT 4128
#define CTRL 4129
#define SPACE 32
#define ALT 4135
#define BLOQ 4133
#define BZ 90
#define BX 88
#define BC 67
#define BV 86
#define BB 66
#define BN 78
#define BM 77
#define BCUT 44
#define BDOT 46
#define MIN 45
#define BDIV 47
#define BPROD 42
#define BRPLUS 43
#define BMSG 60
#define BTAB 0

SpKeyboard::SpKeyboard(QWidget* parent, const QString& name ): QWidget(parent), o_parent{parent}{
    setupUi(this);
    setObjectName(name);
    init();
}

SpKeyboard::~SpKeyboard(){
    delete signal_mapper;
}

void SpKeyboard::init(){
    signal_mapper = new QSignalMapper(this);
    signal_mapper->setObjectName("kb_mapper");
    button_list = new HList<QPushButton>;

    setFocusPolicy(Qt::NoFocus);

    button_list->append(esc,QString::number(ESC));
    button_list->append(f1,QString::number(F1));
    button_list->append(f2,QString::number(F2));
    button_list->append(f3,QString::number(F3));
    button_list->append(f4,QString::number(F4));
    button_list->append(f5,QString::number(F5));
    button_list->append(f6,QString::number(F6));
    button_list->append(f7,QString::number(F7));
    button_list->append(f8,QString::number(F8));
    button_list->append(f9,QString::number(F9));
    button_list->append(f10,QString::number(F10));
    button_list->append(f11,QString::number(F11));
    button_list->append(f12,QString::number(F12));
    button_list->append(oa,QString::number(OA_ASCII));
    button_list->append(b1,QString::number(B1));
    button_list->append(b2,QString::number(B2));
    button_list->append(b3,QString::number(B3));
    button_list->append(b4,QString::number(B4));
    button_list->append(b5,QString::number(B5));
    button_list->append(b6,QString::number(B6));
    button_list->append(b7,QString::number(B7));
    button_list->append(b8,QString::number(B8));
    button_list->append(b9,QString::number(B9));
    button_list->append(b0,QString::number(B0));
    button_list->append(bint,QString::number(BINT));
    button_list->append(bdel,QString::number(DEL));
    button_list->append(btab,QString::number(BTAB));
    button_list->append(bq,QString::number(BQ));
    button_list->append(bw,QString::number(BW));
    button_list->append(be,QString::number(BE));
    button_list->append(br,QString::number(BR));
    button_list->append(bt,QString::number(BT));
    button_list->append(by,QString::number(BY));
    button_list->append(bu,QString::number(BU));
    button_list->append(bi,QString::number(BI));
    button_list->append(bo,QString::number(BO));
    button_list->append(bp,QString::number(BP));
    button_list->append(benter,QString::number(ENTER));
    button_list->append(bmay,QString::number(MAY));
    button_list->append(ba,QString::number(BA));
    button_list->append(bs,QString::number(BS));
    button_list->append(bd,QString::number(BD));
    button_list->append(bf,QString::number(BF));
    button_list->append(bg,QString::number(BG));
    button_list->append(bh,QString::number(BH));
    button_list->append(bj,QString::number(BJ));
    button_list->append(bk,QString::number(BK));
    button_list->append(bl,QString::number(BL));
    button_list->append(ben,QString::number(BEN_ASCII));
    button_list->append(blsh,QString::number(SHIFT));
    button_list->append(bmsg,QString::number(BMSG));
    button_list->append(bz,QString::number(BZ));
    button_list->append(bx,QString::number(BX));
    button_list->append(bc,QString::number(BC));
    button_list->append(bv,QString::number(BV));
    button_list->append(bb,QString::number(BB));
    button_list->append(bn,QString::number(BN));
    button_list->append(bm,QString::number(BM));
    button_list->append(bcut,QString::number(BCUT));
    button_list->append(bdot,QString::number(BDOT));
    button_list->append(blmi,QString::number(MIN));
    button_list->append(brsh,QString::number(SHIFT));
    button_list->append(blctrl,QString::number(CTRL));
    button_list->append(blalt,QString::number(ALT));
    button_list->append(bspace,QString::number(SPACE));
    button_list->append(bralt,QString::number(ALT));
    button_list->append(brctrl,QString::number(CTRL));
    button_list->append(bbloq,QString::number(BLOQ));
    button_list->append(bdiv,QString::number(BDIV));
    button_list->append(bprod,QString::number(BPROD));
    button_list->append(brmin,QString::number(MIN));
    button_list->append(brplus,QString::number(BRPLUS));
    button_list->append(brenter,QString::number(INTRO));
    button_list->append(brdot,QString::number(BDOT));
    button_list->append(br0,QString::number(B0));
    button_list->append(br1,QString::number(B1));
    button_list->append(br2,QString::number(B2));
    button_list->append(br3,QString::number(B3));
    button_list->append(br4,QString::number(B4));
    button_list->append(br5,QString::number(B5));
    button_list->append(br6,QString::number(B6));
    button_list->append(br7,QString::number(B7));
    button_list->append(br8,QString::number(B8));
    button_list->append(br9,QString::number(B9));

    signal_mapper->setMapping(esc,ESC);
    signal_mapper->setMapping(f1,F1);
    signal_mapper->setMapping(f2,F2);
    signal_mapper->setMapping(f3,F3);
    signal_mapper->setMapping(f4,F4);
    signal_mapper->setMapping(f5,F5);
    signal_mapper->setMapping(f6,F6);
    signal_mapper->setMapping(f7,F7);
    signal_mapper->setMapping(f8,F8);
    signal_mapper->setMapping(f9,F9);
    signal_mapper->setMapping(f10,F10);
    signal_mapper->setMapping(f11,F11);
    signal_mapper->setMapping(f12,F12);
    signal_mapper->setMapping(oa,OA_ASCII);
    signal_mapper->setMapping(b1,B1);
    signal_mapper->setMapping(b2,B2);
    signal_mapper->setMapping(b3,B3);
    signal_mapper->setMapping(b4,B4);
    signal_mapper->setMapping(b5,B5);
    signal_mapper->setMapping(b6,B6);
    signal_mapper->setMapping(b7,B7);
    signal_mapper->setMapping(b8,B8);
    signal_mapper->setMapping(b9,B9);
    signal_mapper->setMapping(b0,B0);
    signal_mapper->setMapping(bint,BINT);
    signal_mapper->setMapping(bdel,DEL);
    signal_mapper->setMapping(btab,BTAB);
    signal_mapper->setMapping(bq,BQ);
    signal_mapper->setMapping(bw,BW);
    signal_mapper->setMapping(be,BE);
    signal_mapper->setMapping(br,BR);
    signal_mapper->setMapping(bt,BT);
    signal_mapper->setMapping(by,BY);
    signal_mapper->setMapping(bu,BU);
    signal_mapper->setMapping(bi,BI);
    signal_mapper->setMapping(bo,BO);
    signal_mapper->setMapping(bp,BP);
    signal_mapper->setMapping(benter,ENTER);
    signal_mapper->setMapping(bmay,MAY);
    signal_mapper->setMapping(ba,BA);
    signal_mapper->setMapping(bs,BS);
    signal_mapper->setMapping(bd,BD);
    signal_mapper->setMapping(bf,BF);
    signal_mapper->setMapping(bg,BG);
    signal_mapper->setMapping(bh,BH);
    signal_mapper->setMapping(bj,BJ);
    signal_mapper->setMapping(bk,BK);
    signal_mapper->setMapping(bl,BL);
    signal_mapper->setMapping(ben,BEN_ASCII);
    signal_mapper->setMapping(blsh,SHIFT);
    signal_mapper->setMapping(bmsg,BMSG);
    signal_mapper->setMapping(bz,BZ);
    signal_mapper->setMapping(bx,BX);
    signal_mapper->setMapping(bc,BC);
    signal_mapper->setMapping(bv,BV);
    signal_mapper->setMapping(bb,BB);
    signal_mapper->setMapping(bn,BN);
    signal_mapper->setMapping(bm,BM);
    signal_mapper->setMapping(bcut,BCUT);
    signal_mapper->setMapping(bdot,BDOT);
    signal_mapper->setMapping(blmi,MIN);
    signal_mapper->setMapping(brsh,SHIFT);
    signal_mapper->setMapping(blctrl,CTRL);
    signal_mapper->setMapping(blalt,ALT);
    signal_mapper->setMapping(bspace,SPACE);
    signal_mapper->setMapping(bralt,ALT);
    signal_mapper->setMapping(brctrl,CTRL);
    signal_mapper->setMapping(bbloq,BLOQ);
    signal_mapper->setMapping(bdiv,BDIV);
    signal_mapper->setMapping(bprod,BPROD);
    signal_mapper->setMapping(brmin,MIN);
    signal_mapper->setMapping(brplus,BRPLUS);
    signal_mapper->setMapping(brenter,INTRO);
    signal_mapper->setMapping(brdot,BDOT);
    signal_mapper->setMapping(br0,B0);
    signal_mapper->setMapping(br1,B1);
    signal_mapper->setMapping(br2,B2);
    signal_mapper->setMapping(br3,B3);
    signal_mapper->setMapping(br4,B4);
    signal_mapper->setMapping(br5,B5);
    signal_mapper->setMapping(br6,B6);
    signal_mapper->setMapping(br7,B7);
    signal_mapper->setMapping(br8,B8);
    signal_mapper->setMapping(br9,B9);

    connect(f1,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f2,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f3,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f4,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f5,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f6,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f7,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f8,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f9,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f10,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f11,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(f12,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(oa,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b1,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b2,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b3,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b4,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b5,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b6,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b7,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b8,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b9,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(b0,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bint,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bdel,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(btab,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bq,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bw,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(be,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bt,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(by,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bu,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bi ,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bo,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bp,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(benter,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bmay ,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(ba,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bs ,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bd,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bf ,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bg,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bh,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bj ,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bk,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bl ,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(ben,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(blsh,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bmsg,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bz,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bx,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bc,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bv,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bb,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bn,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bm,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bcut,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bdot,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(blmi,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(brsh,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(blctrl,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(blalt ,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bspace,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bralt,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(brctrl,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bbloq,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bdiv ,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(bprod,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(brmin,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(brplus,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(brenter,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(brdot,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br0 ,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br1,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br2,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br3,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br4,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br5,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br6,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br7,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br8,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(br9,SIGNAL(released()),signal_mapper,SLOT(map()));
    connect(signal_mapper,SIGNAL(mapped(int)),this,SLOT(keyClicked(int)));
    connect(exit_button,SIGNAL(released()),this,SLOT(exitSlot()));
    connect(clear_button,SIGNAL(released()),this,SLOT(clearSlot()));
    connect(output_line,SIGNAL(textChanged(const QString &)),this,SIGNAL(textChanged(const QString &)));
    connect(exit_button,SIGNAL(released()), this,SIGNAL(exitClicked()));

    output_line->setFocus();

    int count = button_list->count();
    for(int i=0;i<count;i++){
        QPushButton* b = button_list->at(i);
        b->setFocusProxy(this);
        b->setFocusPolicy(Qt::NoFocus);
    }
}

void SpKeyboard::exitWithEnter(bool on){
    exit_with_enter = on;
}

void SpKeyboard::exitSlot(){
    //exits produce an enter event.
    if (exit_with_enter)
        emit enterPressed(output_line->text());
    else
        emit exitClicked();
}

void SpKeyboard::clearSlot(){
    output_line->clear();
}

void SpKeyboard::sendEvents(bool on){
    send_events = on;
}

void SpKeyboard::keyClicked(int num){
    QString texto;
    //at now it does not support event passing
    switch (num){
    case ESC:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep {nullptr};
            QKeyEvent *ker {nullptr};
            //                kep = new QKeyEvent(QEvent::KeyPress, ESC, ESC_ASCII, state);
            kep = new QKeyEvent(QEvent::KeyPress, ESC, Qt::MetaModifier, state);
            //                ker = new QKeyEvent(QEvent::KeyRelease, ESC, ESC_ASCII, state);
            ker = new QKeyEvent(QEvent::KeyRelease, ESC, Qt::MetaModifier, state);
            qApp->postEvent(o_parent,(QEvent*) kep);
            qApp->postEvent(o_parent,(QEvent*) ker);
        }
        exitSlot();
        break;
    case F1:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep { nullptr};
            QKeyEvent *ker {nullptr};
            kep = new QKeyEvent(QEvent::KeyPress,F1,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F1,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F2:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep {nullptr};
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F2,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F2,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F3:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F3,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F3,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F4:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F4,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F4,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F5:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F5,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F5,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F6:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F6,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F6,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F7:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F7,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F7,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F8:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F8,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F8,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F9:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F9,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F9,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F10 :
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F10,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F10,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F11:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F11,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F11,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F12:
        if ((send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F12,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F12,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case OA_ASCII :
        break;
    case B1:
        output_line->setText(output_line->text()+"1");
        break;
    case B2:
        output_line->setText(output_line->text()+"2");
        break;
    case B3:
        output_line->setText(output_line->text()+"3");
        break;
    case B4:
        output_line->setText(output_line->text()+"4");
        break;
    case B5:
        output_line->setText(output_line->text()+"5");
        break;
    case B6:
        output_line->setText(output_line->text()+"6");
        break;
    case B7:
        output_line->setText(output_line->text()+"7");
        break;
    case B8:
        output_line->setText(output_line->text()+"8");
        break;
    case B9:
        output_line->setText(output_line->text()+"9");
        break;
    case B0:
        output_line->setText(output_line->text()+"0");
        break;
    case BINT:
        output_line->setText(output_line->text()+"?");
        break;
    case DEL :
        texto = output_line->text();
        texto.truncate(texto.length() -1 );
        output_line->setText(texto);
        break;
    case ENTER :
        emit enterPressed(output_line->text());
        break;
    case  INTRO:
        emit enterPressed(output_line->text());
        break;
    case BQ:
        if (may)
            output_line->setText(output_line->text()+"Q");
        else
            output_line->setText(output_line->text()+"q");
        break;
    case BW:
        if (may)
            output_line->setText(output_line->text()+"W");
        else
            output_line->setText(output_line->text()+"w");
        break;
    case BE :
        if (may)
            output_line->setText(output_line->text()+"E");
        else
            output_line->setText(output_line->text()+"e");
        break;
    case BR :
        if (may)
            output_line->setText(output_line->text()+"R");
        else
            output_line->setText(output_line->text()+"r");
        break;
    case BT:
        if (may)
            output_line->setText(output_line->text()+"T");
        else
            output_line->setText(output_line->text()+"t");
        break;
    case BY:
        if (may)
            output_line->setText(output_line->text()+"Y");
        else
            output_line->setText(output_line->text()+"y");
        break;
    case BU:
        if (may)
            output_line->setText(output_line->text()+"U");
        else
            output_line->setText(output_line->text()+"u");
        break;
    case BI:
        if (may)
            output_line->setText(output_line->text()+"I");
        else
            output_line->setText(output_line->text()+"i");
        break;
    case BO :
        if (may)
            output_line->setText(output_line->text()+"O");
        else
            output_line->setText(output_line->text()+"o");
        break;
    case BP:
        if (may)
            output_line->setText(output_line->text()+"P");
        else
            output_line->setText(output_line->text()+"p");
        break;
    case MAY :
        may = !may;
        break;
    case BA :
        if (may)
            output_line->setText(output_line->text()+"A");
        else
            output_line->setText(output_line->text()+"a");
        break;
    case BS:
        if (may)
            output_line->setText(output_line->text()+"S");
        else
            output_line->setText(output_line->text()+"s");
        break;
    case BD:
        if (may)
            output_line->setText(output_line->text()+"D");
        else
            output_line->setText(output_line->text()+"d");
        break;
    case BF:
        if (may)
            output_line->setText(output_line->text()+"F");
        else
            output_line->setText(output_line->text()+"f");
        break;
    case BG:
        if (may)
            output_line->setText(output_line->text()+"G");
        else
            output_line->setText(output_line->text()+"g");
        break;
    case BH:
        if (may)
            output_line->setText(output_line->text()+"H");
        else
            output_line->setText(output_line->text()+"h");
        break;
    case BJ:
        if (may)
            output_line->setText(output_line->text()+"J");
        else
            output_line->setText(output_line->text()+"j");
        break;
    case BK:
        if (may)
            output_line->setText(output_line->text()+"K");
        else
            output_line->setText(output_line->text()+"k");
        break;
    case BL:
        if (may)
            output_line->setText(output_line->text()+"L");
        else
            output_line->setText(output_line->text()+"l");
        break;
    case SHIFT :
        break;
    case CTRL :
        break;
    case SPACE :
        output_line->setText(output_line->text()+" ");
        break;
    case ALT :
        break;
    case BLOQ :
        break;
    case BZ :
        if (may)
            output_line->setText(output_line->text()+"Z");
        else
            output_line->setText(output_line->text()+"z");
        break;
    case BX:
        if (may)
            output_line->setText(output_line->text()+"X");
        else
            output_line->setText(output_line->text()+"x");
        break;
    case BC:
        if (may)
            output_line->setText(output_line->text()+"C");
        else
            output_line->setText(output_line->text()+"c");
        break;
    case BV:
        if (may)
            output_line->setText(output_line->text()+"V");
        else
            output_line->setText(output_line->text()+"v");
        break;
    case BB:
        if (may)
            output_line->setText(output_line->text()+"B");
        else
            output_line->setText(output_line->text()+"b");
        break;
    case BN:
        if (may)
            output_line->setText(output_line->text()+"N");
        else
            output_line->setText(output_line->text()+"n");
        break;
    case BM:
        if (may)
            output_line->setText(output_line->text()+"M");
        else
            output_line->setText(output_line->text()+"m");
        break;
    case BCUT :
        if (may)
            output_line->setText(output_line->text()+";");
        else
            output_line->setText(output_line->text()+",");
        break;
    case BDOT:
        if (may)
            output_line->setText(output_line->text()+":");
        else
            output_line->setText(output_line->text()+".");
        break;
    case MIN :
        if (may)
            output_line->setText(output_line->text()+"_");
        else
            output_line->setText(output_line->text()+"-");
        break;
    case BDIV :
        output_line->setText(output_line->text()+"/");
        break;
    case BPROD :
        output_line->setText(output_line->text()+"*");
        break;
    case BRPLUS :
        output_line->setText(output_line->text()+"+");
        break;
    case BMSG :
        if (may)
            output_line->setText(output_line->text()+">");
        else
            output_line->setText(output_line->text()+"<");
        break;
    case BTAB:
        output_line->setText(output_line->text()+"\t");
        break;
    case 65535:
        break;
    }

}
