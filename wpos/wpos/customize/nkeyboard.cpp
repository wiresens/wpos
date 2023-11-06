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
#include <wposwidget/global.h>

#include <QSignalMapper>
#include <QLayout>
#include <QLineEdit>
#include <QKeyEvent>
#include <QApplication>
#include <QButtonGroup>
#include <QString>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QRegExp>
#include <QPixmap>
#include <QIcon>

#include <xmlconfig.h>
#include <optionnode.h>
#include <QFile>

#include <iostream>
using namespace std;

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
        QWidget *parent,
        const QString& name ):

    QMenu(parent),
    keyboard_buttons  {new HList<QPushButton>},
    lounges {new HList<LoungeData>},
    o_parent{ parent }
{
    setupUi(this);
    setObjectName(name);
    setFocusPolicy(Qt::NoFocus);
    init();
}

NKeyboard::NKeyboard(
        const QString& text,
        QWidget *parent,
        const QString& name):

    NKeyboard(parent, name)
{
    output_line->setText(text);
}

NKeyboard::~NKeyboard(){
    delete signal_mapper;
}

void NKeyboard::init(){

    signal_mapper = new QSignalMapper(this);
    signal_mapper->setObjectName("keyboard_signal_mapper");

    keyboard_buttons->append(esc,QString::number(ESC));
    keyboard_buttons->append(f1,QString::number(F1));
    keyboard_buttons->append(f2,QString::number(F2));
    keyboard_buttons->append(f3,QString::number(F3));
    keyboard_buttons->append(f4,QString::number(F4));
    keyboard_buttons->append(f5,QString::number(F5));
    keyboard_buttons->append(f6,QString::number(F6));
    keyboard_buttons->append(f7,QString::number(F7));
    keyboard_buttons->append(f8,QString::number(F8));
    keyboard_buttons->append(f9,QString::number(F9));
    keyboard_buttons->append(f10,QString::number(F10));
    keyboard_buttons->append(f11,QString::number(F11));
    keyboard_buttons->append(f12,QString::number(F12));
    keyboard_buttons->append(oa,QString::number(OA_ASCII));
    keyboard_buttons->append(b1,QString::number(B1));
    keyboard_buttons->append(b2,QString::number(B2));
    keyboard_buttons->append(b3,QString::number(B3));
    keyboard_buttons->append(b4,QString::number(B4));
    keyboard_buttons->append(b5,QString::number(B5));
    keyboard_buttons->append(b6,QString::number(B6));
    keyboard_buttons->append(b7,QString::number(B7));
    keyboard_buttons->append(b8,QString::number(B8));
    keyboard_buttons->append(b9,QString::number(B9));
    keyboard_buttons->append(b0,QString::number(B0));
    keyboard_buttons->append(bint,QString::number(BINT));
    keyboard_buttons->append(bdel,QString::number(DEL));
    keyboard_buttons->append(btab,QString::number(BTAB));
    keyboard_buttons->append(bq,QString::number(BQ));
    keyboard_buttons->append(bw,QString::number(BW));
    keyboard_buttons->append(be,QString::number(BE));
    keyboard_buttons->append(br,QString::number(BR));
    keyboard_buttons->append(bt,QString::number(BT));
    keyboard_buttons->append(by,QString::number(BY));
    keyboard_buttons->append(bu,QString::number(BU));
    keyboard_buttons->append(bi,QString::number(BI));
    keyboard_buttons->append(bo,QString::number(BO));
    keyboard_buttons->append(bp,QString::number(BP));
    keyboard_buttons->append(benter,QString::number(ENTER));
    keyboard_buttons->append(bmay,QString::number(MAY));
    keyboard_buttons->append(ba,QString::number(BA));
    keyboard_buttons->append(bs,QString::number(BS));
    keyboard_buttons->append(bd,QString::number(BD));
    keyboard_buttons->append(bf,QString::number(BF));
    keyboard_buttons->append(bg,QString::number(BG));
    keyboard_buttons->append(bh,QString::number(BH));
    keyboard_buttons->append(bj,QString::number(BJ));
    keyboard_buttons->append(bk,QString::number(BK));
    keyboard_buttons->append(bl,QString::number(BL));
    keyboard_buttons->append(ben,QString::number(BEN_ASCII));
    keyboard_buttons->append(blsh,QString::number(SHIFT));
    keyboard_buttons->append(bmsg,QString::number(BMSG));
    keyboard_buttons->append(bz,QString::number(BZ));
    keyboard_buttons->append(bx,QString::number(BX));
    keyboard_buttons->append(bc,QString::number(BC));
    keyboard_buttons->append(bv,QString::number(BV));
    keyboard_buttons->append(bb,QString::number(BB));
    keyboard_buttons->append(bn,QString::number(BN));
    keyboard_buttons->append(bm,QString::number(BM));
    keyboard_buttons->append(bcut,QString::number(BCUT));
    keyboard_buttons->append(bdot,QString::number(BDOT));
    keyboard_buttons->append(blmi,QString::number(MIN));
    keyboard_buttons->append(brsh,QString::number(SHIFT));
    keyboard_buttons->append(blctrl,QString::number(CTRL));
    keyboard_buttons->append(blalt,QString::number(ALT));
    keyboard_buttons->append(bspace,QString::number(SPACE));
    keyboard_buttons->append(bralt,QString::number(ALT));
    keyboard_buttons->append(brctrl,QString::number(CTRL));
    keyboard_buttons->append(bbloq,QString::number(BLOQ));
    keyboard_buttons->append(bdiv,QString::number(BDIV));
    keyboard_buttons->append(bprod,QString::number(BPROD));
    keyboard_buttons->append(brmin,QString::number(MIN));
    keyboard_buttons->append(brplus,QString::number(BRPLUS));
    keyboard_buttons->append(brenter,QString::number(INTRO));
    keyboard_buttons->append(brdot,QString::number(BDOT));
    keyboard_buttons->append(br0,QString::number(B0));
    keyboard_buttons->append(br1,QString::number(B1));
    keyboard_buttons->append(br2,QString::number(B2));
    keyboard_buttons->append(br3,QString::number(B3));
    keyboard_buttons->append(br4,QString::number(B4));
    keyboard_buttons->append(br5,QString::number(B5));
    keyboard_buttons->append(br6,QString::number(B6));
    keyboard_buttons->append(br7,QString::number(B7));
    keyboard_buttons->append(br8,QString::number(B8));
    keyboard_buttons->append(br9,QString::number(B9));

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

    connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(keyClicked(int)));

    connect(tables_exit_button, SIGNAL(released()), this, SLOT(exitSlot()));
    connect(exit_button, SIGNAL(released()), this, SLOT(exitSlot()));

    connect(exit_button, SIGNAL(released()), this, SIGNAL(exitClicked()));

    connect(clear_button, SIGNAL(released()),this, SLOT(clearSlot()));
    connect(output_line, SIGNAL(textChanged(const QString &)), this, SIGNAL(textChanged(const QString &)));

    connect(lounge_button, SIGNAL(clicked()), this,SLOT(loungeButtonClicked()));
    connect(kb_button, SIGNAL(clicked()), this, SLOT(kbButtonClicked()));

    lounge_button->setObjectName("button_group");
    lounge_button->setIcon(QPixmap("controls:lounge.png"));
    exit_button->setIcon(QPixmap("controls:gexit.png"));
    kb_button->setIcon(QPixmap("controls:keyboard.png"));
    tables_exit_button->setIcon(exit_button->icon());

    output_line->setFocus();

    for(auto button : *keyboard_buttons){
        button->setFocusProxy(this);
        button->setFocusPolicy(Qt::NoFocus);
    }

    lounge_buttongroup = new QButtonGroup(lounge_page);
    lounge_buttongroup->setExclusive(true);
    connect(lounge_buttongroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this , SLOT(switchLounge(QAbstractButton*)) );

    main_stack->setCurrentWidget(keyboard_page);

    if ( !QFile(LOUNGES_FILE).exists() ) lounge_button->hide();
    else{
        XmlConfig xml (LOUNGES_FILE);
        if (xml.isValid()) loadLounges(&xml);
    }
}

