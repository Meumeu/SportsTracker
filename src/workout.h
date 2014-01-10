#ifndef WORKOUT_H
#define WORKOUT_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QtPositioning/QGeoPositionInfo>

class Workout : public QObject
{
    Q_OBJECT
public:
    explicit Workout(QObject *parent = 0);

    struct Waypoint
    {
        double longitude;
        double latitude;
        double altitude;
        QDateTime timestamp;
    };

private:
    QVector<Waypoint> track;
    double distance;

signals:

public slots:
    void addPoint(const QGeoPositionInfo&);
};

#endif // WORKOUT_H
