#include "workoutsummary.h"
#include "gpx.h"
#include <QDir>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QGeoPositionInfo>
#include <QtDebug>
#include <iostream>
#include <stdexcept>

void WorkoutSummary::setSport(const QString & sport)
{
    _sport = sport;
}

void WorkoutSummary::setDate(QDateTime date)
{
    _date = date;
}

void WorkoutSummary::setDistance(double distance)
{
    _distance = distance;
}

void WorkoutSummary::setDuration(double time)
{
    _duration = time;
}

WorkoutSummary::WorkoutSummary(QString filename) :
    _filename(filename)
{
    // TODO: cache
    gpx tmp;
    tmp.load(filename);

    _sport = tmp.sport();
    _date = tmp.startDate();
    _distance = tmp.distance();
    _duration = tmp.duration();
}

WorkoutSummary::WorkoutSummary() :
    _filename(),
    _sport(),
    _date(),
    _distance(0),
    _duration(0)
{
}


