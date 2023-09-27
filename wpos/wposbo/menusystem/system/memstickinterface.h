/***************************************************************************
                          memstickinterface.h  -  description
                             -------------------
    begin                : mar jul 27 2004
    copyright            : (C) 2004 by Carlos Manzanedo
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
#ifndef MEMSTICKINTERFACE_H
#define MEMSTICKINTERFACE_H

class MemStickInterface{
    virtual void operationResultSlot(bool) = 0; //ASYNC
};
#endif
