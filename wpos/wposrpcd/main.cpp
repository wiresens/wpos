#include "validationserver.h"
#include "rpcserver.h"
#include "rpcencoder.h"

#include <QCoreApplication>

int main(int argc, char* argv[]){

    QCoreApplication app(argc, argv, false);
    XmlPayloadFormater encoder;
    // RpcServer server{new TcpRpcServer(encoder, TcpRpcServer::SERVER_DEFAULT_PORT) };
    RpcServer server{new XRValidationServer(encoder, XRValidationServer::SERVER_DEFAULT_PORT)};

    if( server.start() )
        return app.exec();
    return EXIT_FAILURE;
}
