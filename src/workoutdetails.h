#ifndef WORKOUTDETAILS_H
#define WORKOUTDETAILS_H

#include <QObject>
#include <QRunnable>
#include <QVector2D>
#include <QVariantList>
#include "gpx.h"

class QmlPlotDataSource;

class WorkoutDetails : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(QmlPlotDataSource * speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(QmlPlotDataSource * altitude READ altitude NOTIFY altitudeChanged)
    Q_PROPERTY(QString sport READ sport NOTIFY sportChanged)
    Q_PROPERTY(QDateTime date READ date NOTIFY dateChanged)
    Q_PROPERTY(double duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(double distance READ distance NOTIFY distanceChanged)
    Q_PROPERTY(double maxSpeed READ maxSpeed NOTIFY maxSpeedChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    struct Data
    {
        std::vector<QVector2D> speed;
        std::vector<QVector2D> altitude;
        QString sport;
        QDateTime date;
        double duration;
        double distance;
        double maxSpeed;
        std::vector<QGeoPositionInfo> points;
    };
private:

    QString _filename;
    QmlPlotDataSource * const _speed;
    QmlPlotDataSource * const _altitude;
    Data _data;
    bool _loading;

    void setData(Data data);

public:
    explicit WorkoutDetails(QObject *parent = 0);

    void setFilename(QString filename);

    const QString& filename() const { return _filename; }
    QmlPlotDataSource * speed() { return _speed; }
    QmlPlotDataSource * altitude() { return _altitude; }
    QString sport() const { return _data.sport; }
    QDateTime date() const { return _data.date; }
    double duration() const { return _data.duration; }
    double distance() const { return _data.distance; }
    double maxSpeed() const { return _data.maxSpeed; }
    bool loading() const { return _loading; }

    Q_INVOKABLE void fillPolyLine(QObject * obj) const;

signals:
    void filenameChanged(QString filename);
    void speedChanged(QmlPlotDataSource * speed);
    void altitudeChanged(QmlPlotDataSource * altitude);
    void sportChanged(QString sport);
    void dateChanged(QDateTime date);
    void durationChanged(double duration);
    void distanceChanged(double distance);
    void maxSpeedChanged(double maxSpeed);
    void loadingChanged(bool loading);

public slots:

};



class WorkoutDetailsLoader: public QObject, public QRunnable
{
    Q_OBJECT
    QString _filename;
    WorkoutDetails::Data data;
public:
    explicit WorkoutDetailsLoader(QString filename);

    void run();

signals:
    void finished(WorkoutDetails::Data data);
};

#endif // WORKOUTDETAILS_H
