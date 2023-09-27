
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
class PayloadFormater;

class RpcServerImp : public QObject {
    Q_OBJECT

public:
    static const QString SERVER_NAME;
    using Port = unsigned int;
    static const Port SERVER_DEFAULT_PORT;
    static const size_t PLAINTEXT_MAX_SIZE;

    ~RpcServerImp() override;

    virtual void serveNewConnection();
    virtual bool listen(const QHostAddress &address = QHostAddress::Any,
                        Port port = SERVER_DEFAULT_PORT) = 0;

    virtual void setMaxPendingConnections(int numConnections) = 0;
    virtual AbstractSocket* nextPendingConnection() = 0;

    virtual void close() =0;
    virtual Port serverPort() =0;
    virtual QString serverName() =0;
    virtual QString errorString() const = 0;

signals:
    void methodCall(int req, const QString& methodname,
                    const QList<QVariant>& params, bool& handled);
    void newConnection();

public slots:
    /**
     * When a methodCall is handled with a successful
     * (non-fault) result, this slot is used to send the
     * response back to the caller.  Either this, or
     * RpcServerImp::sendFault may be called, but not both.
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
    void sendMethodResponse(int req_id, const QVariant& result);

    /**
     * When a methodCall is handled with an unsuccessful
     * (fault) result, this slot is used to send the
     * fault back to the caller.  Either this, or
     * RpcServerImp::sendMethodResponse may be called, but not both.
     *
     * @param req the request this is a fault for
     * @param fault_code the number of the fault that
     * occured.
     * @param fault_string the string representation of
     * the fault
     * @see http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php
     * @see sendHttpResponse
     */
    void sendFault(int req_id, int fault_code, const QString& fault_string);

protected slots:
    virtual void readFromClient();
    virtual void dropClient();
    virtual void showSocketState(QAbstractSocket::SocketState state);

protected:
    explicit RpcServerImp(QObject* parent = nullptr);
    virtual PayloadFormater& formater() = 0;

    void parseContentAndEmit(AbstractSocket* socket);
    void sendHttpError(AbstractSocket* s, int status_code,
                       const QString& reason, QString resp);
    void sendHttpResponse(AbstractSocket* s, int status_code,
                          const QString& reason, QNetworkRequest& request, QString resp);

    void showRequestHeader(AbstractSocket& socket) const;
    void showReplyHeader(const QNetworkRequest& reply, const AbstractSocket& socket) const;
    void showReplyHeader(const QByteArray& headerList, const AbstractSocket& socket) const;
    void showPayload(const QString& payload, const AbstractSocket& socket ) const;

//    template <typename T>
//    concept IsSocket = requires(T socket){
//        socket.peerAddress();
//        socket.peerPort();
//    }

//    template< IsSocket Socket> QString clientAddress(const Socket& socket) const{
//        return std::move( socket.peerAddress().toString().replace("::ffff:","") +":"+ QString::number(socket.peerPort()));
//    }

    //    QString clientAddress(const AbstractSocket& client);

    template< typename Socket> QString clientAddress(const Socket& socket) const{
        return std::move( socket.peerAddress().toString().replace("::ffff:","") +":"+ QString::number(socket.peerPort()));
    }

    bool isDeflated(AbstractSocket* socket) const;
    void cleanCache(AbstractSocket* socket);

private:
    QMap<AbstractSocket*, QMap<QString, QVariant>> m_headers;
    QMap<int, AbstractSocket*> m_req_to_socket; //Keeps track of requests for each socket.
    int m_next_req_id{0}; //Counter that increases with each request.
};

#endif
