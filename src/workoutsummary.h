#ifndef WORKOUTSUMMARY_H
#define WORKOUTSUMMARY_H

#include <QObject>
#include <QDateTime>

class WorkoutSummary : public QObject
{
    Q_OBJECT
public:
    explicit WorkoutSummary(QString filename, QObject *parent = 0);

    Q_PROPERTY(QString sport READ sport CONSTANT)
    Q_PROPERTY(QDateTime date READ date CONSTANT)
    Q_PROPERTY(double distance READ distance CONSTANT)
    Q_PROPERTY(double time READ time CONSTANT)

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

signals:

public slots:

};

#endif // WORKOUTSUMMARY_H
