#ifndef WORKOUTSUMMARY_H
#define WORKOUTSUMMARY_H

#include <QString>
#include <QDateTime>

class WorkoutSummary
{
public:
    explicit WorkoutSummary(QString filename);
    WorkoutSummary();

    QString filename() const;
    QString sport() const;
    QDateTime date() const;
    double distance() const;
    double time() const;

private:
    QString _filename;
    QString _sport;
    QDateTime _date;
    double _distance;
    double _time;
};

#endif // WORKOUTSUMMARY_H
