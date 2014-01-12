#ifndef GPX_H
#define GPX_H

#include <QDateTime>
#include <QDomDocument>
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

    QString save(QDateTime start_date, double duration);

private:
    QDomDocument * _doc;
    void saveTrackSegment(QDomElement& trk, const TrackSegment& segment);
    void saveWaypoint(QDomElement& trkseg, const QGeoPositionInfo& pt);
    void saveMetadataExt(QDomElement& metadataExt, const QString& name, const QString& value);
    void saveMetadata(QDomElement& metadata, const QString& name, const QString& value);
};

#endif // GPX_H
