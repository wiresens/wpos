/***************************************************************************
                          optionnodedata.h  -  description
                             -------------------
    begin                : jue jul 10 2003
    copyright            : (C) 2003 by Napsis S.L.
    email                : support@napsis.com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRODUCTEXTRAINFO_H
#define PRODUCTEXTRAINFO_H

#include <QStringList>
/**
  *@author Carlos Manzanedo
  */
class QString;

class ProductExtraInfo{
public:
    ProductExtraInfo();
    explicit ProductExtraInfo(const QString option_type);
    ~ProductExtraInfo();

    void addOption(const QString& option, bool is_default=false);
    bool removeOption(const QString& option);

    void setOptionType(const QString& option_type);
    bool setDefaultOption(int pos);
    void setDefaultOption(const QString& option);
    bool contains(const QString& option);

    QStringList getValues(); //@benes values
    QString getOptionType(); //@benes type
    QString getDefaultOption(); //@benes defautType()
    int getDefaultPos();        //@benes defaultPosition

protected:
    int optionPos(const QString& option);

    QString m_option_type;
    QStringList m_option_values;
    int m_default_position{-1};

};

#endif
