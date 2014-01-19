#ifndef GPX_H
#define GPX_H

#include <QDateTime>
#include <QGeoPositionInfo>
#include <vector>

class QXmlStreamWriter;
class QXmlStreamReader;

class gpx
{
public:
    typedef std::vector<QGeoPositionInfo> TrackSegment;
    typedef std::vector<TrackSegment> Track;

private:
    std::vector<TrackSegment> _track;

    double _distance;
    QDateTime _start_date;
    QString _sport;
    double _duration;

public:
    gpx();

    void setStartDate(QDateTime);
    void setSport(QString);
    void setDuration(double);
    void setTrack(const Track& track);

    double distance() const { return _distance; }
    QDateTime startDate() const { return _start_date; }
    QString sport() const { return _sport; }
    double duration() const { return _duration; }
    const Track& track() const { return _track; }


    void addPoint(const QGeoPositionInfo& pt);

    QString save();
    void load(const QString& filename);

    static double distance(QGeoCoordinate a, QGeoCoordinate b);
};

#endif // GPX_H
