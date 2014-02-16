#include "gpx.h"
#include <QDataStream>

static const int current_version = 1;

void gpx::serialize(QIODevice * file)
{
    QDataStream stream(file);

    stream << current_version << _distance << _start_date << _sport << _duration << _track;
}

bool gpx::deserialize(QIODevice * file)
{
    QDataStream stream(file);
    int ver;
    stream >> ver;
    if (ver != current_version)
        return false;

    stream >> _distance >> _start_date >> _sport >> _duration >> _track;

    return true;
}
