#ifndef WORKOUTDETAILS_H
#define WORKOUTDETAILS_H

#include <QObject>
#include <QRunnable>
#include <QVector2D>
#include "gpx.h"

class QmlPlotData;

class WorkoutDetails : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(QmlPlotData * speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(QmlPlotData * altitude READ altitude NOTIFY altitudeChanged)
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
    };
private:

    QString _filename;
    QmlPlotData * _speed;
    QmlPlotData * _altitude;
    Data _data;
    bool _loading;

    void setData(Data data);

public:
    explicit WorkoutDetails(QObject *parent = 0);

    void setFilename(QString filename);

    const QString& filename() const { return _filename; }
    QmlPlotData * speed() { return _speed; }
    QmlPlotData * altitude() { return _altitude; }
    QString sport() const { return _data.sport; }
    QDateTime date() const { return _data.date; }
    double duration() const { return _data.duration; }
    double distance() const { return _data.distance; }
    double maxSpeed() const { return _data.maxSpeed; }
    bool loading() const { return _loading; }

signals:
    void filenameChanged(QString filename);
    void speedChanged(QmlPlotData * speed);
    void altitudeChanged(QmlPlotData * altitude);
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
