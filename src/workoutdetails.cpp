#include "workoutdetails.h"
#include "gpx.h"
#include "qmlplotdata.h"

#include <memory>
#include <QQuickPaintedItem>
#include <QThread>
#include <QThreadPool>

WorkoutDetails::WorkoutDetails(QObject *parent) :
    QObject(parent),
    _loading(true)
{
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
    delete _data.altitude;
    delete _data.speed;

    _data = data;

    _data.altitude->setParent(this);
    _data.altitude->setAxisId(QmlPlotData::Right);

    _data.speed->setParent(this);
    _data.speed->setAxisId(QmlPlotData::Left);

    _loading = false;

    emit speedChanged(data.speed);
    emit altitudeChanged(data.altitude);
    emit sportChanged(data.sport);
    emit durationChanged(data.duration);
    emit distanceChanged(data.distance);
    emit maxSpeedChanged(data.maxSpeed);
    emit dateChanged(data.date);
    emit loadingChanged(_loading);
}

WorkoutDetailsLoader::WorkoutDetailsLoader(QString filename) : _filename(filename)
{
    data.altitude = new QmlPlotData;
    data.speed = new QmlPlotData;
}

void WorkoutDetailsLoader::run()
{
    gpx file;
    file.load(_filename);

    std::vector<QVector2D> speed_data;
    std::vector<QVector2D> altitude_data;

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
                speed_data.emplace_back(t, j.attribute(QGeoPositionInfo::GroundSpeed));
                data.maxSpeed = std::max<float>(data.maxSpeed, j.attribute(QGeoPositionInfo::GroundSpeed));
            }

            if (j.coordinate().type() == QGeoCoordinate::Coordinate3D)
            {
                altitude_data.emplace_back(t, j.coordinate().altitude());
            }
        }

        data.duration += t0.msecsTo(i.back().timestamp()) / 1000;
    }

    data.altitude->setData(std::move(altitude_data));
    data.speed->setData(std::move(speed_data));

    data.date = file.startDate();
    data.distance = file.distance();
    data.sport = file.sport();

    emit finished(data);
}
