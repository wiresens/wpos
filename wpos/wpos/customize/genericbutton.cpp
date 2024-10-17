/***************************************************************************
                          GenericButton.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "genericbutton.h"

#include <wposcore/genericsignalmanager.h>
#include <libbslxml/xmlconfig.h>

#include <QString>

struct EventAspect{
    QString text;
    QString pixmap;
    bool set_icon;
};

struct XmlPair{
    QString domain;
    QString value;
};

struct GenericDataRelations{
    QList<XmlPair*> pair;
    QString signal_data;
};

struct GenericButtonEvent{
    HList<GenericDataRelations> *data_relations{};
    EventAspect *aspect{};
    QStringList signal_names;
    QString event_name;
};

GenericButton::GenericButton(
    const QString& text,
    QWidget *parent,
    const QString& name ):
    QPushButton(text, parent)
{
    setObjectName(name);
    init(text);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
}

GenericButton::~GenericButton(){
    clearAllRelationships();
    event_list->clear();
}

void GenericButton::init(const QString& text){
    QString aux_text;
    event_list = new HList<GenericButtonEvent>;
    resetEventList();

    aux_text = text;
    aux_text.replace("_"," ");
    aux_text.replace(" ","\n");
    setText(aux_text);

    //connect each signal
    connect(this, &GenericButton::released, this, &GenericButton::releasedSlot);
    connect(this, &GenericButton::clicked, this, &GenericButton::clickedSlot);
    connect(this, &GenericButton::pressed, this, &GenericButton::pressedSlot);
    connect(this, &GenericButton::toggled, this, &GenericButton::toggleSlot);

//    resize(Product::MIN_WIDTH,Product::MIN_HEIGHT);
//    setPalette(QPalette(PRODUCT_BG_COLOR));
//    setBackgroundRole(QPalette::Window);;
//    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

}

void GenericButton::resetEventList(){
    GenericButtonEvent *event{};
    //clear the lists
    clearAllRelationships();
    event_list->clear();

    QStringList event_names {"released", "clicked", "pressed", "toggle_on", "toggle_off" };

    //create a node for each signal event
    for( auto name : event_names){
        event = new GenericButtonEvent;
        event->data_relations = new HList<GenericDataRelations>;
        event->event_name = name;
        event_list->append(event, event->event_name);
    }
}

GenericButtonEvent* GenericButton::getEvent(EventType event_type){
    GenericButtonEvent *event{};
    switch (event_type){
    case Released:
        event = event_list->find("released");
        break;
    case Clicked:
        event = event_list->find("clicked");
        break;
    case Pressed:
        event = event_list->find("pressed");
        break;
    case ToggleOn:
        event = event_list->find("toggle_on");
        break;
    case ToggleOff:
        event = event_list->find("toggle_off");
        break;
    }
    return event;
}

bool GenericButton::createRelationship(
    const QString& signal_name,
    EventType event_type)
{

    GenericButtonEvent* event { getEvent(event_type) };
    if (!event) return false;

    auto gsm = GenericSignalManager::instance();
    if ( !isGenericSignalRegistered(signal_name) )
        gsm->publishGenericSignal(signal_name, this);

    event->signal_names.append(signal_name);
    last_signal_name = signal_name;
    last_event_type = event_type;
    return true;
}

bool GenericButton::createDataRelationship(
    const QString & signal_name,
    EventType type)
{

    GenericButtonEvent *event = getEvent(type);
    if (!event) return false;

    //register this button for that relationship
    auto gsm = GenericSignalManager::instance();
    if ( !isGenericDataSignalRegistered(signal_name) )
        gsm->publishGenericDataSignal(signal_name, this);

    //check if the signal already exists at this event
    GenericDataRelations *relation = event->data_relations->find(signal_name);
    if (relation) return false;

    //create the new relation
    relation = new GenericDataRelations;
    relation->signal_data = signal_name;
    event->data_relations->append(relation, signal_name);

    last_signal_name = signal_name;
    last_event_type = type;
    return true;
}

bool GenericButton::addContent(
    const QString& signal_name,
    const QString& domain,
    const QString& value,
    EventType type )
{
    GenericDataRelations *relation {};
    XmlPair *pair {};
    GenericButtonEvent *event {};
    EventType correct_event_type = type;

    if (type == LastEvent){
        if (!last_event_type)
            correct_event_type = DefaultEventType;
        else
            correct_event_type = last_event_type;
    }
    last_event_type = correct_event_type;

    event = getEvent(correct_event_type);
    if (!event)
        return false;

    relation = event->data_relations->find(signal_name);
    if (!relation){
        createDataRelationship(signal_name, correct_event_type);
        relation = event->data_relations->find(signal_name);
    }

    pair = new XmlPair;
    pair->domain = domain;
    pair->value = value;
    (relation->pair).append(pair);
    return true;
}

bool GenericButton::addContent(
    const QString& domain,
    const QString& value,
    EventType event)
{
    if (last_signal_name.isEmpty())
        return false;

    return addContent(last_signal_name, domain, value, event);
}

void GenericButton::changeAspect(
    EventType event_type,
    const QString& text,
    const QString& pixmap,
    bool setIcon)
{
    GenericButtonEvent *event = getEvent(event_type);
    if ( text.isEmpty() || pixmap.isEmpty() || !event ) return;

    if ( event->aspect )
        delete event->aspect;

    EventAspect *aspect = new EventAspect{ text, pixmap , setIcon};
    event->aspect = aspect;
}

void GenericButton::clearAspect(EventType type){

    GenericButtonEvent *event = getEvent(type);
    if (!event) return;

    delete event->aspect;
    event->aspect = 0;
}

void GenericButton::clearRelationship(
    const QString& signal_name)
{
    clearRelationship(Released, signal_name);
    clearRelationship(Clicked, signal_name);
    clearRelationship(Pressed, signal_name);
    clearRelationship(ToggleOn,signal_name);
    clearRelationship(ToggleOff,signal_name);
}

void GenericButton::clearDataRelationship(const QString & signal_name){
    clearDataRelationship(Released,signal_name);
    clearDataRelationship(Clicked,signal_name);
    clearDataRelationship(Pressed,signal_name);
    clearDataRelationship(ToggleOn,signal_name);
    clearDataRelationship(ToggleOff,signal_name);
}

void GenericButton::clearRelationship(
    EventType type,
    const QString& signal_name)
{
    int count,i;
    QString aux;
    GenericButtonEvent *event = getEvent(type);
    if (!event) return;

    auto gsm = GenericSignalManager::instance();
    //all the signals should be removed
    if (signal_name.isEmpty()){
        count = event->signal_names.count();
        for (i=0;i<count;i++){
            aux = event->signal_names[i];
            gsm->unpublishGenericSignal(aux, this);
        }
        //clear the list
        event->signal_names.clear();
    }
    else{//only "signal_name" should be removed
        count = event->signal_names.count();
        for (i=0;i<count;i++){
            aux = event->signal_names[i];
            if ( aux == signal_name )
                gsm->unpublishGenericSignal(aux,this);
        }
    }
}

void GenericButton::clearDataRelationship(
    EventType event,
    const QString& signal_name)
{
    GenericButtonEvent *e{};
    GenericDataRelations *relation {};
    int i;
    int count = 0;
    QString aux;
    e = getEvent(event);
    if (!e) return;

    auto gsm = GenericSignalManager::instance();
    //all the signals should be removed
    if (signal_name.isEmpty()){
        e->data_relations->count();
        for (i=0;i<count;i++){
            relation = e->data_relations->at(i);
            aux = relation->signal_data;
            relation->pair.clear();
            gsm->unpublishGenericDataSignal(aux,this);
        }
        //clear the list
        e->data_relations->clear();
    }
    else{//only "signal_name" should be removed
        relation = e->data_relations->find(signal_name);
        if (!relation)  //no signal with this name at event
            return;
        relation->pair.clear();
        gsm->unpublishGenericSignal(signal_name,this);
        e->data_relations->remove(signal_name);
    }
}

void GenericButton::clearAllRelationships(){
    clearRelationship("");
    clearDataRelationship("");
    last_signal_name.clear();
}

void GenericButton::releasedSlot(){
    execEvent(Released);
}

void GenericButton::clickedSlot(){
    execEvent(Clicked);
}

void GenericButton::pressedSlot(){
    execEvent(Pressed);
}

void GenericButton::toggleSlot(bool state){
    if (state) execEvent(ToggleOn);
    else execEvent(ToggleOff);
}

/**
*       All the button events are executed in three steps:
*       step 1: the "aspect" is changed
*       step 2: the "genericSignals" are emited
*       step 3: the "genericDataSignals" are emited
*/

