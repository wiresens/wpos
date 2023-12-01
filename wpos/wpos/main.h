/***************************************************************************
                          main.h  -  description
                             -------------------
    begin                : mar jul 22 2003
    copyright            : (C) 2003 by Carlos Manzanedo
    email                : support@bluesock.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#include <wposcore/genericsignalmanager.h>
#include "auth/authcore.h"
#include "barcore/barcore.h"
#include "barcore/filemanager.h"
#include <QString>

AuthCore *authCore;
FileManager *file_manager;

QString CASHBOX_DEVICE;
QString CASHBOX_TYPE;

#endif
