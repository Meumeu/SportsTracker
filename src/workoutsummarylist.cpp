#include "workoutsummarylist.h"
#include <iostream>
#include <algorithm>
#include <QDir>
#include <QStandardPaths>

WorkoutSummaryList::WorkoutSummaryList(QObject *parent) :
    QObject(parent),
    _workoutList(this, _list),
    _totalDistance(0),
    _totalTime(0)
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QStringList filter;
    filter << "*.gpx";
    QStringList files = dir.entryList(filter, QDir::Files, QDir::Name);

    for(const QString i: files)
    {
        std::cerr << "File: " << i.toStdString() << std::endl;
        addWorkout(i);
    }
}

void WorkoutSummaryList::computeDistanceAndTime()
{
    _totalDistance = 0;
    _totalTime = 0;
    for(WorkoutSummary * i: _list)
    {
        _totalDistance += i->distance();
        _totalTime += i->time();
    }
}

void WorkoutSummaryList::addWorkout(QString filename)
{
    QList<WorkoutSummary*>::iterator i = std::find_if(_list.begin(), _list.end(),
        [filename](WorkoutSummary * s)
        {
            return s->filename() <= filename;
        });

    if (i != _list.end() && (*i)->filename() == filename)
    {
        delete *i;
        *i = new WorkoutSummary(filename, this);
    }
    else
    {
        _list.insert(i, new WorkoutSummary(filename, this));
    }

    //_list << new WorkoutSummary(filename, this);
    computeDistanceAndTime();

    emit workoutListChanged(_workoutList);
    emit totalDistanceChanged(_totalDistance);
    emit totalTimeChanged(_totalTime);
}

void WorkoutSummaryList::removeWorkout(QString filename)
{
}

WorkoutSummaryList::WorkoutList& WorkoutSummaryList::workoutList()
{
    return _workoutList;
}

double WorkoutSummaryList::totalDistance()
{
    return _totalDistance;
}

double WorkoutSummaryList::totalTime()
{
    return _totalTime;
}
