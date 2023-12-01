/***************************************************************************
                          nkeyboard.cpp  -  description
                             -------------------
    begin                : lun jun 7 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com
 **************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "nkeyboard.h"
#include <libbslxml/xmlconfig.h>
#include <wposcore/productextrainfo.h>
#include <wposwidget/global.h>

#include <QSignalMapper>
//#include <QLayout>
//#include <QLineEdit>
#include <QKeyEvent>
//#include <QApplication>
//#include <QButtonGroup>
//#include <QString>
//#include <QStackedWidget>
#include <QButtonGroup>
//#include <QAbstractButton>
//#include <QRegExp>
//#include <QPixmap>
//#include <QIcon>
//#include <QFile>

#include <iostream>

#define BTAB         0

Qt::KeyboardModifiers ESC_ASCII;
#define SPACE       32
#define BINT        39

#define BPROD       42
#define BRPLUS      43
#define BCUT        44
#define MIN         45
#define BDOT        46
#define BDIV        47
#define B0          48
#define B1          49
#define B2          50
#define B3          51
#define B4          52
#define B5          53
#define B6          54
#define B7          55
#define B8          56
#define B9          57

#define BMSG        60
#define BA          65
#define BB          66
#define BC          67
#define BD          68
#define BE          69
#define BF          70
#define BG          71
#define BH          72
#define BI          73
#define BJ          74
#define BK          75
#define BL          76
#define BM          77
#define BN          78
#define BO          79
#define BP          80
#define BQ          81
#define BR          82
#define BS          83
#define BT          84
#define BU          85
#define BV          86
#define BW          87
#define BX          88
#define BY          89
#define BZ          90

#define OA_ASCII   186
#define BEN_ASCII  241

#define ESC       4096
#define DEL       4099
#define ENTER     4100
#define INTRO     4101
#define SHIFT     4128
#define CTRL      4129
#define MAY       4132
#define BLOQ      4133
#define ALT       4135
#define F1        4144
#define F2        4145
#define F3        4146
#define F4        4147
#define F5        4148
#define F6        4149
#define F7        4150
#define F8        4151
#define F9        4152
#define F10       4153
#define F11       4154
#define F12       4155

#define OA       65535
#define BEN      65535

#define LOUNGES_FILE "/etc/ntpv/lounges.xml"

static const uint  BUTTON_WIDTH = 65;
static const uint  BUTTON_HEIGTH = 45;
static constexpr QSize BUTTON_SIZE{BUTTON_WIDTH, BUTTON_HEIGTH};

static const uint GRID_ROW = 3;
static const uint GRID_COL = 10;

NKeyboard::NKeyboard(
    QWidget* parent,
    const QString& name
)
    :QMenu(parent),
    o_parent{ parent }
{
    setupUi(this);
    setObjectName(name);
    setFocusPolicy(Qt::NoFocus);

    signalMapper = new QSignalMapper(this);
    signalMapper->setObjectName("KeyboardSignalMapper");
    setMapping(signalMapper);
}

NKeyboard::NKeyboard(
    const QString& text,
    QWidget* parent,
    const QString& name
)
    :NKeyboard(parent, name)
{
    output_line->setText(text);
}

void NKeyboard::setMapping(QSignalMapper *signalMapper){
    connect(signalMapper, &QSignalMapper::mappedInt, this, &NKeyboard::keyClicked);

    keyboard_buttons.append(esc,QString::number(ESC));
    keyboard_buttons.append(f1,QString::number(F1));
    keyboard_buttons.append(f2,QString::number(F2));
    keyboard_buttons.append(f3,QString::number(F3));
    keyboard_buttons.append(f4,QString::number(F4));
    keyboard_buttons.append(f5,QString::number(F5));
    keyboard_buttons.append(f6,QString::number(F6));
    keyboard_buttons.append(f7,QString::number(F7));
    keyboard_buttons.append(f8,QString::number(F8));
    keyboard_buttons.append(f9,QString::number(F9));
    keyboard_buttons.append(f10,QString::number(F10));
    keyboard_buttons.append(f11,QString::number(F11));
    keyboard_buttons.append(f12,QString::number(F12));
    keyboard_buttons.append(oa,QString::number(OA_ASCII));
    keyboard_buttons.append(b1,QString::number(B1));
    keyboard_buttons.append(b2,QString::number(B2));
    keyboard_buttons.append(b3,QString::number(B3));
    keyboard_buttons.append(b4,QString::number(B4));
    keyboard_buttons.append(b5,QString::number(B5));
    keyboard_buttons.append(b6,QString::number(B6));
    keyboard_buttons.append(b7,QString::number(B7));
    keyboard_buttons.append(b8,QString::number(B8));
    keyboard_buttons.append(b9,QString::number(B9));
    keyboard_buttons.append(b0,QString::number(B0));
    keyboard_buttons.append(bint,QString::number(BINT));
    keyboard_buttons.append(bdel,QString::number(DEL));
    keyboard_buttons.append(btab,QString::number(BTAB));
    keyboard_buttons.append(bq,QString::number(BQ));
    keyboard_buttons.append(bw,QString::number(BW));
    keyboard_buttons.append(be,QString::number(BE));
    keyboard_buttons.append(br,QString::number(BR));
    keyboard_buttons.append(bt,QString::number(BT));
    keyboard_buttons.append(by,QString::number(BY));
    keyboard_buttons.append(bu,QString::number(BU));
    keyboard_buttons.append(bi,QString::number(BI));
    keyboard_buttons.append(bo,QString::number(BO));
    keyboard_buttons.append(bp,QString::number(BP));
    keyboard_buttons.append(benter,QString::number(ENTER));
    keyboard_buttons.append(bmay,QString::number(MAY));
    keyboard_buttons.append(ba,QString::number(BA));
    keyboard_buttons.append(bs,QString::number(BS));
    keyboard_buttons.append(bd,QString::number(BD));
    keyboard_buttons.append(bf,QString::number(BF));
    keyboard_buttons.append(bg,QString::number(BG));
    keyboard_buttons.append(bh,QString::number(BH));
    keyboard_buttons.append(bj,QString::number(BJ));
    keyboard_buttons.append(bk,QString::number(BK));
    keyboard_buttons.append(bl,QString::number(BL));
    keyboard_buttons.append(ben,QString::number(BEN_ASCII));
    keyboard_buttons.append(blsh,QString::number(SHIFT));
    keyboard_buttons.append(bmsg,QString::number(BMSG));
    keyboard_buttons.append(bz,QString::number(BZ));
    keyboard_buttons.append(bx,QString::number(BX));
    keyboard_buttons.append(bc,QString::number(BC));
    keyboard_buttons.append(bv,QString::number(BV));
    keyboard_buttons.append(bb,QString::number(BB));
    keyboard_buttons.append(bn,QString::number(BN));
    keyboard_buttons.append(bm,QString::number(BM));
    keyboard_buttons.append(bcut,QString::number(BCUT));
    keyboard_buttons.append(bdot,QString::number(BDOT));
    keyboard_buttons.append(blmi,QString::number(MIN));
    keyboard_buttons.append(brsh,QString::number(SHIFT));
    keyboard_buttons.append(blctrl,QString::number(CTRL));
    keyboard_buttons.append(blalt,QString::number(ALT));
    keyboard_buttons.append(bspace,QString::number(SPACE));
    keyboard_buttons.append(bralt,QString::number(ALT));
    keyboard_buttons.append(brctrl,QString::number(CTRL));
    keyboard_buttons.append(bbloq,QString::number(BLOQ));
    keyboard_buttons.append(bdiv,QString::number(BDIV));
    keyboard_buttons.append(bprod,QString::number(BPROD));
    keyboard_buttons.append(brmin,QString::number(MIN));
    keyboard_buttons.append(brplus,QString::number(BRPLUS));
    keyboard_buttons.append(brenter,QString::number(INTRO));
    keyboard_buttons.append(brdot,QString::number(BDOT));
    keyboard_buttons.append(br0,QString::number(B0));
    keyboard_buttons.append(br1,QString::number(B1));
    keyboard_buttons.append(br2,QString::number(B2));
    keyboard_buttons.append(br3,QString::number(B3));
    keyboard_buttons.append(br4,QString::number(B4));
    keyboard_buttons.append(br5,QString::number(B5));
    keyboard_buttons.append(br6,QString::number(B6));
    keyboard_buttons.append(br7,QString::number(B7));
    keyboard_buttons.append(br8,QString::number(B8));
    keyboard_buttons.append(br9,QString::number(B9));

    signalMapper->setMapping(esc,ESC);
    signalMapper->setMapping(f1,F1);
    signalMapper->setMapping(f2,F2);
    signalMapper->setMapping(f3,F3);
    signalMapper->setMapping(f4,F4);
    signalMapper->setMapping(f5,F5);
    signalMapper->setMapping(f6,F6);
    signalMapper->setMapping(f7,F7);
    signalMapper->setMapping(f8,F8);
    signalMapper->setMapping(f9,F9);
    signalMapper->setMapping(f10,F10);
    signalMapper->setMapping(f11,F11);
    signalMapper->setMapping(f12,F12);
    signalMapper->setMapping(oa,OA_ASCII);
    signalMapper->setMapping(b1,B1);
    signalMapper->setMapping(b2,B2);
    signalMapper->setMapping(b3,B3);
    signalMapper->setMapping(b4,B4);
    signalMapper->setMapping(b5,B5);
    signalMapper->setMapping(b6,B6);
    signalMapper->setMapping(b7,B7);
    signalMapper->setMapping(b8,B8);
    signalMapper->setMapping(b9,B9);
    signalMapper->setMapping(b0,B0);
    signalMapper->setMapping(bint,BINT);
    signalMapper->setMapping(bdel,DEL);
    signalMapper->setMapping(btab,BTAB);
    signalMapper->setMapping(bq,BQ);
    signalMapper->setMapping(bw,BW);
    signalMapper->setMapping(be,BE);
    signalMapper->setMapping(br,BR);
    signalMapper->setMapping(bt,BT);
    signalMapper->setMapping(by,BY);
    signalMapper->setMapping(bu,BU);
    signalMapper->setMapping(bi,BI);
    signalMapper->setMapping(bo,BO);
    signalMapper->setMapping(bp,BP);
    signalMapper->setMapping(benter,ENTER);
    signalMapper->setMapping(bmay,MAY);
    signalMapper->setMapping(ba,BA);
    signalMapper->setMapping(bs,BS);
    signalMapper->setMapping(bd,BD);
    signalMapper->setMapping(bf,BF);
    signalMapper->setMapping(bg,BG);
    signalMapper->setMapping(bh,BH);
    signalMapper->setMapping(bj,BJ);
    signalMapper->setMapping(bk,BK);
    signalMapper->setMapping(bl,BL);
    signalMapper->setMapping(ben,BEN_ASCII);
    signalMapper->setMapping(blsh,SHIFT);
    signalMapper->setMapping(bmsg,BMSG);
    signalMapper->setMapping(bz,BZ);
    signalMapper->setMapping(bx,BX);
    signalMapper->setMapping(bc,BC);
    signalMapper->setMapping(bv,BV);
    signalMapper->setMapping(bb,BB);
    signalMapper->setMapping(bn,BN);
    signalMapper->setMapping(bm,BM);
    signalMapper->setMapping(bcut,BCUT);
    signalMapper->setMapping(bdot,BDOT);
    signalMapper->setMapping(blmi,MIN);
    signalMapper->setMapping(brsh,SHIFT);
    signalMapper->setMapping(blctrl,CTRL);
    signalMapper->setMapping(blalt,ALT);
    signalMapper->setMapping(bspace,SPACE);
    signalMapper->setMapping(bralt,ALT);
    signalMapper->setMapping(brctrl,CTRL);
    signalMapper->setMapping(bbloq,BLOQ);
    signalMapper->setMapping(bdiv,BDIV);
    signalMapper->setMapping(bprod,BPROD);
    signalMapper->setMapping(brmin,MIN);
    signalMapper->setMapping(brplus,BRPLUS);
    signalMapper->setMapping(brenter,INTRO);
    signalMapper->setMapping(brdot,BDOT);
    signalMapper->setMapping(br0,B0);
    signalMapper->setMapping(br1,B1);
    signalMapper->setMapping(br2,B2);
    signalMapper->setMapping(br3,B3);
    signalMapper->setMapping(br4,B4);
    signalMapper->setMapping(br5,B5);
    signalMapper->setMapping(br6,B6);
    signalMapper->setMapping(br7,B7);
    signalMapper->setMapping(br8,B8);
    signalMapper->setMapping(br9,B9);



    connect(f1, &QPushButton::released,signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f2, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f3, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f4, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f5, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f6, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f7, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f8, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f9, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f10, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f11, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(f12, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(oa, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b1, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b2, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b3, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b4, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b5, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b6, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b7, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b8, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b9, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(b0, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bint, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bdel, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(btab, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bq, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bw, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(be, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bt, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(by, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bu, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bi , &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bo, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bp, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(benter, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bmay , &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ba, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bs , &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bd, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bf , &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bg, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bh, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bj , &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bk, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bl , &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ben, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(blsh, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bmsg, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bz, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bx, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bc, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bv, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bb, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bn, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bm, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bcut, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bdot, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(blmi, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(brsh, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(blctrl, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(blalt , &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bspace, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bralt, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(brctrl, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bbloq, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bdiv , &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(bprod, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(brmin, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(brplus, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(brenter, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(brdot, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br0 , &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br1, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br2, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br3, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br4, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br5, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br6, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br7, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br8, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(br9, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));

    connect(tables_exit_button,  &QPushButton::released, this, &NKeyboard::exitSlot);
    connect(exit_button,  &QPushButton::released, this, &NKeyboard::exitSlot);
    connect(exit_button,  &QPushButton::released, this, &NKeyboard::exitClicked);
    connect(clear_button,  &QPushButton::released,this, &NKeyboard::clearSlot);
    connect(output_line, &QLineEdit::textChanged, this, &NKeyboard::textChanged);

    connect(lounge_button, &QPushButton::clicked, this, &NKeyboard::loungeButtonClicked);
    connect(kb_button, &QPushButton::clicked, this, &NKeyboard::kbButtonClicked);

    lounge_button->setObjectName("button_group");
    lounge_button->setIcon(QPixmap("controls:lounge.png"));
    exit_button->setIcon(QPixmap("controls:gexit.png"));
    kb_button->setIcon(QPixmap("controls:keyboard.png"));
    tables_exit_button->setIcon(exit_button->icon());

    output_line->setFocus();

    for(auto button : keyboard_buttons){
        button->setFocusProxy(this);
        button->setFocusPolicy(Qt::NoFocus);
    }

    lounge_buttongroup = new QButtonGroup(lounge_page);
    lounge_buttongroup->setExclusive(true);
    connect(lounge_buttongroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this , &NKeyboard::switchLounge);

    main_stack->setCurrentWidget(keyboard_page);

    if ( !QFile(LOUNGES_FILE).exists() ) lounge_button->hide();
    else{
        XmlConfig xml (LOUNGES_FILE);
        if (xml.wellFormed()) loadLounges(&xml);
    }
}

void NKeyboard::switchLounge(QAbstractButton* button){
    std::cout <<"Lounge : "<< lounge_buttongroup->buttons().indexOf(button) << '\n';
    lounge_stack->setCurrentIndex(lounge_buttongroup->buttons().indexOf(button));
    button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));

    for( auto other_button : lounge_buttongroup->buttons()){
        if( ! other_button->isDown())
            other_button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
        button->setBackgroundRole(QPalette::Button);
    }
}

void NKeyboard::showLounge(){
    main_stack->setCurrentWidget(lounge_page);
    emit loungeSignal(true);
}

void NKeyboard::kbButtonClicked(){
    main_stack->setCurrentWidget(keyboard_page);
    emit loungeSignal(false);
}

void NKeyboard::loadLounges(XmlConfig *xml){
    int lounge_count;
    int tables_count;
    QPushButton *lounge_buttongroup_button = 0;
    QGridLayout *glayout = 0;
    QButtonGroup *lounge_stack_buttongroup = 0;

    lounges.clear();
    while(lounge_stack->currentWidget())
        lounge_stack->removeWidget(lounge_stack->currentWidget());

    xml->delDomain();
    lounge_count = xml->howManyTags("lounge");
    if ( lounge_count )  main_stack->setCurrentWidget(lounge_page);

    for (auto i = 0; i < lounge_count; i++){
        xml->setDomain("lounge["+QString::number(i)+"]");
        auto lounge_name = xml->readString("name");
        tables_count = xml->readInt("tables");

        if ( lounge_name.isEmpty() || tables_count == 0) continue;

        lounge_buttongroup_button = new QPushButton(lounge_page);
        lounge_buttongroup_button->setCheckable(true);
        lounge_buttongroup_button->setVisible(true);

        lounge_buttongroup->addButton(lounge_buttongroup_button);
        lounge_button_layout->insertWidget(i, lounge_buttongroup_button);

        auto lounge = new LoungeData();
        lounge->name = lounge_name;
        lounge->tables = tables_count;

        lounge_buttongroup_button->setObjectName(lounge->name);
        lounge_buttongroup_button->setText(lounge->name.replace(' ', "\n"));
        lounge->button = lounge_buttongroup_button;
        lounge->regex = QRegExp("^" + lounge->name + "[0-9]{1,2}");

        auto iconset = xml->readString("iconset");
        auto pixmap = xml->readString("pixmap");
        auto tables_iconset = xml->readString("tablesiconset");

        if (!iconset.isEmpty())
            lounge_buttongroup_button->setIcon(QIcon(iconset));

        if (!pixmap.isEmpty())
            lounge_buttongroup_button->setIcon(QPixmap(pixmap));

        if (xml->setDomain("options")){
            for( auto j = 0 ; j < xml->howManyTags("option"); j++){
                if ( j == 0 )  lounge->option_nodes = new HList<ProductExtraInfo>;
                auto option_type = xml->readString("option["+QString::number(j)+"].type");
                auto option_value = xml->readString("option["+QString::number(j)+"].value");
                if ((option_type.isEmpty()) || (option_value.isEmpty()))  continue;
                auto option = lounge->option_nodes->find(option_value);
                if ( !option ){
                    option = new ProductExtraInfo(option_type);
                    lounge->option_nodes->append(option, option_type);
                }
                option->addOption(option_value, true);
            }
            xml->releaseDomain("options");
        }

        auto lounge_stack_page = new QFrame(lounge_stack);
        lounge_stack_page->setFrameShape(QFrame::NoFrame);
        lounge_stack_buttongroup = new QButtonGroup(lounge_stack_page);
        lounge_stack_buttongroup->setExclusive(true);
        lounge_stack_buttongroup->setObjectName(lounge->name);

        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lounge_stack->sizePolicy().hasHeightForWidth());
        lounge_stack_page->setSizePolicy(sizePolicy);

        glayout = new QGridLayout(lounge_stack_page);
        lounge->button_group = lounge_stack_buttongroup;

        connect(lounge_stack_buttongroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
                this, QOverload<QAbstractButton*>::of(&NKeyboard::tableButtonClicked));

        lounge->stack_index = lounge_stack->addWidget(lounge_stack_page);

        for ( auto j = 0; j < tables_count; j++){
            auto lounge_table_button = new QPushButton(lounge_stack_page);
            lounge_table_button->setCheckable(true);
            lounge_table_button->setObjectName(QString::number(j+1));
            lounge_table_button->setText(lounge_table_button->objectName());
            lounge_table_button->setFixedSize(BUTTON_SIZE);

            if (!tables_iconset.isEmpty())
                lounge_table_button->setIcon(QIcon(tables_iconset));

            auto row = (int) (j / GRID_COL ) ;
            auto col = j % GRID_COL;

            glayout->addWidget(lounge_table_button, row, col);
            lounge_stack_buttongroup->addButton(lounge_table_button);
            lounge->table_buttons->append(lounge_table_button, lounge_table_button->objectName());
        }
        lounges.append(lounge, lounge->name);
        xml->releaseDomain("lounge");
    }

    if (lounge_count){
        lounges.at(0)->button->setChecked(true);
        lounges.at(0)->button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
        lounges.at(0)->button->setBackgroundRole(QPalette::Button);
        lounge_stack->setCurrentIndex(lounges.at(0)->stack_index);
    }

}

void NKeyboard::loungeButtonClicked(bool checked){

    (void) checked;
    LoungeData *lounge{};
    QPushButton* button{};
    for( auto child : lounge_buttongroup->children() ){
        button = qobject_cast<QPushButton*>(child);
        if( !button ) continue;
        if (button->isDown()){
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
            button->setBackgroundRole(QPalette::Button);
            lounge = lounges.find(button->objectName());
            lounge_stack->setCurrentIndex(lounge->stack_index);
        }
        else{
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
            button->setBackgroundRole(QPalette::Button);
        }
    }
}

void NKeyboard::tableButtonClicked(QAbstractButton* button){

    button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
    auto lounge_stack_buttongroup = qobject_cast<QButtonGroup*>(sender());

    for( auto lounge : lounges){
        if( lounge->button_group != lounge_stack_buttongroup)
            clearLoungeStack(lounge->name);
        else output_line->setText( lounge->name + " " + button->objectName() );
    }

    for( auto other_button : lounge_stack_buttongroup->buttons() ){
        if (! other_button->isDown())
            other_button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
        other_button->setBackgroundRole(QPalette::Window);
    }
}

void NKeyboard::clearLoungeStack(const QString &name, bool clear_enabled){
    int i,count;
    LoungeData *lounge{};
    QPushButton *button=0;

    lounge = lounges.find(name);
    if (!lounge) return;

    count = lounge->table_buttons->count();
    for(i=0;i<count;i++){
        button = lounge->table_buttons->at(i);
        if (clear_enabled){
            button->setEnabled(true);
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
            button->setBackgroundRole(QPalette::Window);
            button->setChecked(false);
        }
        else{
            if (button->isEnabled()){
                button->setDown(false);
                button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
                button->setBackgroundRole(QPalette::Window);
            }
        }
    }
}

void NKeyboard::clearAllStacks(bool clear_enabled){
    int i,count;
    LoungeData *lounge{};

    count = lounges.count();
    for(i=0;i<count;i++){
        lounge = lounges.at(i);
        clearLoungeStack(lounge->name, clear_enabled);
    }
}

void NKeyboard::setTableButtonSelected(const QString& name){
    int i,count;
    int pos,real_int;
    bool ok;
    LoungeData *lounge{};
    QPushButton *button = 0;
    QString aux_name;
    QString aux_int;

    pos = name.indexOf(' ');
    aux_name = name.mid(0,pos);
    aux_int = name.mid(pos+1,name.length()-(pos+1));

    real_int = aux_int.toInt(&ok);
    if ((!ok)&&(real_int>=0))
        return;

    count = lounges.count();
    for (i=0;i<count;i++){
        lounge = lounges.at(i);
        button = lounge->button;
        button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
        button->setBackgroundRole(QPalette::Window);
        button->setDown(false);
        if (lounge->name.contains(aux_name)){
            button = lounge->button;
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
            button->setBackgroundRole(QPalette::Window);
            button->setDown(true);
            lounge_stack->setCurrentIndex(lounge->stack_index);
            if ((real_int-1)>lounge->tables)
                return;
            button = lounge->table_buttons->at(real_int-1);
            button->setEnabled(true);
            button->setDown(true);
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
            button->setBackgroundRole(QPalette::Window);
        }
    }
}

void NKeyboard::setTableButtonUsed(const QString& name){
    int i,count;
    int pos,real_int;
    bool ok;
    LoungeData *lounge{};
    QPushButton *button = 0;
    QString aux_name;
    QString aux_int;

    pos = name.indexOf(' ');
    aux_name = name.mid(0, pos);
    aux_int = name.mid(pos+1,name.length()-(pos+1));

    real_int = aux_int.toInt(&ok);
    if ((!ok)&&(real_int>=0))
        return;

    count = lounges.count();
    for (i=0;i<count;i++){
        lounge = lounges.at(i);
        if (lounge->name.contains(aux_name)){
            if ((real_int-1)>lounge->tables)
                return;
            button = lounge->table_buttons->at(real_int-1);
            button->setEnabled(false);
            button->setDown(false);
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_SELECTED_COLOR));
            button->setBackgroundRole(QPalette::Window);
        }
    }
}

void NKeyboard::setUsedTables(QStringList table_codes){

    auto selected_tbl = output_line->text();
    clearAllStacks(true);

    for (auto& used_table : table_codes)
        setTableButtonUsed(used_table);

    setTableButtonSelected(selected_tbl);
}

bool NKeyboard::isAtTable(){
    QString aux;
    aux = getLounge();
    return (!aux.isEmpty());
}

QString NKeyboard::getLounge(){
    int i,count;
    QString aux="";
    LoungeData *lounge{} ;
    aux = output_line->text();
    count = lounges.count();
    for(i=0;i<count;i++){
        lounge = lounges.at(i);
        if (lounge->regex.exactMatch(aux)){
            return lounge->name;
        }
    }
    return aux;
}

int NKeyboard::getTable(){
    int i,count;
    int pos,real_int;
    bool ok=false;
    QString aux="";
    QString aux_int;
    LoungeData *lounge{} ;
    aux = output_line->text();
    count = lounges.count();
    for(i=0;i<count;i++){
        lounge = lounges.at(i);
        if (lounge->regex.exactMatch(aux)){
            pos = aux.indexOf(' ');
            aux_int = aux.mid(pos+1,aux.length()-(pos+1));
            real_int = aux_int.toInt(&ok);
            if ((ok)&&(real_int>=0))
                return real_int;
        }
    }
    return -1;
}

HList<ProductExtraInfo>* NKeyboard::getOptionsFromLounge(const QString& lounge){
    LoungeData *lounge_data{} ;
    lounge_data  = lounges.find(lounge);
    if (lounge_data)
        return lounge_data->option_nodes;

    return 0;
}

void NKeyboard::setExitWithEnter(bool on){
    exit_with_enter = on;
}

void NKeyboard::setText(const QString& text){
    output_line->setText(text);
    setTableButtonSelected(text);
}

void NKeyboard::exitSlot(){
    //exits produce an enter event.
    if (exit_with_enter)
        emit enterPressed(output_line->text());
    else
        emit exitClicked();
}

void NKeyboard::clearSlot(){
    output_line->clear();
}

void NKeyboard::setSendEvents(bool on){
    has_send_events = on;
}

void NKeyboard::keyClicked(int num){
    QString texto;
    //Presently, this does not support event passing
    switch (num){
    case ESC:
        if ( has_send_events && o_parent ){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress, ESC, ESC_ASCII ,state);
            ker = new QKeyEvent(QEvent::KeyRelease,ESC,ESC_ASCII, state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        this->exitSlot();
        break;
    case F1:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F1,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F1,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F2:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F2,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F2,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F3:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F3,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F3,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F4:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F4,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F4,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F5:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F5,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F5,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F6:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F6,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F6,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F7:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F7,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F7,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F8:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F8,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F8,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F9:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress, F9, 0,state);
            ker = new QKeyEvent(QEvent::KeyRelease, F9, 0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F10 :
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress,F10,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F10,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F11:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress, F11,0,state);
            ker = new QKeyEvent(QEvent::KeyRelease,F11,0,state);
            qApp->postEvent(o_parent,(QEvent *) kep);
            qApp->postEvent(o_parent,(QEvent *) ker);
        }
        break;
    case F12:
        if ((has_send_events)&&(o_parent)){
            QKeyEvent *kep = 0;
            QKeyEvent *ker = 0;
            kep = new QKeyEvent(QEvent::KeyPress, Qt::Key_F12 ,0, state);
            ker = new QKeyEvent(QEvent::KeyRelease, F12, 0 ,state);
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
        if (caps_lock)
            output_line->setText(output_line->text()+"Q");
        else
            output_line->setText(output_line->text()+"q");
        break;
    case BW:
        if (caps_lock)
            output_line->setText(output_line->text()+"W");
        else
            output_line->setText(output_line->text()+"w");
        break;
    case BE :
        if (caps_lock)
            output_line->setText(output_line->text()+"E");
        else
            output_line->setText(output_line->text()+"e");
        break;
    case BR :
        if (caps_lock)
            output_line->setText(output_line->text()+"R");
        else
            output_line->setText(output_line->text()+"r");
        break;
    case BT:
        if (caps_lock)
            output_line->setText(output_line->text()+"T");
        else
            output_line->setText(output_line->text()+"t");
        break;
    case BY:
        if (caps_lock)
            output_line->setText(output_line->text()+"Y");
        else
            output_line->setText(output_line->text()+"y");
        break;
    case BU:
        if (caps_lock)
            output_line->setText(output_line->text()+"U");
        else
            output_line->setText(output_line->text()+"u");
        break;
    case BI:
        if (caps_lock)
            output_line->setText(output_line->text()+"I");
        else
            output_line->setText(output_line->text()+"i");
        break;
    case BO :
        if (caps_lock)
            output_line->setText(output_line->text()+"O");
        else
            output_line->setText(output_line->text()+"o");
        break;
    case BP:
        if (caps_lock)
            output_line->setText(output_line->text()+"P");
        else
            output_line->setText(output_line->text()+"p");
        break;
    case MAY :
        caps_lock = !caps_lock;
        break;
    case BA :
        if (caps_lock)
            output_line->setText(output_line->text()+"A");
        else
            output_line->setText(output_line->text()+"a");
        break;
    case BS:
        if (caps_lock)
            output_line->setText(output_line->text()+"S");
        else
            output_line->setText(output_line->text()+"s");
        break;
    case BD:
        if (caps_lock)
            output_line->setText(output_line->text()+"D");
        else
            output_line->setText(output_line->text()+"d");
        break;
    case BF:
        if (caps_lock)
            output_line->setText(output_line->text()+"F");
        else
            output_line->setText(output_line->text()+"f");
        break;
    case BG:
        if (caps_lock)
            output_line->setText(output_line->text()+"G");
        else
            output_line->setText(output_line->text()+"g");
        break;
    case BH:
        if (caps_lock)
            output_line->setText(output_line->text()+"H");
        else
            output_line->setText(output_line->text()+"h");
        break;
    case BJ:
        if (caps_lock)
            output_line->setText(output_line->text()+"J");
        else
            output_line->setText(output_line->text()+"j");
        break;
    case BK:
        if (caps_lock)
            output_line->setText(output_line->text()+"K");
        else
            output_line->setText(output_line->text()+"k");
        break;
    case BL:
        if (caps_lock)
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
        if (caps_lock)
            output_line->setText(output_line->text()+"Z");
        else
            output_line->setText(output_line->text()+"z");
        break;
    case BX:
        if (caps_lock)
            output_line->setText(output_line->text()+"X");
        else
            output_line->setText(output_line->text()+"x");
        break;
    case BC:
        if (caps_lock)
            output_line->setText(output_line->text()+"C");
        else
            output_line->setText(output_line->text()+"c");
        break;
    case BV:
        if (caps_lock)
            output_line->setText(output_line->text()+"V");
        else
            output_line->setText(output_line->text()+"v");
        break;
    case BB:
        if (caps_lock)
            output_line->setText(output_line->text()+"B");
        else
            output_line->setText(output_line->text()+"b");
        break;
    case BN:
        if (caps_lock)
            output_line->setText(output_line->text()+"N");
        else
            output_line->setText(output_line->text()+"n");
        break;
    case BM:
        if (caps_lock)
            output_line->setText(output_line->text()+"M");
        else
            output_line->setText(output_line->text()+"m");
        break;
    case BCUT :
        if (caps_lock)
            output_line->setText(output_line->text()+";");
        else
            output_line->setText(output_line->text()+",");
        break;
    case BDOT:
        if (caps_lock)
            output_line->setText(output_line->text()+":");
        else
            output_line->setText(output_line->text()+".");
        break;
    case MIN :
        if (caps_lock)
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
        if (caps_lock)
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
