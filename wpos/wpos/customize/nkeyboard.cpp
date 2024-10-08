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

#include "ui_nkeyboardwidget.h"

#include "nkeyboard.h"
#include <libbslxml/xmlconfig.h>
#include <wposcore/productextrainfo.h>
#include <wposcore/config.h>
#include <wposgui/common/global.h>

#include <QSignalMapper>
#include <QKeyEvent>
#include <QButtonGroup>

static constexpr int ESC {4096};
static constexpr Qt::KeyboardModifiers  ESC_ASCII;
static constexpr int F1 {4144};
static constexpr int F2 {4145};
static constexpr int F3 {4146};
static constexpr int F4 {4147};
static constexpr int F5 {4148};
static constexpr int F6 {4149};
static constexpr int F7 {4150};
static constexpr int F8 {4151};
static constexpr int F9 {4152};
static constexpr int F10 {4153};
static constexpr int F11 {4154};
static constexpr int F12 {4155};
static constexpr int OA {65535};
static constexpr int OA_ASCII {186};
static constexpr int B1 {49};
static constexpr int B2 {50};
static constexpr int B3 {51};
static constexpr int B4 {52};
static constexpr int B5 {53};
static constexpr int B6 {54};
static constexpr int B7 {55};
static constexpr int B8 {56};
static constexpr int B9 {57};
static constexpr int B0 {48};
static constexpr int BINT {39};
static constexpr int DEL {4099};
static constexpr int ENTER {4100};
static constexpr int INTRO {4101};
static constexpr int BQ {81};
static constexpr int BW {87};
static constexpr int BE {69};
static constexpr int BR {82};
static constexpr int BT {84};
static constexpr int BY {89};
static constexpr int BU {85};
static constexpr int BI {73};
static constexpr int BO {79};
static constexpr int BP {80};
static constexpr int MAY {44132};
static constexpr int BA {65};
static constexpr int BS {83};
static constexpr int BD {68};
static constexpr int BF {70};
static constexpr int BG {71};
static constexpr int BH {72};
static constexpr int BJ {74};
static constexpr int BK {75};
static constexpr int BL {76};
static constexpr int BEN {65535};
static constexpr int BEN_ASCII {241};
static constexpr int SHIFT {4128};
static constexpr int CTRL {4129};
static constexpr int SPACE {32};
static constexpr int ALT {4135};
static constexpr int BLOQ {4133};
static constexpr int BZ {90};
static constexpr int BX {88};
static constexpr int BC {67};
static constexpr int BV {86};
static constexpr int BB {66};
static constexpr int BN {78};
static constexpr int BM {77};
static constexpr int BCUT {44};
static constexpr int BDOT {46};
static constexpr int MIN {45};
static constexpr int BDIV {47};
static constexpr int BPROD {42};
static constexpr int BRPLUS {43};
static constexpr int BMSG {60};
static constexpr int BTAB {0};

static constexpr uint  BTN_WIDTH  = 65;
static constexpr uint  BTN_HEIGTH = 45;
static constexpr QSize BTN_SIZE{BTN_WIDTH, BTN_HEIGTH};

static constexpr uint GRID_ROW = 3;
static constexpr uint GRID_COL = 10;

namespace KeyBoard{
struct Key
{
    QPushButton *key;
    const int val;
};
}

struct LoungeData {
    LoungeData() = default;
    ~LoungeData();

    HList<QPushButton>      table_buttons;
    HList<ProductExtraInfo> *option_nodes{};
    QButtonGroup            *button_group{};
    QPushButton             *button{};

    QRegExp     regex;
    QString     name;
    uint        stack_index{};
    uint        tables{};
};

LoungeData::~LoungeData(){

    if (option_nodes){
        option_nodes->clear();
        delete option_nodes;
    }

    delete button;
    button_group = 0;
}

NKeyboard::NKeyboard(
    QWidget* parent,
    const QString& name
)
    :QMenu(parent),
    ui{ new Ui::NKeyboardWidget},
    m_obj_parent{ parent }
{
    ui->setupUi(this);
    QSignalMapper *sgmpr = new QSignalMapper(this);
    setObjectName(name);
    setFocusPolicy(Qt::NoFocus);   
    build(sgmpr);
}

NKeyboard::~NKeyboard(){
    delete ui;
}

