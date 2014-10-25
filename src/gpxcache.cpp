#include "gpx.h"
#include <QDataStream>

static const QString build_date = __DATE__ " " __TIME__;

void gpx::serialize(QIODevice * file)
{
    QDataStream stream(file);
    stream.setVersion(QDataStream::Version::Qt_5_1);

    stream << build_date;

    stream << _distance << _start_date << _sport << _duration << _track;
}

bool gpx::deserialize(QIODevice * file)
{
    QDataStream stream(file);
    QString tmp;
    stream.setVersion(QDataStream::Version::Qt_5_1);

    stream >> tmp;
    if (tmp != build_date) return false;

    stream >> _distance >> _start_date >> _sport >> _duration >> _track;

    return true;
}
