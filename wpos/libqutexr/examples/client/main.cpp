#include "../../xrclient.h"
#include <iostream>
#include <QApplication>
#include "clientimpl.h"

int main(int argc, char* argv[]){
    QApplication app(argc,argv);
    ClientImpl client;
    client.show();
    return app.exec();
}