void NKeyboard::build(QSignalMapper* sgmpr)
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

    ui->lounge_button->setObjectName("LoungeButton"); //kb
    ui->lounge_button->setIcon(QPixmap("controls:lounge.png"));//kb
    ui->exit_button->setIcon(QPixmap("controls:gexit.png"));//kb
    ui->kb_button->setIcon(QPixmap("controls:keyboard.png")); //lg
    ui->tables_exit_button->setIcon(ui->exit_button->icon());//lg

    ui->output_line->setFocus();
    connect(sgmpr, &QSignalMapper::mappedInt, this, &NKeyboard::keyClicked); //kb
    for( auto rec : key_map){
        // keys.append(rec.key, QString::number(rec.val));
        sgmpr->setMapping(rec.key, rec.val);
        rec.key->setFocusProxy(this);
        rec.key->setFocusPolicy(Qt::NoFocus);
        connect(rec.key, &QPushButton::released, sgmpr, QOverload<>::of(&QSignalMapper::map));
    }
    disconnect(ui->esc, &QPushButton::released, sgmpr, QOverload<>::of(&QSignalMapper::map));

    connect(ui->tables_exit_button,  &QPushButton::released, this, &NKeyboard::exitSlot);//lg
    connect(ui->exit_button,  &QPushButton::released, this, &NKeyboard::exitClicked);//kb
    connect(ui->exit_button, &QPushButton::released, this, &NKeyboard::exitSlot);//kb
    connect(ui->clear_button, &QPushButton::released, this, &NKeyboard::clearSlot);//kb
    connect(ui->output_line,  &QLineEdit::textChanged, this, &NKeyboard::textChanged);//kb

    connect(ui->lounge_button, &QPushButton::clicked, this, &NKeyboard::loungeButtonClicked); //kb
    connect(ui->kb_button, &QPushButton::clicked, this, &NKeyboard::showKeyBoard);//kb

    m_lounge_btn_group = new QButtonGroup(ui->lounge_page); //lg
    m_lounge_btn_group->setExclusive(true);
    connect(m_lounge_btn_group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this , &NKeyboard::switchLounge);

    ui->main_stack->setCurrentWidget(ui->keyboard_page);

    auto lounge_xml_file = QFile(cfg::xmlFileByKey(cfg::XMLKey::Lounges)); //lg
    if ( ! lounge_xml_file.exists())
        ui->lounge_button->hide();
    else{
        XmlConfig xml (lounge_xml_file.fileName());
        if ( xml.wellFormed() ) loadLounges(xml);
    }
}

void NKeyboard::write(const QString& text){
    ui->output_line->setText(text);
}

QString NKeyboard::read() const{
    return ui->output_line->text();
}

void NKeyboard::setExitWithEnter(bool on){
    m_exit_with_enter = on;
}

void NKeyboard::exitSlot(){
    //exits produce an enter event.
    if (m_exit_with_enter)
        emit enterPressed(read());
    else
        emit exitClicked();
}

void NKeyboard::switchLounge(QAbstractButton* button){
    ui->lounge_stack->setCurrentIndex(m_lounge_btn_group->buttons().indexOf(button));
    button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));

    for( auto other_button : m_lounge_btn_group->buttons()){
        if( ! other_button->isDown())
            other_button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
        button->setBackgroundRole(QPalette::Button);
    }
}

void NKeyboard::showLounge(){
    ui->main_stack->setCurrentWidget(ui->lounge_page);
    emit loungeRequested(true);
}

void NKeyboard::showKeyBoard(){
    ui->main_stack->setCurrentWidget(ui->keyboard_page);
    emit loungeRequested(false);
}

