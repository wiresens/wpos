/***************************************************************************
                          filealterationwatcherinterface.h  -  description
                             -------------------
    begin                : may 12 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef _ALTERATION_WATCHER_INTERFACE_
#define _ALTERATION_WATCHER_INTERFACE_

/**
  *@author Carlos Manzanedo
*/

class NAlterationWatcherInterface {
    virtual void initDownButtonGroupSlot()=0;
    virtual void initUpButtonGroupSlot()=0;
    virtual void initProductsSlot()=0;
    virtual void initDevicesSlot()=0;
    virtual void initOffersSlot()=0;
};
#endif
