#include "gpx.h"
#include <iostream>
#include <assert.h>
#include <QDir>
#include <QStandardPaths>
#include <QSaveFile>
#include <QXmlStreamWriter>
#include <math.h>

typedef std::tuple<double, double, double> vector3;

static vector3 wgs84_to_ecef(double lat, double lon, double alt)
{
    double a = 6378137;
    double e2 = 6.69437999014E-3;

    double cos_lat = cos(lat * M_PI / 180);
    double sin_lat = sin(lat * M_PI / 180);
    double cos_lon = cos(lon * M_PI / 180);
    double sin_lon = sin(lon * M_PI / 180);

    double N = a / sqrt(1 - e2 * sin_lat * sin_lat);

    return vector3{
        (N + alt) * cos_lat * cos_lon,
        (N + alt) * cos_lat * sin_lon,
        (N * (1 - e2) + alt) * sin_lat
    };
}

static vector3 wgs84_to_ecef(const QGeoCoordinate& coord)
{
    return wgs84_to_ecef(coord.latitude(), coord.longitude(), 0);
}

static double distance(const vector3& a, const vector3& b)
{
    double x = std::get<0>(a) - std::get<0>(b);
    double y = std::get<1>(a) - std::get<1>(b);
    double z = std::get<2>(a) - std::get<2>(b);

    return sqrt(x * x + y * y + z * z);
}

double gpx::distance(const QGeoCoordinate& a, const QGeoCoordinate& b)
{
    return ::distance(wgs84_to_ecef(a), wgs84_to_ecef(b));
}

gpx::gpx() : _distance(0)
{
}

void gpx::addPoint(const QGeoPositionInfo &pt)
{
    if (!pt.timestamp().isValid()) return;

    if (track.empty())
        track.emplace_back(TrackSegment{});

    if (!track.back().empty())
    {
        if (track.back().back().timestamp().msecsTo(pt.timestamp()) > 5000)
            track.emplace_back(TrackSegment{});
    }

    if (!track.back().empty())
    {
        _distance += distance(track.back().back().coordinate(), pt.coordinate());
    }

    std::cerr << "Added " << pt.coordinate().toString().toStdString() << std::endl;
    track.back().push_back(pt);
}

void gpx::saveWaypoint(QXmlStreamWriter& doc, const QGeoPositionInfo& pt)
{
    doc.writeStartElement("trkpt");
    doc.writeAttribute("lat", QString::number(pt.coordinate().latitude(), 'f', 10));
    doc.writeAttribute("lon", QString::number(pt.coordinate().longitude(), 'f', 10));

    if (pt.coordinate().type() == QGeoCoordinate::Coordinate3D)
        doc.writeTextElement("ele", QString::number(pt.coordinate().altitude()));

    doc.writeTextElement("time", pt.timestamp().toUTC().toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));

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

QString gpx::save(QDateTime start_date, double duration)
{
    start_date = start_date.toUTC();
    QString filename = start_date.toString("yyyy'-'MM'-'dd'_'hh'-'mm'-'ss'.gpx'");
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir(path).mkpath(".");

    QSaveFile file(path + QDir::separator() + filename);

    std::cerr << "Saving to " << file.fileName().toStdString() << std::endl;
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QXmlStreamWriter doc(&file);

    doc.writeStartDocument();
    doc.writeStartElement("gpx");
    doc.writeDefaultNamespace("http://www.topografix.com/GPX/1/1");
    doc.writeNamespace("http://sportstracker.meumeu.org/");
    doc.writeAttribute("version", "1.1");
    doc.writeAttribute("creator", "http://sportstracker.meumeu.org/");

    doc.writeStartElement("metadata");
    doc.writeTextElement("time", start_date.toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
    doc.writeStartElement("extensions");
    doc.writeTextElement("http://sportstracker.meumeu.org/", "duration", QString::number(duration));
    doc.writeTextElement("http://sportstracker.meumeu.org/", "distance", QString::number(_distance));
    doc.writeTextElement("http://sportstracker.meumeu.org/", "sport", "foobar");
    doc.writeEndElement();
    doc.writeEndElement();

    doc.writeStartElement("trk");
    for(const auto& i: track)
    {
        doc.writeStartElement("trkseg");
        for(const QGeoPositionInfo& j: i)
        {
            saveWaypoint(doc, j);
        }
        doc.writeEndElement();
    }
    doc.writeEndElement();

    doc.writeEndElement();
    doc.writeEndDocument();

    file.commit();

    return filename;
}
