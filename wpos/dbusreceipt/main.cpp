/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Thu Jan  8 16:58:58 CET 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "dbusreceiptmanager.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    auto appPath = app.applicationDirPath();
    auto appName = QFileInfo(QFile(argv[0]).fileName()).baseName();

    QDir::setSearchPaths( "xmldocs",  QStringList( appPath ) );
    QDir::setSearchPaths( "dtddocs",  QStringList( appPath ) );
    app.setApplicationName(appName);

    DBusReceiptManager dbusreceipt(&app, app.applicationName());
    return app.exec();
}
