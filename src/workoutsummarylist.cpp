#include "workoutsummarylist.h"
#include <iostream>
#include <algorithm>
#include <QDir>
#include <QStandardPaths>

WorkoutSummaryList::WorkoutSummaryList(QObject *parent) :
    QAbstractListModel(parent),
    _totalDistance(0),
    _totalTime(0)
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QStringList filter;
    filter << "*.gpx";
    QStringList files = dir.entryList(filter, QDir::Files, QDir::Name);
    _list.reserve(files.size());

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
    std::vector<WorkoutSummary*>::iterator i = std::find_if(_list.begin(), _list.end(),
        [filename](WorkoutSummary * s)
        {
            return s->filename() <= filename;
        });

    int idx = i - _list.begin();
    if (i != _list.end() && (*i)->filename() == filename)
    {
        delete *i;
        *i = new WorkoutSummary(filename, this);
        emit dataChanged(index(idx), index(idx));
    }
    else
    {
        beginInsertRows(QModelIndex(), idx, idx);
        _list.insert(i, new WorkoutSummary(filename, this));
        endInsertRows();
    }

    //_list << new WorkoutSummary(filename, this);
    computeDistanceAndTime();

    //emit workoutListChanged(_workoutList);
    emit totalDistanceChanged(_totalDistance);
    emit totalTimeChanged(_totalTime);
}

void WorkoutSummaryList::removeWorkout(QString filename)
{
}

double WorkoutSummaryList::totalDistance()
{
    return _totalDistance;
}

double WorkoutSummaryList::totalTime()
{
    return _totalTime;
}

QVariant WorkoutSummaryList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _list.size())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    return QVariant::fromValue(_list.at(index.row()));
}

int WorkoutSummaryList::rowCount(const QModelIndex &) const
{
    return _list.size();
}
