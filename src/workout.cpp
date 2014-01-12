#include "workout.h"
#include <iostream>
#include <assert.h>
#include <QStringList>
#include <QGeoPositionInfoSource>


Workout::Workout(QObject *parent) :
    QObject(parent),
    _distance(0),
    _status(NotStarted),
    _start_date(QDateTime{}),
    _pause_date(QDateTime{}),
    _duration(0),
    _timer(new QTimer(this)),
    position_source(QGeoPositionInfoSource::createDefaultSource(this))
{
    if (position_source)
    {
        connect(position_source, &QGeoPositionInfoSource::positionUpdated, this, &Workout::addPoint);
        position_source->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        position_source->startUpdates();
    }

    _timer->setSingleShot(true);
    connect(_timer, &QTimer::timeout, this, &Workout::timerShot);
    connect(this, &Workout::durationChanged, this, &Workout::computeAvgSpeed);
    connect(this, &Workout::distanceChanged, this, &Workout::computeAvgSpeed);
}

void Workout::reset()
{
    _distance = 0;
    _status = NotStarted;
    track = gpx{};
    _start_date = QDateTime{};
    _pause_date = QDateTime{};
    _duration = 0;

    emit distanceChanged(_distance);
    emit statusChanged(_status);
    emit speedChanged(0);
    emit durationChanged(0);
}

void Workout::addPoint(const QGeoPositionInfo& position)
{
    if (position.isValid())
    {
        if (_status == Tracking)
        {
            track.addPoint(position);

            _distance = track.distance();
            emit distanceChanged(_distance);

            if (position.hasAttribute(QGeoPositionInfo::GroundSpeed))
            {
                _speed = position.attribute(QGeoPositionInfo::GroundSpeed);
            }
            else
            {
                // TODO
            }
            emit speedChanged(_speed);
        }

        _lastPosition = position.coordinate().toString();
        emit lastPositionChanged(_lastPosition);
    }
}

void Workout::timerShot()
{
    assert(_status == Tracking);

    QDateTime cur_date = QDateTime::currentDateTimeUtc();
    _duration += _pause_date.msecsTo(cur_date);
    _pause_date = cur_date;

    emit durationChanged(_duration * 0.001);

    int timeout = 1000 - _duration % 1000;
    _timer->start((timeout < 100) ? (timeout + 1000) : timeout);
}


void Workout::setStatus(Status status)
{
    if (status == _status) return;

    if (status == Tracking)
    {
        if (_status == NotStarted)
        {
            _start_date = QDateTime::currentDateTimeUtc();
            _pause_date = _start_date;
        }
        else if (_status == Paused)
        {
            _pause_date = QDateTime::currentDateTimeUtc();
        }

        _timer->start(1000 - _duration % 1000);
    }
    else if (status == Paused)
    {
        if (_status == Tracking)
        {
            _duration += _pause_date.msecsTo(QDateTime::currentDateTimeUtc());
            emit durationChanged(_duration * 0.001);
        }
        _timer->stop();
    }
    else if (status == Stopped)
    {
        if (_status == Paused || _status == Tracking)
        {
            _duration += _pause_date.msecsTo(QDateTime::currentDateTimeUtc());
            emit durationChanged(_duration * 0.001);
        }
        _timer->stop();
    }

    _status = status;
    emit statusChanged(status);
}

void Workout::computeAvgSpeed()
{
    _avgSpeed = _distance / (_duration * 0.001);
    emit avgSpeedChanged(_avgSpeed);
}

void Workout::save()
{
    if (_start_date.isValid())
    {
        emit saved(track.save(_start_date, _duration * 0.001));
    }
}

const QString& Workout::lastPosition() const
{
    return _lastPosition;
}

double Workout::distance() const
{
    return _distance;
}

double Workout::speed() const
{
    return _speed;
}

double Workout::avgSpeed() const
{
    return _avgSpeed;
}

Workout::Status Workout::status() const
{
    return _status;
}

double Workout::duration() const
{
    return _duration * 0.001;
}
