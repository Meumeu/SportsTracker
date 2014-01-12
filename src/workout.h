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
    explicit Workout(QObject *parent = 0);

    enum Status { NotStarted, Tracking, Paused, Stopped };

    Q_PROPERTY(double distance READ distance NOTIFY distanceChanged)
    Q_PROPERTY(double speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(double avgSpeed READ avgSpeed NOTIFY avgSpeedChanged)
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString lastPosition READ lastPosition NOTIFY lastPositionChanged)
    Q_PROPERTY(double duration READ duration NOTIFY durationChanged)

    Q_INVOKABLE void save();
    Q_INVOKABLE void reset();

    double distance() const;
    double speed() const;
    double avgSpeed() const;
    Status status() const;
    const QString& lastPosition() const;
    double duration() const;

    void setStatus(Status status);

private:
    gpx track;
    double _distance;
    double _speed;
    double _avgSpeed;
    Status _status;
    QString _lastPosition;
    QDateTime _start_date;
    QDateTime _pause_date;
    qint64 _duration;

    QTimer * _timer;
    QGeoPositionInfoSource * position_source;

    void addPoint(const QGeoPositionInfo&);
    void timerShot();
    void computeAvgSpeed();

signals:
    void distanceChanged(double distance);
    void speedChanged(double speed);
    void avgSpeedChanged(double avgSpeed);
    void statusChanged(Status status);
    void lastPositionChanged(const QString& position);
    void durationChanged(double duration);
    void saved(QString filename);

public slots:
};

#endif // WORKOUT_H
