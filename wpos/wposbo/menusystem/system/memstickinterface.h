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
#ifndef MEM_STICK_INTERFACE_H
#define MEM_STICK_INTERFACE_H

class MemStickInterface {
    virtual void setOperationResult(bool) = 0; // ASYNC
};

#endif // MEM_STICK_INTERFACE_H
