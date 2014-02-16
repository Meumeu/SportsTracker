#include "gpx.h"
#include <QXmlStreamWriter>
#include <QStandardPaths>
#include <QLocale>
#include <QDir>
#include <QSaveFile>
#include <QLocale>
#include <iostream>

static void save_waypoint(QXmlStreamWriter& doc, const QGeoPositionInfo& pt)
{
    doc.writeStartElement("trkpt");
    doc.writeAttribute("lat", QString::number(pt.coordinate().latitude(), 'f', 10));
    doc.writeAttribute("lon", QString::number(pt.coordinate().longitude(), 'f', 10));

    if (pt.coordinate().type() == QGeoCoordinate::Coordinate3D)
        doc.writeTextElement("ele", QString::number(pt.coordinate().altitude()));

    doc.writeTextElement("time", pt.timestamp().toUTC().toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss','zzz'Z'"));

    if (pt.hasAttribute(QGeoPositionInfo::MagneticVariation))
        doc.writeTextElement("magvar", QString::number(pt.attribute(QGeoPositionInfo::MagneticVariation)));

    switch(pt.coordinate().type())
    {
    case QGeoCoordinate::Coordinate3D:
        doc.writeTextElement("fix", "3d");
        break;
    case QGeoCoordinate::Coordinate2D:
        doc.writeTextElement("fix", "2d");
        break;
    case QGeoCoordinate::InvalidCoordinate:
        doc.writeTextElement("fix", "none");
        break;
    }

    if (pt.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
        doc.writeTextElement("hdop", QString::number(pt.attribute(QGeoPositionInfo::HorizontalAccuracy)));

    if (pt.hasAttribute(QGeoPositionInfo::VerticalAccuracy))
        doc.writeTextElement("vdop", QString::number(pt.attribute(QGeoPositionInfo::VerticalAccuracy)));

    doc.writeStartElement("extensions");
    if (pt.hasAttribute(QGeoPositionInfo::GroundSpeed))
        doc.writeTextElement("http://sportstracker.meumeu.org/", "groundspeed", QString::number(pt.attribute(QGeoPositionInfo::GroundSpeed)));
    doc.writeEndElement();
    doc.writeEndElement();
}

QString gpx::save()
{
    QDateTime start_date = _start_date.toUTC();
    QString filename = start_date.toString("yyyy'-'MM'-'dd'_'hh'-'mm'-'ss'.gpx'");
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + filename;
    QString cachepath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QDir::separator() + filename + ".bin";
    QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation)).mkpath(".");
    QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)).mkpath(".");

    QSaveFile file(path);

    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QXmlStreamWriter doc(&file);
    doc.setAutoFormatting(true);

    doc.writeStartDocument();
    doc.writeStartElement("gpx");
    doc.writeDefaultNamespace("http://www.topografix.com/GPX/1/1");
    doc.writeNamespace("http://sportstracker.meumeu.org/", "sport");
    doc.writeAttribute("version", "1.1");
    doc.writeAttribute("creator", "http://sportstracker.meumeu.org/");

    doc.writeStartElement("metadata");
    doc.writeTextElement("time", start_date.toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
    doc.writeStartElement("extensions");
    doc.writeTextElement("http://sportstracker.meumeu.org/", "duration", QString::number(_duration));
    doc.writeTextElement("http://sportstracker.meumeu.org/", "sport", _sport);
    doc.writeEndElement();
    doc.writeEndElement();

    doc.writeStartElement("trk");
    for(const auto& i: _track)
    {
        doc.writeStartElement("trkseg");
        for(const QGeoPositionInfo& j: i)
        {
            save_waypoint(doc, j);
        }
        doc.writeEndElement();
    }
    doc.writeEndElement();

    doc.writeEndElement();
    doc.writeEndDocument();

    file.commit();

    QSaveFile cachefile(cachepath);
    cachefile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    serialize(&cachefile);
    cachefile.commit();

    return filename;
}
