#include "gpx.h"
#include <iostream>
#include <stdexcept>
#include <assert.h>
#include <math.h>

typedef std::tuple<double, double, double> vector3;

static vector3 wgs84_to_ecef(double lat, double lon, double alt)
{
    double a = 6378137;
    double e2 = 6.69437999014E-3;

    double cos_lat = cos(lat * M_PI / 180);
    double sin_lat = sin(lat * M_PI / 180);
    double cos_lon = cos(lon * M_PI / 180);
    double sin_lon = sin(lon * M_PI / 180);

    double N = a / sqrt(1 - e2 * sin_lat * sin_lat);

    return vector3{
        (N + alt) * cos_lat * cos_lon,
        (N + alt) * cos_lat * sin_lon,
        (N * (1 - e2) + alt) * sin_lat
    };
}

static vector3 wgs84_to_ecef(const QGeoCoordinate& coord)
{
    return wgs84_to_ecef(coord.latitude(), coord.longitude(), coord.altitude());
}

static double distance(const vector3& a, const vector3& b)
{
    double x = std::get<0>(a) - std::get<0>(b);
    double y = std::get<1>(a) - std::get<1>(b);
    double z = std::get<2>(a) - std::get<2>(b);

    return sqrt(x * x + y * y + z * z);
}

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
