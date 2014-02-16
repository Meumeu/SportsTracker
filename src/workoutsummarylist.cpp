#include "workoutsummarylist.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <QDir>
#include <QStandardPaths>
#include <QRunnable>
#include <QThreadPool>


WorkoutSummaryLoader::WorkoutSummaryLoader(QString filename):
    _filename(filename)
{
}

void WorkoutSummaryLoader::run()
{
    try
    {
        emit finished(WorkoutSummary(_filename));
    }
    catch(...)
    {
        // TODO
    }
}


enum class roles
{
    distance = Qt::UserRole,
    duration,
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
        _totalTime += i.duration();
    }

    emit totalDistanceChanged(_totalDistance);
    emit totalTimeChanged(_totalTime);
}

void WorkoutSummaryList::addWorkout(QString filename)
{
    std::unique_ptr<WorkoutSummaryLoader> runnable(new WorkoutSummaryLoader(filename));

    void(WorkoutSummaryList::*tmp)(WorkoutSummary) = &WorkoutSummaryList::addWorkout;
    connect(runnable.get(), &WorkoutSummaryLoader::finished, this, tmp);

    QThreadPool::globalInstance()->start(runnable.release());
}

void WorkoutSummaryList::addWorkout(WorkoutSummary workout)
{
    auto i = std::find_if(_list.begin(), _list.end(),
        [&workout](WorkoutSummary& s)
        {
            return s.filename() <= workout.filename();
        });

    int idx = i - _list.begin();
    if (i != _list.end() && i->filename() == workout.filename())
    {
        *i = workout;
        emit dataChanged(index(idx), index(idx));
    }
    else
    {
        beginInsertRows(QModelIndex(), idx, idx);
        _list.insert(i, workout);
        endInsertRows();
    }

    computeDistanceAndTime();
}

void WorkoutSummaryList::remove(int index)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation) +
            QDir::separator() + _list[index].filename();
    QFile::remove(path);

    path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
            QDir::separator() + _list[index].filename() + ".bin";
    QFile::remove(path);

    beginRemoveRows(QModelIndex(), index, index);
    _list.erase(_list.begin() + index, _list.begin() + index + 1);
    endRemoveRows();

    computeDistanceAndTime();
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
    case roles::duration:
        return _list[index.row()].duration();
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
    ret[(int)roles::duration] = "duration";
    ret[(int)roles::date] = "date";
    ret[(int)roles::sport] = "sport";
    ret[(int)roles::filename] = "filename";
    return ret;
}
