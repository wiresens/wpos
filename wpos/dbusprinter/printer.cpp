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

#include "printer.h"

#include <QFile>
#include <QTextStream>
#include <string>
#include <iostream>

extern "C"{
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#ifndef _WINDOWS
    #include <cups/cups.h>
    #include <cups/ipp.h>
#endif
}

#define KB 1024
#define BUF_SIZE 4*KB /* bytes */
#define DEFAULT_TYPE "direct"
#define DEFAULT_DEVICE "/dev/lp0"

using namespace std;

extern QString PRINTER_DEVICE;
extern QString PRINTER_NAME;
extern QString PRINTER_SYSTEM;

Printer::Printer (const QString& _type,
                  const QString& _device,
                  const QString& _user,
                  const QString& _passwd):
    type {DEFAULT_TYPE},
    device {DEFAULT_DEVICE}
{
    if (_type == "ipp") type = _type;
    if (! _device.isEmpty() ) device = _device;

    user = _user;
    passwd = _passwd;

    if (!(fd = tmpfile()))
        perror("Failed while trying to open a tmp file for printer :");
}

Printer::~Printer(){
    fclose(fd);
}

void Printer::cmdLineBreak () {
    char c[] = { 0x0a };
    put_bytes (PRINTER_DEVICE.toStdString().c_str(), c, 1);
}


void Printer::string(const char *str) {
    put_bytes (PRINTER_DEVICE.toStdString().c_str(), str, strlen(str)*sizeof(char));
}

void Printer::string(const QString& str) {
    put_bytes (PRINTER_DEVICE.toStdString().c_str(), str.toLatin1(), str.length ());
}

void Printer::put_bytes (const QString& file, const char *c, int n) {
    fwrite (c, sizeof (char), n, fd);
}

void Printer::print(const QString& file) {
    if (type == "direct")    printDirect(file);
    else if (type == "ipp")  printCups(file);
}

void Printer::printDirect(const QString& /*_file*/){
    int readed;
    int printer_fd = -1;
    char buf[BUF_SIZE];

#ifndef _WINDOWS
    printer_fd = open(PRINTER_DEVICE.toStdString().c_str(), O_RDWR | O_SYNC);
#endif

    if ( !printer_fd ){
        perror ("Failed when opening the printer");
        fclose(fd);
        if ( !(fd = tmpfile()) )
            perror("Failed while trying to open a tmp file for printer :");
        return;
    }

    rewind(fd);

    while (!feof (fd)) {
        memset (buf, 0, BUF_SIZE);
        readed = fread (buf, sizeof (char), BUF_SIZE, fd);
#ifndef _WINDOWS
        write(printer_fd, buf, readed);
#endif
        cout << buf << endl;  //Added for debug purpose
    }

#ifndef _WINDOWS
    close(printer_fd);
#endif
    fclose(fd);

    if ( !(fd = tmpfile() ) )
        perror("Failed while trying to open a tmp file for printer :");
}

void Printer::printCups(const QString& /*_file*/){
    QString TMP_PRINTER_FILE {"tmps:ntpv_printer_file.tmp"};
    QFile qfile(TMP_PRINTER_FILE);
    if (qfile.exists())  qfile.remove();

    if ( ! qfile.open(QIODevice::WriteOnly) ){
        fclose(fd);
        if ( !(fd = tmpfile() ) )
            perror("Failed while trying to open a tmp file for Printer :");
        return;
    }

    rewind(fd);

    char buf[BUF_SIZE];
    while ( !feof(fd) ) {
        memset (buf, 0, BUF_SIZE);
        fread (buf, sizeof(char), BUF_SIZE, fd);
        qfile.write(buf, BUF_SIZE);
        cout << buf << endl;  //Added for debug purpose
    }
    fclose(fd);
    if ( !(fd = tmpfile()) )
        perror("failed while trying to open a tmp file for printer :");

    qfile.close();

    auto pos = device.indexOf('@');
    QString tmp, printer_name;
    if ( pos ){
        tmp = device.mid( pos + 1, device.length()- pos );
#ifndef _WINDOWS
        cupsSetServer(tmp.toStdString().c_str());
#endif
        printer_name = device.mid(0, pos);
    }
    else
        printer_name = device;

    if (!user.isEmpty())
#ifndef _WINDOWS
        cupsSetUser(user.toStdString().c_str())
#endif
        ;

#ifndef _WINDOWS
    if ( cupsPrintFile( printer_name.toStdString().c_str(), TMP_PRINTER_FILE.toStdString().c_str(), "ntpv_ticket", 0, 0) <= 0 ){
         ipp_status_t status {cupsLastError()};
        cerr << "problems trying to print wPOS ticket : " << ippErrorString(status) << endl;
    }
#endif

    qfile.remove();
}
