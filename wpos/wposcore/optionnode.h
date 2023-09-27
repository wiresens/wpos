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

#ifndef OPTIONNODE_H
#define OPTIONNODE_H

#include <QStringList>
/**
  *@author Carlos Manzanedo
  */
class QString;

class OptionNode{
public:
    OptionNode();
    explicit OptionNode(const QString option_type);
    ~OptionNode();

    void addOption(const QString& option, bool is_default=false);
    bool removeOption(const QString& option);
    void setOptionType(const QString& option_type);
    bool setDefaultOption(int pos);
    void setDefaultOption(const QString& option);
    bool contains(const QString& option);

    QStringList getValues();
    QString getOptionType();
    QString getDefaultOption();
    int getDefaultPos();

protected:
    int optionPos(const QString& option);

    QStringList m_options;
    int m_default_position;
    QString m_option_type;
};

#endif
