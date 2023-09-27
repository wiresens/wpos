#ifndef UTILITY_H
#define UTILITY_H

#include<QString>
class QDateTime;

QString currentDateTimeString(bool withms = true);
QString dateTimeString(const QDateTime& date_time, bool withms = true);
QString dateTimeString(const QString& date_time_str, bool withms = true);

#endif // UTILITY_H
