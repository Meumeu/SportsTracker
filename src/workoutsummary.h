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
    double duration() const;

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