void NKeyboard::switchLounge(QAbstractButton* button){
    cout <<"Lounge : "<< lounge_buttongroup->buttons().indexOf(button) << endl;
    lounge_stack->setCurrentIndex(lounge_buttongroup->buttons().indexOf(button));
    button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));

    for( auto other_button : lounge_buttongroup->buttons()){
        if( ! other_button->isDown())
            other_button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
        button->setBackgroundRole(QPalette::Button);
    }
}

void NKeyboard::loungeButtonClicked(){
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

    lounges->clear();
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
                if ( j == 0 )  lounge->option_nodes = new HList<OptionNode>;
                auto option_type = xml->readString("option["+QString::number(j)+"].type");
                auto option_value = xml->readString("option["+QString::number(j)+"].value");
                if ((option_type.isEmpty()) || (option_value.isEmpty()))  continue;
                auto option = lounge->option_nodes->find(option_value);
                if ( !option ){
                    option = new OptionNode(option_type);
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

        connect(lounge_stack_buttongroup, SIGNAL(buttonClicked(QAbstractButton*)),
                this, SLOT(tableButtonClicked(QAbstractButton*)));

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
        lounges->append(lounge, lounge->name);
        xml->releaseDomain("lounge");
    }

    if (lounge_count){
        lounges->at(0)->button->setChecked(true);
        lounges->at(0)->button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
        lounges->at(0)->button->setBackgroundRole(QPalette::Button);
        lounge_stack->setCurrentIndex(lounges->at(0)->stack_index);
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
            lounge = lounges->find(button->objectName());
            lounge_stack->setCurrentIndex(lounge->stack_index);
        }
        else{
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
            button->setBackgroundRole(QPalette::Button);
        }
    }
}

