/***************************************************************************
                          printer.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/


#include <iostream>

extern "C"{
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
}
#define KB 1024
#define BUF 512 /* bytes */

using namespace std;

#include "printer.h"

#define TMP_PRINTER_CMNDS "/tmp/cmnds_for_printer"

Printer::Printer () {
    if (!(fd = tmpfile()))
        perror("failed while trying to open a tmp file for printer :");
}

Printer::~Printer(){
    fclose(fd);
}

void Printer::cmdBold(){}

void Printer::cmdItalic(){}

void Printer::cmdUnderline(){}

/* Turn of bold, italic, underline */
void Printer::cmdNormal(){}

void Printer::cmdFontSize(int /*width*/, int /*height*/) {}

void Printer::cmdInvertColors(int/* i*/){}

void Printer::cmdLineBreak(){
    char c[] = { 0x0a };
    put_bytes ("/dev/lp0", c, 1);
}

void Printer::cmdCut(){}

void Printer::string(const char *str){
    put_bytes("/dev/lp0", str, strlen(str)*sizeof(char));
}

void Printer::string(const QString& str){
    put_bytes ("", str.toLatin1(), str.length ());
}

void Printer::put_bytes (const QString& file, const char *c, int n){
    fwrite (c, sizeof(char), n, fd);
}

void Printer::print(const QString& file) {
    int readed;
    int printer_fd = -1;
    char buf[BUF];

    printer_fd = open("/dev/lp0", O_RDWR | O_NONBLOCK | O_SYNC);
    if (printer_fd < 0){
        perror ("Failed whe opening the printer");
        fclose(fd);
        if (!(fd = tmpfile()))
            perror("failed while trying to open a tmp file for printer :");
        return;
    }

    rewind(fd);
    while (!feof (fd)) {
        memset(buf, 0 , BUF);
        readed = fread(buf, sizeof (char), BUF, fd);
        write(printer_fd, buf, readed);
    }

    close(printer_fd);
    fclose(fd);
    if (!(fd = tmpfile()))
        perror("failed while trying to open a tmp file for printer :");
}

void Printer::clean(){}
