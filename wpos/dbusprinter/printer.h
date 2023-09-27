/***************************************************************************
                          printer.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRINTER_H
#define PRINTER_H

#include <QString>

class Printer {
public: 
    Printer (const QString& type,
             const QString& device,
             const QString& user="",
             const QString& passwd="");

    virtual ~Printer();

    virtual void cmdBold (){}
    virtual void cmdItalic (){}
    virtual void cmdUnderline (){}
    virtual void cmdNormal (){}
    virtual void cmdFontSize (int width, int height){}
    virtual void cmdLineBreak ();
    virtual void cmdInvertColors (int i){}
    virtual void cmdCut(){}
    virtual void clean (){}

    virtual void string(const char *str);
    virtual void string(const QString& str);

    virtual void put_bytes (const QString& file, const char *c, int n);

    virtual void print (const QString& file);
    virtual void printDirect(const QString&);
    virtual void printCups(const QString& file);


protected:
    FILE *fd{};

    QString type;
    QString device;
    QString user;
    QString passwd;
};

#endif
