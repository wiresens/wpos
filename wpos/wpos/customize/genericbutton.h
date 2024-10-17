/***************************************************************************
                          genericmenubutton.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef GENERICBUTTON_H
#define GENERICBUTTON_H


#include <QPushButton>
#include <wposcore/hlist.h>


/**
*    this is only a button wich emits a genericSignal whenever it's pressed.
*    it can be used as a virtual class to inherit from.
*    the other class to implement this kind of generic buttons is GenericDataButton
*    wich also add to the class the capability of sending data with the signal.
*
*    this two classes are designed to be used with the generic signal manager.
*    the wright way to do this is the following: first create the button with a generic
*    signal name. BE SURE of the signal name (it's a string). register the QObject at
*    the generic signal manager and the rest of the classes will receive the signal via
*    the generic signal manager
*/

class QString;
class EventAspect;
class XmlPair;
class GenericDataRelations;
class GenericButtonEvent;
class XmlConfig;


class GenericButton : public QPushButton  {
    Q_OBJECT
public:
    enum  EventType {
        LastEvent       = 0,
        Released        = 1,
        Clicked         = 2,
        Pressed         = 3,
        ToggleOn        = 4,
        ToggleOff       = 5,
        DefaultEventType = Released
    };

    GenericButton(const QString& text,
                  QWidget *parent,
                  const QString& name = QString{});

    ~GenericButton();

    //to use the genericSignals
    bool createRelationship(const QString& signal_name, EventType event_type = Released);

    //to use the generic data signals
    bool createDataRelationship(const QString & signal_name, EventType type = Released);
    bool addContent(const QString& signal_name,
                    const QString& domain,
                    const QString& value,
                    EventType type = LastEvent);

    bool addContent(const QString& domain,
                    const QString& value,
                    EventType event = LastEvent);

public slots:

    //used to change the aspect when the button has been pressed, clicked, released, toggledon or toggledout
    void changeAspect(EventType event_type, const QString& text, const QString& pixmap, bool setIcon=true);
    void clearAspect(EventType type);

    /**
     *       used to clear the relationships from the data structs...
     *       without signal_name it will clear all the datasignals with this signal.
     */
    void clearRelationship(const QString& signal_name="");
    void clearDataRelationship(const QString & signal_name="");
    void clearRelationship(EventType type, const QString& signal_name="");
    void clearDataRelationship(EventType event, const QString& signal_name="");
    void clearAllRelationships();

protected slots:
    virtual void releasedSlot();
    virtual void clickedSlot();
    virtual void pressedSlot();
    virtual void toggleSlot(bool state);

signals:
    void genericSignal(const QString& signal_name);
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);

protected:
    void init(const QString& text="");
    void resetEventList();
    GenericButtonEvent* getEvent(EventType event_type);
    void execEvent(EventType event);

    bool isGenericSignalRegistered(const QString& signal_name);
    bool isGenericDataSignalRegistered(const QString& signal_name);

    HList<GenericButtonEvent> *event_list{};
    QString last_signal_name;
    EventType last_event_type{LastEvent};
};

#endif