void NKeyboard::loadLounges(XmlConfig &xml){
    int lounge_count;
    int tables_count;
    QPushButton *lounge_buttongroup_button = 0;
    QGridLayout *glayout = 0;
    QButtonGroup *lounge_stack_buttongroup = 0;

    while(ui->lounge_stack->currentWidget())
        ui->lounge_stack->removeWidget(ui->lounge_stack->currentWidget());

    lounge_count = xml.howManyTags("lounge");
    if ( lounge_count )  ui->main_stack->setCurrentWidget(ui->lounge_page);

    for (auto i = 0; i < lounge_count; i++){
        xml.setDomain("lounge["+QString::number(i)+"]");
        auto lounge_name = xml.readString("name");
        tables_count = xml.readInt("tables");

        if ( lounge_name.isEmpty() || tables_count == 0) continue;

        lounge_buttongroup_button = new QPushButton(ui->lounge_page);
        lounge_buttongroup_button->setCheckable(true);
        lounge_buttongroup_button->setVisible(true);

        m_lounge_btn_group->addButton(lounge_buttongroup_button);
        ui->lounge_button_layout->insertWidget(i, lounge_buttongroup_button);

        auto lounge = new LoungeData();
        lounge->name = lounge_name;
        lounge->tables = tables_count;

        lounge_buttongroup_button->setObjectName(lounge->name);
        lounge_buttongroup_button->setText(lounge->name.replace(' ', "\n"));
        lounge->button = lounge_buttongroup_button;
        lounge->regex = QRegExp("^" + lounge->name + "[0-9]{1,2}");

        auto iconset = xml.readString("iconset");
        auto pixmap = xml.readString("pixmap");
        auto tables_iconset = xml.readString("tablesiconset");

        if (!iconset.isEmpty())
            lounge_buttongroup_button->setIcon(QIcon(iconset));

        if (!pixmap.isEmpty())
            lounge_buttongroup_button->setIcon(QPixmap(pixmap));

        if (xml.setDomain("options")){
            for( auto j = 0 ; j < xml.howManyTags("option"); j++){
                if ( j == 0 )  lounge->option_nodes = new HList<ProductExtraInfo>;
                auto option_type = xml.readString("option["+QString::number(j)+"].type");
                auto option_value = xml.readString("option["+QString::number(j)+"].value");
                if ((option_type.isEmpty()) || (option_value.isEmpty()))  continue;
                auto option = lounge->option_nodes->find(option_value);
                if ( !option ){
                    option = new ProductExtraInfo(option_type);
                    lounge->option_nodes->append(option, option_type);
                }
                option->addOption(option_value, true);
            }
            xml.releaseDomain("options");
        }

        auto lounge_stack_page = new QFrame(ui->lounge_stack);
        lounge_stack_page->setFrameShape(QFrame::NoFrame);
        lounge_stack_buttongroup = new QButtonGroup(lounge_stack_page);
        lounge_stack_buttongroup->setExclusive(true);
        lounge_stack_buttongroup->setObjectName(lounge->name);

        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ui->lounge_stack->sizePolicy().hasHeightForWidth());
        lounge_stack_page->setSizePolicy(sizePolicy);

        glayout = new QGridLayout(lounge_stack_page);
        lounge->button_group = lounge_stack_buttongroup;

        connect(lounge_stack_buttongroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
                this, QOverload<QAbstractButton*>::of(&NKeyboard::tableButtonClicked));

        lounge->stack_index = ui->lounge_stack->addWidget(lounge_stack_page);

        for ( auto j = 0; j < tables_count; j++){
            auto lounge_table_button = new QPushButton(lounge_stack_page);
            lounge_table_button->setCheckable(true);
            lounge_table_button->setObjectName(QString::number(j+1));
            lounge_table_button->setText(lounge_table_button->objectName());
            lounge_table_button->setFixedSize(BTN_SIZE);

            if (!tables_iconset.isEmpty())
                lounge_table_button->setIcon(QIcon(tables_iconset));

            auto row = (int) (j / GRID_COL ) ;
            auto col = j % GRID_COL;

            glayout->addWidget(lounge_table_button, row, col);
            lounge_stack_buttongroup->addButton(lounge_table_button);
            lounge->table_buttons.append(lounge_table_button, lounge_table_button->objectName());
        }
        m_lounges.append(lounge, lounge->name);
        xml.releaseDomain("lounge");
    }

    if (lounge_count){
        m_lounges.at(0)->button->setChecked(true);
        m_lounges.at(0)->button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
        m_lounges.at(0)->button->setBackgroundRole(QPalette::Button);
        ui->lounge_stack->setCurrentIndex(m_lounges.at(0)->stack_index);
    }

}

