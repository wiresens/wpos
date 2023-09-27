#include "validationserver.h"

XRValidationServer::XRValidationServer(
        PayloadFormater& encoder,
        Port port,
        QObject* parent):
    TcpRpcServer(encoder, port, parent)
{
    /*
   * This looks horrific, but it is really simple (just long).
   * This wires up All slots to the methodCall signal.
   */

    connect(this, &TcpRpcServer::methodCall, this, &XRValidationServer::arrayOfStructsTest);
    connect(this, &TcpRpcServer::methodCall, this, &XRValidationServer::countTheEntities);
    connect(this, &TcpRpcServer::methodCall, this, &XRValidationServer::easyStructTest);
    connect(this, &TcpRpcServer::methodCall, this, &XRValidationServer::echoStructTest);
    connect(this, &TcpRpcServer::methodCall, this, &XRValidationServer::manyTypesTest);
    connect(this, &TcpRpcServer::methodCall, this, &XRValidationServer::moderateSizeArrayCheck);
    connect(this, &TcpRpcServer::methodCall, this, &XRValidationServer::nestedStructTest);
    connect(this, &TcpRpcServer::methodCall, this, &XRValidationServer::simpleStructReturnTest);
    connect(this, &TcpRpcServer::methodCall, this, &XRValidationServer::sumAndDifference);
}

void XRValidationServer::arrayOfStructsTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported)
{

    if( ! is_method_supported && methodname == "validator1.arrayOfStructsTest" )
    {
        is_method_supported = true;
        //Here we through caution to the wind, assume all the params are okay
        QList<QVariant> array = params[0].toList().at(0).toList();
        QMap<QString, QVariant> map;
        int result = 0;

        for(auto elt : array) {
            map = elt.toMap();
            if( map.contains("curly") )
                result += map["curly"].toInt();
        }
        sendMethodResponse(req, QVariant(result));
    }
}

void XRValidationServer::countTheEntities(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported)
{
    if( !is_method_supported  && methodname == "validator1.countTheEntities" )
    {
        is_method_supported = true;
        QMap<QString, QVariant> result;
        QString input = params[0].toList().at(0).toString();

        result["ctLeftAngleBrackets"] = input.count('<');
        result["ctRightAngleBrackets"] = input.count('>');
        result["ctAmpersands"] = input.count('&');
        result["ctApostrophes"] = input.count('\'');
        result["ctQuotes"] = input.count('"');
        sendMethodResponse(req,QVariant(result));
    }
}

void XRValidationServer::easyStructTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported)
{
    if(  ! is_method_supported  && methodname == "validator1.easyStructTest" )
    {
        is_method_supported = true;
        int result = 0;
        QMap<QString, QVariant> map = params[0].toList().at(0).toMap();

        result += map["larry"].toInt();
        result += map["moe"].toInt();
        result += map["curly"].toInt();
        sendMethodResponse(req, QVariant(result));
    }
}

void XRValidationServer::echoStructTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported)
{
    if( !is_method_supported && methodname == "validator1.echoStructTest" )
    {
        is_method_supported = true;
        sendMethodResponse(req, QVariant( params[0]));
    }
}

void XRValidationServer::manyTypesTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported)
{
    if( ! is_method_supported && methodname == "validator1.manyTypesTest" )
    {
        is_method_supported = true;
        sendMethodResponse(req, QVariant( params[0] ) );
    }
}

void XRValidationServer::moderateSizeArrayCheck(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported)
{
    if( ! is_method_supported && methodname == "validator1.moderateSizeArrayCheck" )
    {
        is_method_supported = true;
        QList<QVariant> array = params[0].toList().at(0).toList();
        QString result = array.front().toString() + array.back().toString();
        sendMethodResponse(req, QVariant( result ) );
    }
}

void XRValidationServer::nestedStructTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported)
{
    if( ! is_method_supported && methodname == "validator1.nestedStructTest" )
    {
        is_method_supported = true;
        QMap<QString, QVariant> years, months, days, day;
        years = params[0].toList().at(0).toMap();
        months = years["2000"].toMap();
        days = months["04"].toMap();
        day = days["01"].toMap();

        int result = 0;
        result += day["larry"].toInt();
        result += day["moe"].toInt();
        result += day["curly"].toInt();

        sendMethodResponse(req, QVariant( result ) );
    }
}

void XRValidationServer::simpleStructReturnTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported)
{
    if( !is_method_supported && methodname == "validator1.simpleStructReturnTest")
    {
        is_method_supported = true;
        QMap<QString, QVariant> map;
        auto val = params[0].toList().at(0).toInt();
        map["times10"] = val * 10;
        map["times100"] = val * 100;
        map["times1000"] = val * 1000;

        sendMethodResponse(req, QVariant( map ) );
    }
}

void XRValidationServer::sumAndDifference(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported)
{
    if( !is_method_supported && methodname == "sample.sumAndDifference")
    {
        is_method_supported = true;
        auto args = params[0].toList();
        int a = args[0].toInt();
        int b = args[1].toInt();

        QMap<QString, QVariant> result;
        result["difference"] = a - b;
        result["sum"] = a+b;
        sendMethodResponse(req, QVariant( result ) );
    }
}
