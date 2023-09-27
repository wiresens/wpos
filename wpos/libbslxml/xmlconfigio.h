/***************************************************************************
                          xmlconfigio.h  -  description
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

#ifndef XMLCONFIGIO_H
#define XMLCONFIGIO_H


#include <QFile>

extern "C" {
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
}

class QString;
class QFile;
class QDomDocument;
class QDomNode;

/**
  * This is the helper class that will deal with the I/O needed in the xmlconfig class
  * This class is private, the program won't need to deal with this class
  * Just to try to split the xmlconfig complexity a bit.
  *@author jordi@napsis.com
  */

class XmlConfigIO {
public: 
    static const uint XML_TAG_IDENTATION;
    explicit XmlConfigIO(const QString& file_name, QIODevice::OpenMode = QIODevice::ReadWrite);
    XmlConfigIO(QDomDocument* document, const QString& file_name, QIODevice::OpenMode = QIODevice::ReadWrite);
    ~XmlConfigIO();

    /*
  * Damnnn constructors, they don't return anything so we can't know if really we have read
  * something of a file or the file was corrupted or have no XML.
  * If the file is corrupted the tree can't be constructed, and all the methods will fail
  * to check if everything is OK we have this method
  * @return true if correct memory tree false if the object is inconsistent
  */
    bool ok();

    /**
    * Saves the xml data in a file named file and flush it to disk.
    * If the filename is null it will write the data to the same file it reads
    * it from. Note that the constructor of this object needs to be said
    * explicitely that you want to save data with IO_ReadWrite
    * @param file the file we want to save the XML data in
    * @return true if success or false otherwise
    */
    bool save(const QString& file_name);

    /**
    * return the name of the file  we have taken the data from
  * if we save the data in other file, this method will return the name
    * of the new file
    * In short, it returns the name of the representation of the data
    * in disk (surely not updated)
    * @return the name of the file
    */
    QString fileName() const;

    QDomDocument *domDocument() const;

    QIODevice::OpenMode openedMode() const ;

    QString toString() const;

    /**
  * Simply print in stdout what's in the xml right now, it prints the in memory representation
  * Just for debugging purposes, is a bit time consuming so better not leave in a real program
  */
    void debug(const QDomNode& section, uint ident = XML_TAG_IDENTATION) const;

private:

    /** This is the code that share the constructors, one constructor can't call
  * the other so we need this.
  * @param _file: the file with the xml
  * @param mode: the mode we open the file in
  * @return 0 if success
  */
    int init(const QString& file_name, QIODevice::OpenMode mode);


    /**
  * This method will delete all the files that match a filter in a dir
  * Now it's only used to delete all the possible temporal files that we left behind
  * This should not happend because when the xml
  */
    //  void deleteFiles(const QString& _dir, const QString& filter);


    /**
  * Will write the default xml header to this file, this header is writen when
  * a new file is done either temporal or real one. But never overwrite other's headers
  * @param f the file where we write to, this file must be opened and will remain open
  */
    inline void setXmlHeaderAndFooter(QFile &dev);

    QDomDocument* m_domDocument{};  //the document pointing to our data
    FILE* tmp_fd{};
    QFile dev;         //the file we are working with
    QIODevice::OpenMode mode;         //the mode in which we open the file
    bool has_tempory_file{false};  // Are we working with is tempory file or not
    bool is_xml_file_parsed_succesfully{false};
};

#endif
