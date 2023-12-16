/***************************************************************************
                          optionnodedata.cpp  -  description
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

#include "productextrainfo.h"
#include <QString>

ProductExtraInfo::ProductExtraInfo(const QString option_type)
    :m_option_type(option_type)
{}

void ProductExtraInfo::addOption(
    const QString& option,
    bool is_default)
{
    m_option_values.append(option);
    if (is_default) setDefaultOption(option);
}

bool ProductExtraInfo::removeOption(const QString& option){
    int pos = optionPos(option);
    if (pos != -1){
        m_option_values.removeOne(m_option_values.at(pos));
        return true;
    }
    return false;
}

void ProductExtraInfo::setOptionType(const QString& option_type){
    m_option_type = option_type;
}

bool ProductExtraInfo::setDefaultOption(int pos){
    if ( pos >= m_option_values.count() || pos < 0 )
        return false;

    m_default_position = pos;
    return true;
}

bool ProductExtraInfo::contains(const QString& option){
    int i, count;
    count = m_option_values.count();
    for (i=0;i<count;i++){
        if (option == m_option_values[i])
            return true;
    }
    return false;

}

QStringList ProductExtraInfo::getValues(){
    return m_option_values;
}

QString ProductExtraInfo::getOptionType(){
    return m_option_type;
}

QString ProductExtraInfo::getDefaultOption(){
    if ( m_default_position == -1 &&  m_option_values.isEmpty() )
        return "";

    return m_option_values[m_default_position];
}

int ProductExtraInfo::getDefaultPos(){
    return m_default_position;
}

void ProductExtraInfo::setDefaultOption(const QString& option){
    m_default_position = optionPos(option);
}

int ProductExtraInfo::optionPos(const QString& option){
    return m_option_values.indexOf(option);
}
