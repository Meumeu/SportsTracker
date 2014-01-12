#ifndef WORKOUTSUMMARYLIST_H
#define WORKOUTSUMMARYLIST_H

#include <QObject>
#include <QQmlListProperty>
#include "workoutsummary.h"

class WorkoutSummaryList : public QObject
{
    Q_OBJECT

    QList<WorkoutSummary *> _list;
    QQmlListProperty<WorkoutSummary> _workoutList;

    double _totalDistance;
    double _totalTime;

    void computeDistanceAndTime();

public:
    typedef QQmlListProperty<WorkoutSummary> WorkoutList;

    explicit WorkoutSummaryList(QObject *parent = 0);

    Q_PROPERTY(QQmlListProperty<WorkoutSummary> workoutList READ workoutList NOTIFY workoutListChanged)
    Q_PROPERTY(double totalDistance READ totalDistance NOTIFY totalDistanceChanged)
    Q_PROPERTY(double totalTime READ totalTime NOTIFY totalTimeChanged)

    Q_INVOKABLE void addWorkout(QString filename);
    Q_INVOKABLE void removeWorkout(QString filename);

    WorkoutList& workoutList();
    double totalDistance();
    double totalTime();

signals:
    void workoutListChanged(WorkoutList& workoutList);
    void totalDistanceChanged(double totalDistance);
    void totalTimeChanged(double totalTime);

public slots:
};

#endif // WORKOUTSUMMARYLIST_H
