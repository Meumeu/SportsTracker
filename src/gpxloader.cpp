#include "gpx.h"
#include <QXmlStreamReader>
#include <QStandardPaths>
#include <QLocale>
#include <QDir>
#include <QFile>
#include <stdexcept>

static void parse_metadata_extensions(gpx& ws, QXmlStreamReader& doc)
{
    while(!doc.atEnd())
    {
        doc.readNext();
        if (doc.isEndElement()) break;
        if (doc.isWhitespace()) continue;

        if (doc.isStartElement() && doc.name() == "duration")
        {
            bool ok;
            ws.setDuration(QLocale::c().toDouble(doc.readElementText(), &ok));
            if (!ok)
                doc.raiseError("Invalid duration");
        }
        else if (doc.isStartElement() && doc.name() == "sport")
        {
            ws.setSport(doc.readElementText());
        }
        else if (doc.isStartElement())
        {
            doc.skipCurrentElement();
        }
    }
}

static void parse_metadata(gpx& ws, QXmlStreamReader& doc)
{
    while(!doc.atEnd())
    {
        doc.readNext();
        if (doc.isEndElement()) break;
        if (doc.isWhitespace()) continue;

        if (doc.isStartElement() && doc.name() == "time")
        {
            QDateTime t = QDateTime::fromString(doc.readElementText(), Qt::ISODate);
            if (t.isValid())
                ws.setStartDate(t);
            else
                doc.raiseError("Invalid start date");
        }
        else if (doc.isStartElement() && doc.name() == "extensions")
        {
            parse_metadata_extensions(ws, doc);
        }
        else if (doc.isStartElement())
        {
            doc.skipCurrentElement();
        }
    }
}

static void parse_trkpt_extensions(QXmlStreamReader& doc, QGeoPositionInfo& pt)
{
    while(!doc.atEnd())
    {
        doc.readNext();
        if (doc.isEndElement()) break;
        if (doc.isWhitespace()) continue;

        if (doc.isStartElement() && doc.name() == "groundspeed" && doc.namespaceUri() == "http://sportstracker.meumeu.org/")
        {
            bool ok;
            pt.setAttribute(QGeoPositionInfo::GroundSpeed, QLocale::c().toDouble(doc.readElementText(), &ok));
            if (!ok)
            {
                doc.raiseError("Invalid ground speed");
            }
        }
        else if (doc.isStartElement())
        {
            doc.skipCurrentElement();
        }
    }
}

static QGeoPositionInfo parse_trkpt(QXmlStreamReader& doc)
{
    QGeoPositionInfo pt;
    QGeoCoordinate coord;

    bool ok = false;
    const auto & attr = doc.attributes();

    if (attr.hasAttribute("lat"))
        coord.setLatitude(QLocale::c().toDouble(attr.value("lat"), &ok));
    if (!ok)
    {
        doc.raiseError("Invalid latitude");
        doc.skipCurrentElement();
    }

    if (attr.hasAttribute("lon"))
        coord.setLongitude(QLocale::c().toDouble(attr.value("lon"), &ok));
    if (!ok)
    {
        doc.raiseError("Invalid longitude");
        doc.skipCurrentElement();
    }

    while(!doc.atEnd())
    {
        doc.readNext();
        if (doc.isEndElement()) break;
        if (doc.isWhitespace()) continue;

        if (doc.isStartElement() && doc.name() == "ele")
        {
            coord.setAltitude(QLocale::c().toDouble(doc.readElementText(), &ok));
            if (!ok)
            {
                doc.raiseError("Invalid altitude");
            }
        }
        else if (doc.isStartElement() && doc.name() == "time")
        {
            pt.setTimestamp(QDateTime::fromString(doc.readElementText(), Qt::ISODate));
        }
        else if (doc.isStartElement() && doc.name() == "hdop")
        {
            pt.setAttribute(QGeoPositionInfo::HorizontalAccuracy, QLocale::c().toDouble(doc.readElementText(), &ok));
            if (!ok)
            {
                doc.raiseError("Invalid hdop");
            }
        }
        else if (doc.isStartElement() && doc.name() == "vdop")
        {
            pt.setAttribute(QGeoPositionInfo::VerticalAccuracy, QLocale::c().toDouble(doc.readElementText(), &ok));
            if (!ok)
            {
                doc.raiseError("Invalid vdop");
            }
        }
        else if (doc.isStartElement() && doc.name() == "extensions")
        {
            parse_trkpt_extensions(doc, pt);
        }
        else if (doc.isStartElement())
        {
            doc.skipCurrentElement();
        }
    }

    pt.setCoordinate(coord);

    return pt;
}

static gpx::TrackSegment parse_trkseg(QXmlStreamReader& doc)
{
    gpx::TrackSegment trkseg;

    while(!doc.atEnd())
    {
        doc.readNext();
        if (doc.isEndElement()) break;
        if (doc.isWhitespace()) continue;

        if (doc.isStartElement() && doc.name() == "trkpt")
        {
            trkseg.push_back(parse_trkpt(doc));
        }
        else if (doc.isStartElement())
        {
            doc.skipCurrentElement();
        }
    }

    for(size_t i = 1; i < trkseg.size(); ++i)
    {
        double dx = gpx::distance(trkseg[i-1].coordinate(), trkseg[i].coordinate());
        double dt = trkseg[i-1].timestamp().msecsTo(trkseg[i].timestamp()) * 0.001;
        trkseg[i].setAttribute(QGeoPositionInfo::GroundSpeed, dx / dt);
    }
    return trkseg;
}

static gpx::Track parse_trk(QXmlStreamReader& doc)
{
    gpx::Track trk;

    while(!doc.atEnd())
    {
        doc.readNext();
        if (doc.isEndElement()) break;
        if (doc.isWhitespace()) continue;

        if (doc.isStartElement() && doc.name() == "trkseg")
        {
            trk.push_back(parse_trkseg(doc));
        }
        else if (doc.isStartElement())
        {
            doc.skipCurrentElement();
        }
    }

    return trk;
}

static void parse_gpx(gpx& ws, QXmlStreamReader& doc)
{
    while(!doc.atEnd())
    {
        doc.readNext();
        if (doc.isEndElement()) break;
        if (doc.isWhitespace()) continue;

        if (doc.isStartElement() && doc.name() == "metadata")
        {
            parse_metadata(ws, doc);
        }
        else if (doc.isStartElement() && doc.name() == "trk")
        {
            ws.setTrack(parse_trk(doc));
        }
        else if (doc.isStartElement())
        {
            doc.skipCurrentElement();
        }
    }
}

void gpx::load(const QString &filename)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + filename;

    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader doc(&file);

    while(!doc.atEnd())
    {
        doc.readNext();
        if (doc.isWhitespace()) continue;

        if (doc.isStartDocument())
        {
        }
        else if (doc.isStartElement() && doc.name() == "gpx")
        {
            parse_gpx(*this, doc);
        }
    }

    if (doc.hasError())
    {
        throw std::runtime_error(doc.errorString().toStdString());
    }
}
