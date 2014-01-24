#include "workoutdetails.h"
#include "gpx.h"
#include "qmlplotdata.h"

#include <QQuickPaintedItem>
#include <iostream>

WorkoutDetails::WorkoutDetails(QObject *parent) :
    QObject(parent),
    _speed(new QmlPlotData(this)),
    _altitude(new QmlPlotData(this))
{
}

void WorkoutDetails::setFilename(QString filename)
{
    _data.load(filename);

    std::vector<QVector2D> speed_data;
    std::vector<QVector2D> altitude_data;

    const gpx::Track& track = _data.track();

    float maxspeed = 0;

    float duration = 0;
    int n = 0;
    for(const gpx::TrackSegment& i: track)
    {
        ++n;
        QGeoPositionInfo x0 = i[0];
        QDateTime t0 = x0.timestamp();

        int m = 0;
        for(const QGeoPositionInfo& j: i)
        {
            ++m;
            float t = duration + t0.msecsTo(j.timestamp()) / 1000.0;
            std::cerr << "t=" << j.timestamp().toString().toStdString()<< ", " << t << std::endl;

            if (j.hasAttribute(QGeoPositionInfo::GroundSpeed))
            {
                speed_data.emplace_back(t, j.attribute(QGeoPositionInfo::GroundSpeed));
                maxspeed = std::max<float>(maxspeed, j.attribute(QGeoPositionInfo::GroundSpeed));
            }

            if (j.coordinate().type() == QGeoCoordinate::Coordinate3D)
            {
                altitude_data.emplace_back(t, j.coordinate().altitude());
            }
        }

        std::cerr << m << " points" << std::endl;
        duration += t0.msecsTo(i.back().timestamp()) / 1000;
    }

    std::cerr << n << " track segment(s)" << std::endl;
    std::cerr << "Max speed: " << maxspeed << " m/s" << std::endl;

    _speed->setData(speed_data);
    _altitude->setData(altitude_data);
    _filename = filename;

    emit speedChanged(_speed);
    emit altitudeChanged(_altitude);
    emit filenameChanged(filename);

    QQuickPaintedItem * _parent = qobject_cast<QQuickPaintedItem *>(parent());
    if (_parent)
    {
        std::cerr << "updating parent" << std::endl;
        _parent->update();
    }
}
