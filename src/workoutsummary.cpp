#include "workoutsummary.h"
#include <QDir>
#include <QStandardPaths>
#include <QDomDocument>
#include <QDateTime>

WorkoutSummary::WorkoutSummary(QString filename, QObject *parent) :
    QObject(parent), _filename(filename)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + filename;

    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    QDomElement root = doc.firstChildElement("gpx");
    if (root.isNull()) return;

    QDomElement metadata = root.firstChildElement("metadata");
    QDomElement metadataExt = metadata.firstChildElement("extensions");

    _sport = metadataExt.firstChildElement("sportstracker:sport").firstChild().toText().data();
    QString start_date = metadata.firstChildElement("time").firstChild().toText().data();
    _date = QDateTime::fromString(start_date, Qt::ISODate);
    _distance = metadataExt.firstChildElement("sportstracker:distance").firstChild().toText().data().toDouble();
    _time = metadataExt.firstChildElement("sportstracker:duration").firstChild().toText().data().toDouble();
}

QString WorkoutSummary::sport() const
{
    return _sport;
}

QDateTime WorkoutSummary::date() const
{
    return _date;
}

double WorkoutSummary::distance() const
{
    return _distance;
}

double WorkoutSummary::time() const
{
    return _time;
}

QString WorkoutSummary::filename() const
{
    return _filename;
}
