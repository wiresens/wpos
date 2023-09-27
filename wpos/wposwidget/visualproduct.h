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

class VisualProduct {
public:
    VisualProduct();
    ~VisualProduct();

/**
* decremets all anchor which value is greater than max
*/
    QString name;
    QString description;
    QString options;
    float quantity;
    float price;
    int xmlAnchor;
};

#endif

