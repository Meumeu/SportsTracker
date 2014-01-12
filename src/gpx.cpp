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

static void gpx_add_metadata(QDomDocument& doc, QDomElement& metadata, const QString& tag, const QString& data)
{
    QDomElement elem = doc.createElement(tag);
    elem.appendChild(doc.createTextNode(data));
    metadata.appendChild(elem);
}

void gpx::save(QDateTime start_date)
{
    start_date = start_date.toUTC();
    QString filename = start_date.toString("yyyy'-'MM'-'dd'_'hh'-'mm'-'ss'.gpx'");
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir(path).mkpath(".");

    std::cerr << "Data path: " << path.toStdString() << std::endl;

    QFile file(path + "/" + filename);

    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml", R"(version="1.0")"));

    QDomElement root = doc.createElement("gpx");
    root.setAttribute("xmlns", "http://www.topografix.com/GPX/1/1");
    root.setAttribute("version", "1.1");
    root.setAttribute("creator", "http://sportstracker.meumeu.org/");

    QDomElement metadata = doc.createElement("metadata");
    gpx_add_metadata(doc, metadata, "time", start_date.toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));

    root.appendChild(metadata);

    QDomElement trk = doc.createElement("trk");

    for(const auto& i: track)
    {
        QDomElement trkseg = doc.createElement("trkseg");
        for(const QGeoPositionInfo& j: i)
        {
            QDomElement trkpt = doc.createElement("trkpt");
            trkpt.setAttribute("lat", QString::number(j.coordinate().latitude(), 'f', 10));
            trkpt.setAttribute("lon", QString::number(j.coordinate().longitude(), 'f', 10));

            if (j.coordinate().type() == QGeoCoordinate::Coordinate3D)
            {
                QDomElement ele = doc.createElement("ele");
                ele.appendChild(doc.createTextNode(QString::number(j.coordinate().altitude())));
                trkpt.appendChild(ele);
            }

            gpx_add_attribute(doc, trkpt, j, QGeoPositionInfo::MagneticVariation, "magvar");

            QDomElement fix = doc.createElement("fix");
            switch(j.coordinate().type())
            {
                case QGeoCoordinate::Coordinate3D:
                    fix.appendChild(doc.createTextNode("3d"));
                    trkpt.appendChild(fix);
                    break;
                case QGeoCoordinate::Coordinate2D:
                    fix.appendChild(doc.createTextNode("2d"));
                    trkpt.appendChild(fix);
                    break;
                case QGeoCoordinate::InvalidCoordinate:
                    fix.appendChild(doc.createTextNode("none"));
                    trkpt.appendChild(fix);
                    break;
            }

            gpx_add_attribute(doc, trkpt, j, QGeoPositionInfo::HorizontalAccuracy, "hdop");
            gpx_add_attribute(doc, trkpt, j, QGeoPositionInfo::VerticalAccuracy, "vdop");

            trkseg.appendChild(trkpt);
        }
        trk.appendChild(trkseg);
    }

    root.appendChild(trk);
    doc.appendChild(root);

    std::cerr << "Saving to " << file.fileName().toStdString() << std::endl;
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    qint64 rc = file.write(doc.toByteArray());
    if (rc == -1)
        std::cerr << "Error while saving GPX file: " << file.errorString().toStdString() << std::endl;
    file.close();
}
