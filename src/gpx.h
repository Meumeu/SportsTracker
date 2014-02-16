#ifndef GPX_H
#define GPX_H

#include <QDateTime>
#include <QGeoPositionInfo>
#include <QVector>

class QXmlStreamWriter;
class QXmlStreamReader;
class QIODevice;

class gpx
{
public:
    typedef QVector<QGeoPositionInfo> TrackSegment;
    typedef QVector<TrackSegment> Track;

private:
    Track _track;

    double _distance;
    QDateTime _start_date;
    QString _sport;
    double _duration;

    void serialize(QIODevice * file);
    bool deserialize(QIODevice * file);

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
