#include "gpx.h"
#include <iostream>
#include <assert.h>
#include <QDir>
#include <QStandardPaths>
#include <QDomDocument>

typedef std::tuple<double, double, double> vector3;

static vector3 wgs84_to_ecef(double lat, double lon, double alt)
{
    double a = 6378137;
    double e2 = 6.69437999014E-3;

    double cos_lat = cos(lat);
    double sin_lat = sin(lat);
    double cos_lon = cos(lon);
    double sin_lon = sin(lon);

    double N = a / sqrt(1 - e2 * sin_lat * sin_lat);

    return vector3{
        (N + alt) * cos_lat * cos_lon,
        (N + alt) * cos_lat * sin_lon,
        (N * (1 - e2) + alt) * sin_lat
    };
}

static double distance(const vector3& a, const vector3& b)
{
    double x = std::get<0>(a) - std::get<0>(b);
    double y = std::get<1>(a) - std::get<1>(b);
    double z = std::get<2>(a) - std::get<2>(b);

    return sqrt(x * x + y * y + z * z);
}


gpx::gpx() : _distance(0), _doc(nullptr)
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
        vector3 a = wgs84_to_ecef(track.back().back().coordinate().latitude(), track.back().back().coordinate().longitude(), 0);
        vector3 b = wgs84_to_ecef(pt.coordinate().latitude(), pt.coordinate().longitude(), 0);
        _distance += ::distance(a, b);
    }

    std::cerr << "Added " << pt.coordinate().toString().toStdString() << std::endl;
    track.back().push_back(pt);
}

static void gpx_add_attribute(QDomDocument& doc, QDomElement& elem, const QGeoPositionInfo& pos, QGeoPositionInfo::Attribute a, const QString& node_name)
{
    if (!pos.hasAttribute(a)) return;

    QDomElement n = doc.createElement(node_name);
    n.appendChild(doc.createTextNode(QString::number(pos.attribute(a))));
    elem.appendChild(n);
}

void gpx::saveWaypoint(QDomElement& trkseg, const QGeoPositionInfo& pt)
{
    assert(_doc != nullptr);
    assert(trkseg.tagName() == "trkseg");

    QDomElement trkpt = _doc->createElement("trkpt");
    trkpt.setAttribute("lat", QString::number(pt.coordinate().latitude(), 'f', 10));
    trkpt.setAttribute("lon", QString::number(pt.coordinate().longitude(), 'f', 10));

    if (pt.coordinate().type() == QGeoCoordinate::Coordinate3D)
    {
        QDomElement ele = _doc->createElement("ele");
        ele.appendChild(_doc->createTextNode(QString::number(pt.coordinate().altitude())));
        trkpt.appendChild(ele);
    }

    gpx_add_attribute(*_doc, trkpt, pt, QGeoPositionInfo::MagneticVariation, "magvar");

    QDomElement fix = _doc->createElement("fix");
    switch(pt.coordinate().type())
    {
        case QGeoCoordinate::Coordinate3D:
            fix.appendChild(_doc->createTextNode("3d"));
            trkpt.appendChild(fix);
            break;
        case QGeoCoordinate::Coordinate2D:
            fix.appendChild(_doc->createTextNode("2d"));
            trkpt.appendChild(fix);
            break;
        case QGeoCoordinate::InvalidCoordinate:
            fix.appendChild(_doc->createTextNode("none"));
            trkpt.appendChild(fix);
            break;
    }

    gpx_add_attribute(*_doc, trkpt, pt, QGeoPositionInfo::HorizontalAccuracy, "hdop");
    gpx_add_attribute(*_doc, trkpt, pt, QGeoPositionInfo::VerticalAccuracy, "vdop");

    QDomElement ext = _doc->createElement("extensions");
    if (pt.hasAttribute(QGeoPositionInfo::GroundSpeed))
    {
        QDomElement speed = _doc->createElement("sport:groundspeed");
        speed.appendChild(_doc->createTextNode(QString::number(pt.attribute(QGeoPositionInfo::GroundSpeed))));
        ext.appendChild(speed);
    }

    if (ext.hasChildNodes())
        trkpt.appendChild(ext);

    trkseg.appendChild(trkpt);
}

void gpx::saveTrackSegment(QDomElement& trk, const TrackSegment& segment)
{
    assert(_doc != nullptr);
    assert(trk.tagName() == "trk");

    QDomElement trkseg = _doc->createElement("trkseg");

    for(const QGeoPositionInfo& i: segment)
    {
        saveWaypoint(trkseg, i);
    }

    trk.appendChild(trkseg);
}

void gpx::saveMetadataExt(QDomElement& metadataExt, const QString& name, const QString& value)
{
    assert(_doc != nullptr);
    assert(metadataExt.tagName() == "extensions");

    QDomElement ext = _doc->createElement("sport:" + name);
    ext.appendChild(_doc->createTextNode(value));
    metadataExt.appendChild(ext);
}

void gpx::saveMetadata(QDomElement& metadata, const QString& name, const QString& value)
{
    assert(_doc != nullptr);
    assert(metadata.tagName() == "metadata");

    QDomElement ext = _doc->createElement(name);
    ext.appendChild(_doc->createTextNode(value));
    metadata.appendChild(ext);
}

QString gpx::save(QDateTime start_date, double duration)
{
    start_date = start_date.toUTC();
    QString filename = start_date.toString("yyyy'-'MM'-'dd'_'hh'-'mm'-'ss'.gpx'");
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir(path).mkpath(".");

    QFile file(path + QDir::separator() + filename);

    _doc = new QDomDocument;
    _doc->appendChild(_doc->createProcessingInstruction("xml", R"(version="1.0")"));

    QDomElement root = _doc->createElement("gpx");
    root.setAttribute("xmlns", "http://www.topografix.com/GPX/1/1");
    root.setAttribute("xmlns:sport", "http://sportstracker.meumeu.org/");
    root.setAttribute("version", "1.1");
    root.setAttribute("creator", "http://sportstracker.meumeu.org/");

    QDomElement metadata = _doc->createElement("metadata");
    QDomElement metadataExt = _doc->createElement("extensions");
    root.appendChild(metadata);
    metadata.appendChild(metadataExt);

    saveMetadata(metadata, "time", start_date.toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
    saveMetadataExt(metadataExt, "duration", QString::number(duration));
    saveMetadataExt(metadataExt, "distance", QString::number(_distance));
    saveMetadataExt(metadataExt, "sport", "foobar");


    QDomElement trk = _doc->createElement("trk");

    for(const auto& i: track)
    {
        saveTrackSegment(trk, i);
    }

    root.appendChild(trk);
    _doc->appendChild(root);

    std::cerr << "Saving to " << file.fileName().toStdString() << std::endl;
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    qint64 rc = file.write(_doc->toByteArray());
    if (rc == -1)
        std::cerr << "Error while saving GPX file: " << file.errorString().toStdString() << std::endl;
    file.close();

    return filename;
}
