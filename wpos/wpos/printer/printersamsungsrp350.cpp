/***************************************************************************
                          printersamsungsrp350.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include <iostream>
using namespace std;

#include "printersamsungsrp350.h"

PrinterSamsungSRP350::PrinterSamsungSRP350() :
    Printer(){}

PrinterSamsungSRP350::~PrinterSamsungSRP350(){}

void PrinterSamsungSRP350::cmdBold(){
    char c[] = { '\x1b', '\x45', '\xff' };

    put_bytes ("/dev/lp0", c, 3);
    //  stream.writeRawBytes (c, 3);
}

void PrinterSamsungSRP350::cmdItalic(){}

void PrinterSamsungSRP350::cmdUnderline(){
    char c[] = { 0x1b, 0x2d, 0x32 };

    put_bytes ("/dev/lp0", c, 3);
    //  stream.writeRawBytes (c, 3);
}

void PrinterSamsungSRP350::cmdNormal(){
    char c[] = { 0x1b, 0x45, 0x0, /* BOLD */
                 0x1b, 0x2d, 0x0  /* UNDERLINE */
               };

    put_bytes ("/dev/lp0", c, 6);
    //  stream.writeRawBytes (c, 6);
}

void PrinterSamsungSRP350::cmdFontSize(int width, int height) {
    char c[] = { 0x1d, 0x21, 0 };

    if (width > 8)
        width = 8;
    if (height > 8)
        height = 8;
    if (width < 0)
        width = 0;
    if (height < 0)
        height = 0;

    if (!width)
        width = 1;

    c[2] = (width-1) * 16;
    c[2] += height;

    put_bytes ("/dev/lp0", c, 3);
}

void PrinterSamsungSRP350::cmdInvertColors (int i) {
    char c[] = { '\x1d', '\x42', (char)i };
    put_bytes ("/dev/lp0", c, 3);

    //  stream.writeRawBytes (c, 3);
}

void PrinterSamsungSRP350::cmdCut() {
    char c[] = { 0x1d, 0x56, 0x1 };
    char e[] = { 0xc };

    put_bytes ("/dev/lp0", e, 1);
    put_bytes ("/dev/lp0", c, 3);
    put_bytes ("/dev/lp0", e, 1);
}
