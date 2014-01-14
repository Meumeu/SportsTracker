#include "workoutsummarylist.h"
#include <iostream>
#include <algorithm>
#include <QDir>
#include <QStandardPaths>

enum class roles
{
    distance = Qt::UserRole,
    time,
    date,
    sport,
    filename
};

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
    for(WorkoutSummary& i: _list)
    {
        _totalDistance += i.distance();
        _totalTime += i.time();
    }

    emit totalDistanceChanged(_totalDistance);
    emit totalTimeChanged(_totalTime);
}

void WorkoutSummaryList::addWorkout(QString filename)
{
    auto i = std::find_if(_list.begin(), _list.end(),
        [filename](WorkoutSummary& s)
        {
            return s.filename() <= filename;
        });

    int idx = i - _list.begin();
    if (i != _list.end() && i->filename() == filename)
    {
        *i = WorkoutSummary(filename);
        emit dataChanged(index(idx), index(idx));
    }
    else
    {
        beginInsertRows(QModelIndex(), idx, idx);
        _list.insert(i, WorkoutSummary(filename));
        endInsertRows();
    }

    computeDistanceAndTime();
}

bool WorkoutSummaryList::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    _list.erase(_list.begin() + row, _list.begin() + row + count - 1);

    endRemoveRows();
    return true;
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
    if (!index.isValid() || index.row() >= (ssize_t)_list.size())
        return QVariant();

    switch((roles)role)
    {
    case roles::distance:
        return _list[index.row()].distance();
    case roles::time:
        return _list[index.row()].time();
    case roles::date:
        return _list[index.row()].date();
    case roles::sport:
        return _list[index.row()].sport();
    case roles::filename:
        return _list[index.row()].filename();
    }

    return QVariant();
}

int WorkoutSummaryList::rowCount(const QModelIndex &) const
{
    return _list.size();
}

QHash<int, QByteArray> WorkoutSummaryList::roleNames() const
{
    QHash<int, QByteArray> ret;
    ret[(int)roles::distance] = "distance";
    ret[(int)roles::time] = "time";
    ret[(int)roles::date] = "date";
    ret[(int)roles::sport] = "sport";
    ret[(int)roles::filename] = "filename";
    return ret;
}
