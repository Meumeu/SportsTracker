#include "workoutdetails.h"
#include "gpx.h"
#include "qmlplotdata.h"

#include <memory>
#include <QQuickPaintedItem>
#include <QThread>
#include <QThreadPool>

WorkoutDetails::WorkoutDetails(QObject *parent) :
    QObject(parent),
    _speed(new QmlPlotData(this)),
    _altitude(new QmlPlotData(this)),
    _loading(true)
{
    _speed->setAxisId(QmlPlotData::Left);
    _altitude->setAxisId(QmlPlotData::Right);
}

void WorkoutDetails::setFilename(QString filename)
{
    _filename = filename;
    _loading = true;
    emit filenameChanged(filename);
    emit loadingChanged(_loading);

    std::unique_ptr<WorkoutDetailsLoader> runnable(new WorkoutDetailsLoader(filename));
    connect(runnable.get(), &WorkoutDetailsLoader::finished, this, &WorkoutDetails::setData);
    QThreadPool::globalInstance()->start(runnable.release());
}

void WorkoutDetails::setData(Data data)
{
    _data = std::move(data);

    _altitude->setData(std::move(_data.altitude));
    _speed->setData(std::move(_data.speed));

    _loading = false;

    emit speedChanged(_speed);
    emit altitudeChanged(_altitude);
    emit sportChanged(data.sport);
    emit durationChanged(data.duration);
    emit distanceChanged(data.distance);
    emit maxSpeedChanged(data.maxSpeed);
    emit dateChanged(data.date);
    emit loadingChanged(_loading);
}

WorkoutDetailsLoader::WorkoutDetailsLoader(QString filename) : _filename(filename)
{
}

void WorkoutDetailsLoader::run()
{
    gpx file;
    file.load(_filename);

    const gpx::Track& track = file.track();

    data.maxSpeed = 0;
    data.duration = 0;
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
            float t = data.duration + t0.msecsTo(j.timestamp()) / 1000.0;

            if (j.hasAttribute(QGeoPositionInfo::GroundSpeed))
            {
                data.speed.emplace_back(t, j.attribute(QGeoPositionInfo::GroundSpeed) * 3.6);
                data.maxSpeed = std::max<float>(data.maxSpeed, j.attribute(QGeoPositionInfo::GroundSpeed));
            }

            if (j.coordinate().type() == QGeoCoordinate::Coordinate3D)
            {
                data.altitude.emplace_back(t, j.coordinate().altitude());
            }
        }

        data.duration += t0.msecsTo(i.back().timestamp()) / 1000;
    }

    data.date = file.startDate();
    data.distance = file.distance();
    data.sport = file.sport();

    emit finished(data);
}
