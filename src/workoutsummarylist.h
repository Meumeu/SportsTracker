#ifndef WORKOUTSUMMARYLIST_H
#define WORKOUTSUMMARYLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QRunnable>
#include <vector>
#include "workoutsummary.h"

#if (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 7))
#define override
#endif

class QQmlEngine;
class QJSEngine;


class WorkoutSummaryList : public QAbstractListModel
{
    Q_OBJECT

    std::vector<WorkoutSummary> _list;

    double _totalDistance;
    double _totalTime;

    void computeDistanceAndTime();

public:

    explicit WorkoutSummaryList(QObject *parent = 0);

    Q_PROPERTY(double totalDistance READ totalDistance NOTIFY totalDistanceChanged)
    Q_PROPERTY(double totalTime READ totalTime NOTIFY totalTimeChanged)

    Q_INVOKABLE void addWorkout(QString filename);
    Q_INVOKABLE void addWorkout(WorkoutSummary workout);
    Q_INVOKABLE void remove(int index);

    double totalDistance();
    double totalTime();

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;

    static QObject * provider(QQmlEngine *, QJSEngine *) { return new WorkoutSummaryList; }

signals:
    void totalDistanceChanged(double totalDistance);
    void totalTimeChanged(double totalTime);

public slots:
};

class WorkoutSummaryLoader : public QObject, public QRunnable
{
    Q_OBJECT
    QString _filename;

public:
    WorkoutSummaryLoader(QString filename);

    void run();
signals:
    void finished(WorkoutSummary summary);
};

#endif // WORKOUTSUMMARYLIST_H
