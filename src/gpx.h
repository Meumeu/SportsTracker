#ifndef GPX_H
#define GPX_H

#include <QDateTime>
#include <QtPositioning/QGeoPositionInfo>
#include <vector>

class QXmlStreamWriter;

class gpx
{
    double _distance;
    typedef std::vector<QGeoPositionInfo> TrackSegment;
    std::vector<TrackSegment> track;
public:

    gpx();
    void addPoint(const QGeoPositionInfo& pt);
    double distance() { return _distance; }

    QString save(QDateTime start_date, double duration);

    static double distance(const QGeoCoordinate& a, const QGeoCoordinate& b);

private:
    void saveWaypoint(QXmlStreamWriter& doc, const QGeoPositionInfo& pt);
};

#endif // GPX_H
