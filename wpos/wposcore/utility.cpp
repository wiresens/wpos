#include "utility.h"
#include <QDateTime>

static bool WithoutMs{ false };
QString currentDateTimeString(bool withms){
    auto dateTime = QDateTime::currentDateTime();
    if( withms )
        return dateTime.toString(Qt::ISODateWithMs).replace("T"," ");
    else
        return dateTime.toString(Qt::ISODate).replace("T"," ");
}

QString dateTimeString(const QDateTime& date_time, bool withms){
    Qt::DateFormat format;
    if(withms)
        format = Qt::ISODateWithMs;
    else
        format = Qt::ISODate;

    return date_time.toString(format).replace("T"," ");
}

QString dateTimeString(const QString& date_time_str, bool withms){
    Qt::DateFormat format;
    if(withms)
        format = Qt::ISODateWithMs;
    else
        format = Qt::ISODate;

    return QDateTime::fromString(date_time_str, Qt::ISODateWithMs).toString(format).replace("T"," ");
}
