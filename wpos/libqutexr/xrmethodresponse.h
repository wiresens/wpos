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

#ifndef xrmethodresponse
#define xrmethodresponse

#include <QString>
#include <QList>
#include <QVariant>
#include <QtXml/QtXml>

#include "xrvariant.h"

/**
 * Represents the XML structure for XML-RPC method responses.
 * Handles serialization and deserialization through QDomDocument.
 *
 */

class XRMethodResponse : public QDomDocument {

public:
    /**
     * Creates an empty XRMethodResponse.  This is for use
     * with the QDomDocument methods for initializing from
     * XML, particularly QDomDocument::setContent.
     */
    XRMethodResponse() = default;
    /**
     * Creates a non-fault result.
     * @param result the param for this result
     */
    explicit XRMethodResponse(const QVariant& result);
    /**
     * Create a fault result
     * @param faultCode the code for the fault.
     * @param faultString the string for the fault.
     * @see http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php
     */
    XRMethodResponse(int faultCode, const QString& str);

    /**
     * This must be called after using any QDomDocument setting
     * methods.
     * @return true if the underlying QDomDocument is a valid xmlrpc message
     */
    bool parseXmlRpc();

    /**
      * @param faultCode gets set to the fault code if return is true
     * @param faultString gets set to the fault string if return is true
     * @return true if this response is a fault
     */
    bool getFault(int& fault_code, QString& fault_string) const;

    /**
     * @return the NON-FAULT response.  If this is a fault, this
     * returns the null QVariant.
     */
    const QVariant& getResponse() const { return _response; }
protected:

    static void faultToDomDoc(int fault_code,
                              const QString& fault_string,
                              QDomDocument& doc);

    static bool fromDomDoc(const QDomDocument& doc,
                           QVariant& result,
                           bool& isFault);


    /* This is NOT a fault */
    static void responseToDomDoc(const QVariant& result,
                                 QDomDocument& doc);

    QVariant _response;
    QString _fault_string;
    int _fault_code{0};
    bool _is_fault{false};

};

#endif
