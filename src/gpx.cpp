#include "gpx.h"
#include <iostream>
#include <stdexcept>
#include <assert.h>
#include <math.h>
#include "geodesy.h"


void gpx::setStartDate(QDateTime t) { _start_date = t; }
void gpx::setSport(QString s) { _sport = s; }
void gpx::setDuration(double d) { _duration = d; }

void gpx::setTrack(const Track& track)
{
    _track = track;
    _distance = 0;
    for(const TrackSegment& i: _track)
    {
        for(ssize_t j = 1; j < i.size(); ++j)
        {
            _distance += distance(i[j-1].coordinate(), i[j].coordinate());
        }
    }
}

double gpx::distance(QGeoCoordinate a, QGeoCoordinate b)
{
    if (a.type() != QGeoCoordinate::Coordinate3D || b.type() != QGeoCoordinate::Coordinate3D)
    {
        a.setAltitude(0);
        b.setAltitude(0);
    }

    return ::distance(wgs84_to_ecef(a), wgs84_to_ecef(b));
}

gpx::gpx() : _distance(0)
{
}

void gpx::addPoint(const QGeoPositionInfo &pt)
{
    if (!pt.timestamp().isValid()) return;

    if (_track.empty())
        _track.push_back(TrackSegment{});

    if (!_track.back().empty())
    {
        if (_track.back().back().timestamp().msecsTo(pt.timestamp()) > 5000)
            _track.push_back(TrackSegment{});
    }

    if (!_track.back().empty())
    {
        _distance += distance(_track.back().back().coordinate(), pt.coordinate());
    }

    std::cerr << "Added " << pt.coordinate().toString().toStdString() << std::endl;
    _track.back().push_back(pt);
}