void NKeyboard::loungeButtonClicked(bool checked){

    (void) checked;
    LoungeData *lounge{};
    QPushButton* button{};
    for( auto child : m_lounge_btn_group->children() ){
        button = qobject_cast<QPushButton*>(child);
        if( !button ) continue;
        if (button->isDown()){
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
            button->setBackgroundRole(QPalette::Button);
            lounge = m_lounges.find(button->objectName());
            ui->lounge_stack->setCurrentIndex(lounge->stack_index);
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

    for( auto lounge : m_lounges){
        if( lounge->button_group != lounge_stack_buttongroup)
            clearLoungeStack(lounge->name);
        else write( lounge->name + " " + button->objectName() );
    }

    for( auto other_button : lounge_stack_buttongroup->buttons() ){
        if (! other_button->isDown())
            other_button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
        other_button->setBackgroundRole(QPalette::Window);
    }
}

void NKeyboard::clearLoungeStack(const QString &name, bool clear_enabled){
    LoungeData *lounge{};
    lounge = m_lounges.find(name);
    if (!lounge) return;

    for(auto * button : lounge->table_buttons){
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

    count = m_lounges.count();
    for(i=0;i<count;i++){
        lounge = m_lounges.at(i);
        clearLoungeStack(lounge->name, clear_enabled);
    }
}

void NKeyboard::setTableButtonSelected(const QString& name){
    int i,count;
    int pos,real_int;
    bool ok;
    QString aux_name;
    QString aux_int;

    pos = name.indexOf(' ');
    aux_name = name.mid( 0, pos);
    aux_int = name.mid( pos + 1, name.length() - ( pos + 1 ) );

    real_int = aux_int.toInt(&ok);
    if ((!ok)&&(real_int>=0))
        return;

    count = m_lounges.count();
    for (auto lounge : m_lounges){
        QPushButton *button = lounge->button;
        button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_OFF_COLOR));
        button->setBackgroundRole(QPalette::Window);
        button->setDown(false);
        if (lounge->name.contains(aux_name)){
            button = lounge->button;
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
            button->setBackgroundRole(QPalette::Window);
            button->setDown(true);
            ui->lounge_stack->setCurrentIndex(lounge->stack_index);
            if ( real_int - 1  > lounge->tables )
                return;
            button = lounge->table_buttons.at(real_int - 1 );
            button->setEnabled(true);
            button->setDown(true);
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_ON_COLOR));
            button->setBackgroundRole(QPalette::Window);
        }
    }
}

void NKeyboard::setTableButtonUsed(const QString& name){
    int pos,real_int;
    bool ok;
    LoungeData *lounge{};
    QPushButton *button = 0;
    QString aux_name;
    QString aux_int;

    pos = name.indexOf(' ');
    aux_name = name.mid(0, pos);
    aux_int = name.mid( pos + 1, name.length() -( pos + 1 ) );

    real_int = aux_int.toInt(&ok);
    if ( !ok && real_int >= 0 ) return;

    for ( auto* lounge : m_lounges){
        if ( lounge->name.contains(aux_name) ){
            if ( real_int - 1  > lounge->tables ) return;

            button = lounge->table_buttons.at(real_int - 1 );
            button->setEnabled(false);
            button->setDown(false);
            button->setPalette(QPalette(Colors::NUM_KEYBOARD_BUTTON_SELECTED_COLOR));
            button->setBackgroundRole(QPalette::Window);
        }
    }
}

void NKeyboard::setUsedTables(QStringList table_codes){

    auto selected_tbl = read();
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
    auto lounge_name = read();
    for( const auto& lounge : m_lounges ){
        if (lounge->regex.exactMatch(lounge_name)){
            lounge_name =  lounge->name;
            break;
        }
    }
    return lounge_name;
}

