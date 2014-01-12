#ifndef GPX_H
#define GPX_H

#include <QDateTime>
#include <QtPositioning/QGeoPositionInfo>
#include <vector>

class gpx
{
    double _distance;
    typedef std::vector<QGeoPositionInfo> TrackSegment;
    std::vector<TrackSegment> track;
public:

    gpx();
    void addPoint(const QGeoPositionInfo& pt);
    double distance() { return _distance; }

    void save(QDateTime start_date);
};

#endif // GPX_H
