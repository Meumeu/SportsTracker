#include "workout.h"

Workout::Workout(QObject *parent) :
    QObject(parent),
    distance(0)
{
}

void Workout::addPoint(const QGeoPositionInfo& position)
{
    Waypoint wpt;
    wpt.longitude = position.coordinate().longitude();
    wpt.latitude = position.coordinate().latitude();
    wpt.altitude = position.coordinate().altitude();
    wpt.timestamp = position.timestamp();

    track.push_back(wpt);
}
