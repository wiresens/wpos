/***************************************************************************
                          linuxnight.h  -  description
                             -------------------
    begin                : jun 25 11:17:52 CEST 2003
    copyright            : (C) 2003 by BS Team
    email                : bsteam@support.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VISUALPRODUCT_H
#define VISUALPRODUCT_H

#include <QString>

// decremets all anchor which value is greater than max
struct VisualProduct {
    QString name;
    QString description;
    QString options;
    float quantity{0.0};
    float price{0.0};
    int xmlAnchor{};
};

#endif

