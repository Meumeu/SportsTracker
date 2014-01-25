#ifndef WORKOUTSUMMARY_H
#define WORKOUTSUMMARY_H

#include <QString>
#include <QDateTime>

class WorkoutSummary
{
public:
    explicit WorkoutSummary(QString filename);
    WorkoutSummary();

    QString filename() const { return _filename; }
    QString sport() const { return _sport; }
    QDateTime date() const { return _date; }
    double distance() const { return _distance; }
    double duration() const { return _duration; }

    void setSport(const QString&);
    void setDate(QDateTime);
    void setDistance(double);
    void setDuration(double);

private:
    QString _filename;
    QString _sport;
    QDateTime _date;
    double _distance;
    double _duration;
};

#endif // WORKOUTSUMMARY_H
