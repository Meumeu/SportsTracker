#ifndef WORKOUTDETAILS_H
#define WORKOUTDETAILS_H

#include <QObject>
#include "gpx.h"

class QmlPlotData;

class WorkoutDetails : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(const QmlPlotData * speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(const QmlPlotData * altitude READ altitude NOTIFY altitudeChanged)


    QString _filename;
    gpx _data;

    QmlPlotData * _speed;
    QmlPlotData * _altitude;

public:
    explicit WorkoutDetails(QObject *parent = 0);

    void setFilename(QString filename);

    const QString& filename() const { return _filename; }
    const QmlPlotData * speed() const { return _speed; }
    const QmlPlotData * altitude() const { return _altitude; }

signals:
    void filenameChanged(QString filename);
    void speedChanged(QmlPlotData * speed);
    void altitudeChanged(QmlPlotData * altitude);

public slots:

};

#endif // WORKOUTDETAILS_H
