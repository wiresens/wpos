
/*
qutexrlib QT-based XML-RPC library
Copyright (C) 2003  P. Oscar Boykin <boykin@pobox.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Modified by Carlos Manzanedo <carlos@napsis.com>
Ported to Qt 5.12.8 by Gilles Bene Pougoue <gilles.benepougoue@gmail.com>

*/

#ifndef H_RPC_SERVER_IMP
#define H_RPC_SERVER_IMP

#include <QTextStream>
#include <QtNetwork/QtNetwork>
#include <QString>
#include <QList>
#include <QVariant>
#include <QRegExp>
#include <QtXml/QtXml>
#include <QMap>

#include "xrmethodcall.h"
#include "xrmethodresponse.h"
#include "xrfaultcodes.h"

/**
 * Provides a minimal HTTP Server to handle XML-RPC
 * requests and emit signals when requests are recieved.
 * This server implements only what is required for XML-RPC, with a
 * few extras.  In particular, connections are left open
 * (when compatible with the HTTP spec).  Leaving TCP
 * connections open results in usually 3 packets being sent
 * per methodCall (assuming small calls and responses), as
 * opposed to 10-12 if the connection is set up and
 * destroyed each time.
 * It also supports sending compressed (deflate) responses
 * which DRAMATICALLY reduces the size of XML-RPC responses.
 *
 * @see http://www.xmlrpc.com/spec
 * @see http://www.ietf.org/internet-drafts/draft-megacz-xmc-01.txt
 */

class AbstractSocket;
class RpcServerImp : public QObject {
    //Macro for QObject subclasses:
    Q_OBJECT

public:

    static const QString SERVER_NAME;
    static const unsigned int SERVER_DEFAULT_PORT;
    using Port = unsigned int;

    ~RpcServerImp() override;

    virtual void setMaxPendingConnections(int numConnections) = 0;
    virtual QString errorString() const = 0;
    virtual void close() =0;
//    virtual void serveNewConnection() = 0;
    virtual AbstractSocket* nextPendingConnection() = 0;

    virtual bool listen(const QHostAddress &address = QHostAddress::Any, Port port = SERVER_DEFAULT_PORT) = 0;
    virtual Port serverPort() =0;
    virtual QString serverName() =0;

signals:
    void methodCall(int req,
                    const QString& methodname,
                    const QList<QVariant>& params,
                    bool& handled);

    void newConnection();

public slots:
    /**
     * When a methodCall is handled with a successful
     * (non-fault) result, this slot is used to send the
     * response back to the caller.  Either this, or
     * XRServer::sendFault may be called, but not both.
     *
     *
     * @param req the request this is a response for
     * @param result the result of the methodcall.  If
     * the result cannot be converted to a XML-RPC type
     * (using XRVariant) an application fault is sent
     * (faultCode -32500).
     * @see methodCall
     * @see sendHttpResponse
     * @see XRVariant
     */
    void sendMethodResponse(int req, const QVariant& result);

    /**
     * When a methodCall is handled with an unsuccessful
     * (fault) result, this slot is used to send the
     * fault back to the caller.  Either this, or
     * XRServer::sendMethodResponse may be called, but not both.
     *
     * @param req the request this is a fault for
     * @param fault_code the number of the fault that
     * occured.
     * @param fault_string the string representation of
     * the fault
     * @see http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php
     * @see sendHttpResponse
     */
    void sendFault(int req_id,
                    int fault_code,
                    const QString& fault_string);

protected slots:
    /**
     * The constructor connects this slot to RpcSocket::readyRead()
     */
    void readFromClient();

    /**
     * The constructor connects this slot to RpcSocket::delayedCloseFinished()
     */

protected:
    RpcServerImp(Port port = RpcServerImp::SERVER_DEFAULT_PORT, QObject* parent = nullptr);
    void parseContentAndEmit(AbstractSocket* socket);
    void sendHttpError(AbstractSocket* s,
                       int status_code,
                       const QString& reason,
                       QString resp);

    void sendHttpResponse(AbstractSocket* s,
                          int status_code,
                          const QString& reason,
                          QNetworkRequest& request,
                          QString resp);
private:
    QString clientAddress(const AbstractSocket& client);

private:

    QTcpServer* rpc_server{};
    QMap<AbstractSocket*, QMap<QString, QVariant>> m_headers;
    QList<AbstractSocket*> m_open_sockets;
    QMap<int, AbstractSocket*> m_req_to_socket; //Keeps track of requests for each socket.

    int m_last_req_id{0}; //Counter that increases with each request.
    Port m_port;

};

#endif
