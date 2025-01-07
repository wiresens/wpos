/***************************************************************************
                          xmlconfigio.cpp  -  description
                             -------------------
    begin                : mar oct 8 2002
    copyright            : (C) 2002 by Napsis S.L.
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

#include "xmlconfigprivate.h"

#include <QtXml/QtXml>
#include <QFile>
#include <QString>
#include <QTextStream>

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

const uint XmlConfigPrivate::XML_TAG_IDENTATION  {4};

XmlConfigPrivate::XmlConfigPrivate(
    const QString& file_name,
    QIODevice::OpenMode mode)
{
    auto tmp_file_name = QFile(file_name).fileName();
    if ( init(tmp_file_name, mode) == -1 ){ //there's no opened file, no point continuing with the constructor
        cerr << "XmlConfigPrivate::XmlConfigPrivate errors happened in the constructor, exiting without xml loaded" << endl;
        m_qfile.close();
        return;
    }

    QString err_msg;
    int err_line{}, err_colm{};
    if ( !(m_xml_parsed_succesfully = m_dom.setContent( &m_qfile, &err_msg, &err_line, &err_colm )) ) {
        cerr << "Parse error in XML file : " << tmp_file_name.toStdString() << endl;
        cerr <<  err_msg.toStdString() << " Line: " << err_line << ", Col: " << err_colm << endl;
    }
    m_qfile.close();
}

XmlConfigPrivate::XmlConfigPrivate(
    const QDomDocument &document,
    const QString& file_name,
    QIODevice::OpenMode mode):
    XmlConfigPrivate(file_name, mode)
{
    m_dom.setContent( document.cloneNode(true).toDocument().toString());
}

//the private shared constructor
int XmlConfigPrivate::init(
    const QString& file_name,
    QIODevice::OpenMode mode)
{

    //if we open in writeonly mode , bad things (tmp) will happen
    if (mode == QIODevice::WriteOnly)  mode = QIODevice::ReadWrite;

    //the file really doesn't exist, we'll use a tempory file instead
    if ( file_name.isEmpty() ){
        m_file = tmpfile();
        if (!m_file){
            perror("XML initialisation: Problems while trying to create a tempory file");
            exit(-1);
        }

        if ( ! m_qfile.open(m_file , mode | QIODevice::Text) ){
            cerr << "XML initialisation: Unable to open the tempory file" << endl;
            exit(-1);
        }

        setXmlHeaderAndFooter(m_qfile);
        m_has_tempory_file = true;
    }
    else{ //a file name has been given.
        m_qfile.setFileName(file_name);
        if (!m_qfile.exists()){    //the file with path to file does not exists
            if(!m_qfile.open(QIODevice::ReadWrite | QIODevice::Text)){
                cerr << "XML initialisation: Unable to create a new file "
                    << file_name.toStdString()
                    << " in the mode "
                    << QFlags<QIODevice::OpenModeFlag>(mode) << endl;
                return -1;
            }
            setXmlHeaderAndFooter(m_qfile);
        }
        else{ // the file exists, so we'll try to open it in the mode specified
            if ( !m_qfile.open( mode | QIODevice::Text) ){
                cerr << "XML initialisation: Unable to create a new file "
                    << file_name.toStdString() \
                    << " in the mode " \
                    << QFlags<QIODevice::OpenModeFlag>(mode) << endl;
                return -1;
            }
        }
    }
    m_mode = m_qfile.openMode();
    return 0;
}

XmlConfigPrivate::~XmlConfigPrivate(){
    if (m_file)
        fclose(m_file);
    else
        m_qfile.close();
}

bool XmlConfigPrivate::ok(){
    return m_xml_parsed_succesfully;
}

bool XmlConfigPrivate::save(const QString& file_name){
    if (!file_name.isEmpty()) m_qfile.setFileName(file_name);

    if (m_has_tempory_file){
        fclose(m_file);
        m_has_tempory_file = false; //now there's no a tempory file but a real one
        m_file = nullptr;
    }

    if( !m_qfile.open( QIODevice::WriteOnly )){
        qDebug() << "XmlConfigPrivate::save() : Unable to save XML file "
            <<  m_qfile.fileName()
            << "\nI/O error : " <<  m_qfile.errorString();
        return false;
    }

    QTextStream stream(&m_qfile);
    m_dom.save(stream, XML_TAG_IDENTATION);
    qDebug() << "Succesfully saved " << m_qfile.fileName();

    m_qfile.close();
    return true;
}

QString XmlConfigPrivate::fileName() const{
    return m_qfile.fileName();
}

QString XmlConfigPrivate::toString() const{
    return m_dom.toString();
}

void XmlConfigPrivate::debug(const QDomNode& section, uint ident) const{

    uint identation{XML_TAG_IDENTATION};
    if (ident )  identation = ident;

    QTextStream stream(stdout, QIODevice::WriteOnly);
    section.save(stream, identation);
    cout << endl;
}

const QDomDocument& XmlConfigPrivate::domDocument() const {
    return m_dom;
}

QIODevice::OpenMode XmlConfigPrivate::openedMode() const{
    return m_qfile.openMode();
}

void XmlConfigPrivate::setXmlHeaderAndFooter(QFile &file){
    QTextStream stream(&file);
    stream << "<?xml version='1.0' encoding='UTF-8'?> \n<wpos version='2.0.0-rc1'>\n" ; // xml header
    stream << "</wpos>"; // xml footer

    //rewind the file to the initial pos so the info just written can be read
    if(!stream.seek(0))
        cerr << "We can't get to the beggining of the file" << endl;
}
