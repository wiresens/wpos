/***************************************************************************
                          printersamsungsrp350.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRINTERSAMSUNGSRP350_H
#define PRINTERSAMSUNGSRP350_H

#include "printer.h"

class PrinterSamsungSRP350 : public Printer  {
public: 
    PrinterSamsungSRP350(const QString& _type,
                         const QString& _device,
                         const QString& _user="",
                         const QString& _passwd="");

    ~PrinterSamsungSRP350();

    void cmdBold ();
    void cmdUnderline ();
    void cmdNormal ();

    void cmdFontSize (int width, int height);
    void cmdInvertColors (int i);
    void cmdCut();
};

#endif
