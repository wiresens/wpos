#include "rpcencoder.h"

bool XmlPayloadFormater::buildMethodCall(
        const QString& content,
        QString& parseErrorMsg,
        QString& invalidMethodErrorMsg)
{
    invalidMethodErrorMsg.clear();
    bool parse_status = xmlcall.setContent(content, false, &parseErrorMsg);
    if( ! xmlcall.parseXmlRpc() ) invalidMethodErrorMsg = QString("Invalid XML-RPC methodCall");
    return parse_status;
}

void XmlPayloadFormater::buildMethodResponse(const QVariant& result){
    using std::swap;
    XRMethodResponse tmp_xmlresp(result);
    swap(xmlresp, tmp_xmlresp);
}

void XmlPayloadFormater::buildMethodResponse(int fault_code, const QString&  fault_string){
    using std::swap;
    XRMethodResponse tmp_xmlresp(fault_code, fault_string);
    swap(xmlresp, tmp_xmlresp);
}
