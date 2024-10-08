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

#ifndef xrclient
#define xrclient

#include <QUrl>
#include <QVariant>
#include <QtXml/QtXml>
#include <QtNetwork/QtNetwork>
#include <QMap>
#include <QBuffer>

#include "xrmethodcall.h"
#include "xrmethodresponse.h"
#include "xrfaultcodes.h"

/**
 * Represents the client which calls the methods on the remote machine.
 * @see http://www.xmlrpc.com/spec
 * @see http://www.ietf.org/internet-drafts/draft-megacz-xmc-01.txt
 */

class XRClient : public QObject {
    Q_OBJECT
public:

    static const QString USER_AGENT;
    static const int CONTENT_INDENTATION;
    static const size_t PLAINTEXT_MAX_SIZE;

    explicit XRClient(
        const QUrl& server_url,
        QObject* parent = nullptr,
        const QString &client_name = QString{}
    );

    ~XRClient();
    /**
     * Allow the server to send us compressed (deflate)
     * responses.  Useful to turn this off if debugging
     * with a network sniffing tool
     */
    void acceptCompressed(bool compressed) { m_accept_compressed = compressed; }

    /**
     * @param method the method on the server to call
	 * @param params a list of the parameters
	 * @return the identifier for this response
     */
    //    QNetworkReply* call(const QString& method, const QList<QVariant>& params);
    int call(const QString& method, const QList<QVariant>& params);


    /**
	 * @return the url of the XML-RPC server
	 */
    const QUrl& getUrl() const { return m_server_url; }
    void setUrl(const QUrl& server_url);

signals:
    /**
	 * this signal is emitted when a NON-FAULT
	 * XML-RPC response is recieved.  You must also
	 * watch for the fault signals
	 * @param response_num the response number for this fault
	 * @param response the deserialized response to the methodCall
	 * @see call
	 */
    void methodResponse(int response_num, const QVariant& response);

    /**
	 * @param response_num the response number for this fault
	 * @param fault_num the faultCode.
	 * @param fault_string the faultString
	 * @see http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php
	 * @see call
     */
    void fault(int response_num, int fault_num, const QString& fault_string);

public slots:
    /**
	 * this is slot which is useful for some GUI widgets
	 */
    void setUrl(const QString& url_str);

protected slots:
    void processHttpResponse(QNetworkReply* reply);

private:
    QString processXmlResponse(const QString& xml_response);
    bool isDeflated(const QNetworkReply& reply);
    //    QHttp _http_client; replaced by @networkManager

private:  
    QMap<QNetworkReply*, int> m_replies;
    QUrl m_server_url;
    QString m_client_name;
    QNetworkAccessManager* network_mgr {nullptr};
    int m_curr_req{0};
    bool m_is_deflated{true};
    bool m_accept_compressed{true};    
};

#endif
