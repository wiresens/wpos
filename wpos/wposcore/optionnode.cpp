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

#include "optionnode.h"

#include <QString>
#define UNKNOWN_OPTION "UNKNOWN_OPTION_TYPE"

#include <iostream>

using namespace std;

OptionNode::OptionNode(): m_default_position(-1), m_option_type("UNKNOWN_OPTION"){}

OptionNode::OptionNode(const QString option_type): m_default_position(-1), m_option_type(option_type){}

OptionNode::~OptionNode(){
    m_options.clear();
}

void OptionNode::addOption(const QString& option, bool is_default){
    m_options.append(option);
    if (is_default) setDefaultOption(option);
}

bool OptionNode::removeOption(const QString& option){
    int pos = optionPos(option);
    if (pos != -1){
        m_options.removeOne(m_options.at(pos));
        return true;
    }
    return false;
}

void OptionNode::setOptionType(const QString& option_type){
    m_option_type = option_type;
}

bool OptionNode::setDefaultOption(int pos){
    if ( pos >= m_options.count() || pos < 0 )
        return false;

    m_default_position = pos;
    return true;
}

bool OptionNode::contains(const QString& option){
    int i, count;
    count = m_options.count();
    for (i=0;i<count;i++){
        if (option == m_options[i])
            return true;
    }
    return false;

}

QStringList OptionNode::getValues(){
    return m_options;
}

QString OptionNode::getOptionType(){
    return m_option_type;
}

QString OptionNode::getDefaultOption(){
    if ( m_default_position == -1 &&  m_options.isEmpty() )
        return "";

    return m_options[m_default_position];
}

int OptionNode::getDefaultPos(){
    return m_default_position;
}

void OptionNode::setDefaultOption(const QString& option){
    m_default_position = optionPos(option);
}

int OptionNode::optionPos(const QString& option){
    return m_options.indexOf(option);
}