int NKeyboard::getTable(){
    int i,count;
    int pos,real_int;
    bool ok=false;
    QString aux="";
    QString aux_int;
    LoungeData *lounge{} ;
    aux = read();
    count = m_lounges.count();
    for(i=0;i<count;i++){
        lounge = m_lounges.at(i);
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

HList<ProductExtraInfo>*
NKeyboard::getOptionsFromLounge(
    const QString& lounge)
{
    LoungeData *lounge_data{} ;
    lounge_data  = m_lounges.find(lounge);
    if (lounge_data)
        return lounge_data->option_nodes;

    return {};
}

void NKeyboard::setText(const QString& text){
    write(text);
    setTableButtonSelected(text);
}

void NKeyboard::clearSlot(){
    ui->output_line->clear();
}

void NKeyboard::setSendEvents(bool on){
    m_has_send_events = on;
}

void NKeyboard::keyClicked(int key){
    QString input_text;
    QKeyEvent *kep{};
    QKeyEvent *ker{};
    auto modifier = Qt::NoModifier;
    //Presently, this does not support event passing
    switch (key){
    case ESC:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   ESC, Qt::MetaModifier);
            ker = new QKeyEvent(QEvent::KeyRelease, ESC, Qt::MetaModifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        exitSlot();
        break;
    case F1:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F1, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F1, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F2:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F2, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F2, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F3:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F3, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F3, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F4:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F4, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F4, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F5:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F5, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F5, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F6:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F6, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F6, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F7:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F7, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F7, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F8:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F8, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F8, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F9:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F9, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F9, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F10 :
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F10, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F10, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F11:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F11,  modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F11,  modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case F12:
        if ( m_has_send_events && m_obj_parent ){
            kep = new QKeyEvent(QEvent::KeyPress,   F12, modifier);
            ker = new QKeyEvent(QEvent::KeyRelease, F12, modifier);
            qApp->postEvent(m_obj_parent, kep);
            qApp->postEvent(m_obj_parent, ker);
        }
        break;
    case OA_ASCII:
    case SHIFT:
    case CTRL:
    case ALT:
    case BLOQ:
    case BEN:
        break;
    case ENTER :
    case INTRO :
        emit enterPressed(read());
        break;
    case DEL :
        input_text = read();
        input_text.truncate(input_text.length() -1 );
        write(input_text);
        break;
    case SPACE :
        write(read()+" ");
        break;
    case BTAB:
        write(read()+"\t");
        break;
    case BCUT :
        if (m_caps_lock)
            write(read()+";");
        else
            write(read()+",");
        break;
    case BDOT:
        if (m_caps_lock)
            write(read()+":");
        else
            write(read()+".");
        break;
    case MIN :
        if (m_caps_lock)
            write(read()+"_");
        else
            write(read()+"-");
        break;
    case BDIV :
        write(read()+"/");
        break;
    case BPROD :
        write(read()+"*");
        break;
    case BRPLUS :
        write(read()+"+");
        break;
    case BMSG :
        if (m_caps_lock)
            write(read()+">");
        else
            write(read()+"<");
        break;
    case B1:
        write(read()+"1");
        break;
    case B2:
        write(read()+"2");
        break;
    case B3:
        write(read()+"3");
        break;
    case B4:
        write(read()+"4");
        break;
    case B5:
        write(read()+"5");
        break;
    case B6:
        write(read()+"6");
        break;
    case B7:
        write(read()+"7");
        break;
    case B8:
        write(read()+"8");
        break;
    case B9:
        write(read()+"9");
        break;
    case B0:
        write(read()+"0");
        break;
    case BINT:
        write(read()+"?");
        break;
    case MAY :
        m_caps_lock = !m_caps_lock;
        break;
    case BA :
        if (m_caps_lock)
            write(read()+"A");
        else
            write(read()+"a");
        break;
    case BB:
        if (m_caps_lock)
            write(read()+"B");
        else
            write(read()+"b");
        break;
    case BC:
        if (m_caps_lock)
            write(read()+"C");
        else
            write(read()+"c");
        break;
    case BD:
        if (m_caps_lock)
            write(read()+"D");
        else
            write(read()+"d");
        break;
    case BE :
        if (m_caps_lock)
            write(read()+"E");
        else
            write(read()+"e");
        break;
    case BF:
        if (m_caps_lock)
            write(read()+"F");
        else
            write(read()+"f");
        break;
    case BG:
        if (m_caps_lock)
            write(read()+"G");
        else
            write(read()+"g");
        break;
    case BH:
        if (m_caps_lock)
            write(read()+"H");
        else
            write(read()+"h");
        break;
    case BI:
        if (m_caps_lock)
            write(read()+"I");
        else
            write(read()+"i");
        break;
    case BJ:
        if (m_caps_lock)
            write(read()+"J");
        else
            write(read()+"j");
        break;
    case BK:
        if (m_caps_lock)
            write(read()+"K");
        else
            write(read()+"k");
        break;
    case BL:
        if (m_caps_lock)
            write(read()+"L");
        else
            write(read()+"l");
        break;
    case BM:
        if (m_caps_lock)
            write(read()+"M");
        else
            write(read()+"m");
        break;
    case BN:
        if (m_caps_lock)
            write(read()+"N");
        else
            write(read()+"n");
        break;
    case BO :
        if (m_caps_lock)
            write(read()+"O");
        else
            write(read()+"o");
        break;
    case BP:
        if (m_caps_lock)
            write(read()+"P");
        else
            write(read()+"p");
        break;
    case BQ:
        if (m_caps_lock)
            write(read()+"Q");
        else
            write(read()+"q");
        break;
    case BR :
        if (m_caps_lock)
            write(read()+"R");
        else
            write(read()+"r");
        break;
    case BS:
        if (m_caps_lock)
            write(read()+"S");
        else
            write(read()+"s");
        break;
    case BT:
        if (m_caps_lock)
            write(read()+"T");
        else
            write(read()+"t");
        break;
    case BU:
        if (m_caps_lock)
            write(read()+"U");
        else
            write(read()+"u");
        break;
    case BV:
        if (m_caps_lock)
            write(read()+"V");
        else
            write(read()+"v");
        break;
    case BW:
        if (m_caps_lock)
            write(read()+"W");
        else
            write(read()+"w");
        break;
    case BX:
        if (m_caps_lock)
            write(read()+"X");
        else
            write(read()+"x");
        break;
    case BY:
        if (m_caps_lock)
            write(read()+"Y");
        else
            write(read()+"y");
        break;
    case BZ :
        if (m_caps_lock)
            write(read()+"Z");
        else
            write(read()+"z");
        break;
    }
}