void GenericButton::execEvent(EventType event){
    int count,i,j,pair_count;
    QString aux;
    GenericButtonEvent *e {};
    GenericDataRelations *relation{};
    XmlPair *pair {};
    XmlConfig *xml {};

    e = getEvent(event);
    if (!e) return;

    //change the aspect.
    if (e->aspect){
        QString text;
        text = e->aspect->text;
        text.replace("_"," ");
        text.replace(" ","\n");
        setText(text);
        if (e->aspect->set_icon)
            setIcon(QIcon(QPixmap(e->aspect->pixmap)));
        else
            setIcon(QPixmap(e->aspect->pixmap));
        setIconSize(QSize{32,32});
    }

    //emit genericSignals
    count = e->signal_names.count();
    for (i=0;i<count;i++){
        aux = e->signal_names[i];
        emit genericSignal(aux);
    }

    //emit genericDataSignals.
    count = e->data_relations->count();
    for (i=0;i<count;i++){
        xml = new XmlConfig();
        xml->delDomain();
        relation = e->data_relations->at(i);
        pair_count = relation->pair.count();
        for( j = 0; j < pair_count; j++ ){
            pair = relation->pair.at(j);
            xml->createElement(pair->domain, pair->value);
        }
        emit genericDataSignal(relation->signal_data, xml); //Is it safe to put xml in std::unique_ptr, because here is a callback
        delete xml;
        xml = 0;
    }
}

bool GenericButton::isGenericSignalRegistered(const QString& signal_name){
    GenericButtonEvent *e = 0;
    int count,i,event_count,j;
    event_count = event_list->count();
    for (j=0; j < event_count ;j++){
        e = event_list->at(j);
        count = e->signal_names.count();
        for (i=0;i<count;i++){
            if (e->signal_names[i] == signal_name)
                return true;
        }
    }
    return false;
}

bool GenericButton::isGenericDataSignalRegistered(const QString& signal_name){
    GenericButtonEvent *e{};
    GenericDataRelations *relation{};
    int event_count,j;
    event_count = event_list->count();
    for (j=0; j < event_count ;j++){
        e = event_list->at(j);
        relation = e->data_relations->find(signal_name);
        if (relation)
            return true;
    }
    return false;
}
