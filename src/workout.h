#ifndef WORKOUT_H
#define WORKOUT_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QTimer>

#include "gpx.h"

class QGeoPositionInfoSource;

class Workout : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)
public:
    enum Status { NotStarted, Tracking, Paused, Stopped };

private:
    gpx track;
    double _speed;
    double _avgSpeed;
    Status _status;
    QGeoPositionInfo _lastPosition;
    QDateTime _pause_date;
    qint64 _duration;
    QString _sport;

    QTimer * _timer;
    QGeoPositionInfoSource * position_source;

public:
    explicit Workout(QObject *parent = 0);

    Q_PROPERTY(double distance READ distance NOTIFY distanceChanged)
    Q_PROPERTY(double speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(double avgSpeed READ avgSpeed NOTIFY avgSpeedChanged)
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QGeoCoordinate lastPosition READ lastPosition NOTIFY lastPositionChanged)
    Q_PROPERTY(double duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString sport READ sport WRITE setSport NOTIFY sportChanged)


    Q_INVOKABLE void save();
    Q_INVOKABLE void reset();

    double distance() const { return track.distance(); }
    double speed() const { return _speed; }
    double avgSpeed() const { return _avgSpeed; }
    Status status() const { return _status; }
    QGeoCoordinate lastPosition() const { return _lastPosition.coordinate(); }
    double duration() const { return _duration * 0.001; }
    const QString& sport() const { return _sport; }

    void setStatus(Status status);
    void setSport(const QString& _sport);

private:
    void addPoint(const QGeoPositionInfo&);
    void timerShot();
    void computeAvgSpeed();

signals:
    void distanceChanged(double distance);
    void speedChanged(double speed);
    void avgSpeedChanged(double avgSpeed);
    void statusChanged(Status status);
    void lastPositionChanged(QGeoCoordinate position);
    void durationChanged(double duration);
    void sportChanged(const QString& sport);
    void saved(QString filename);

public slots:
};

#endif // WORKOUT_H