void NKeyboard::tableButtonClicked(bool checked){
    int i,count;
    (void) checked;
    LoungeData *lounge{};
    LoungeData *aux_lounge = 0;
    QString aux;
    QPushButton *button=0;

    for( auto child : lounge_buttongroup->children() ){

        button = qobject_cast<QPushButton*>(child);
        if( !button ) continue;

        if (button->isDown()){
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
            button->setBackgroundRole(QPalette::Window);
            lounge = lounges->find(button->objectName());
        }
    }

    count = lounges->count();
    for(i=0;i<count;i++){
        aux_lounge = lounges->at(i);
        if (aux_lounge != lounge){
            clearLoungeStack(aux_lounge->name);
        }
    }

    aux = lounge->name+ " ";
    count = lounge->table_buttons->count();
    for(i=0;i<count;i++){
        button = lounge->table_buttons->at(i);
        if (!button->isEnabled())
            continue;
        if (button->isDown()){
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
            button->setBackgroundRole(QPalette::Window);
            aux += button->objectName();
        }
        else{
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
            button->setBackgroundRole(QPalette::Window);
        }
    }
    output_line->setText(aux);

}

void NKeyboard::tableButtonClicked(QAbstractButton* button){

    button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
    auto lounge_stack_buttongroup = qobject_cast<QButtonGroup*>(sender());

    for( auto lounge : *lounges){
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

    lounge = lounges->find(name);
    if (!lounge)
        return;

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

    count = lounges->count();
    for(i=0;i<count;i++){
        lounge = lounges->at(i);
        clearLoungeStack(lounge->name,clear_enabled);
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

    count = lounges->count();
    for (i=0;i<count;i++){
        lounge = lounges->at(i);
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

    count = lounges->count();
    for (i=0;i<count;i++){
        lounge = lounges->at(i);
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
    count = lounges->count();
    for(i=0;i<count;i++){
        lounge = lounges->at(i);
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
    count = lounges->count();
    for(i=0;i<count;i++){
        lounge = lounges->at(i);
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

HList<OptionNode>* NKeyboard::getOptionsFromLounge(const QString& lounge){
    LoungeData *lounge_data{} ;
    lounge_data  = lounges->find(lounge);
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
