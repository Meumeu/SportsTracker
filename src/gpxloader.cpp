#include "gpx.h"
#include <QXmlStreamReader>
#include <QStandardPaths>
#include <QLocale>
#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <stdexcept>

static void parse_metadata_extensions(gpx& ws, QXmlStreamReader& doc)
{
    while(true)
    {
        switch (doc.readNext())
        {
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::EndDocument:
            return;
        case QXmlStreamReader::StartElement:
            if (doc.name() == QStringLiteral("duration"))
            {
                bool ok;
                ws.setDuration(QLocale::c().toDouble(doc.readElementText(), &ok));
                if (!ok)
                    doc.raiseError("Invalid duration");
            }
            else if (doc.name() == QStringLiteral("sport"))
            {
                ws.setSport(doc.readElementText());
            }
            else
            {
                doc.skipCurrentElement();
            }
        default:
            break;
        }
    }
}

static void parse_metadata(gpx& ws, QXmlStreamReader& doc)
{
    while(true)
    {
        switch (doc.readNext())
        {
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::EndDocument:
            return;
        case QXmlStreamReader::StartElement:
            if (doc.name() == QStringLiteral("time"))
            {
                QDateTime t = QDateTime::fromString(doc.readElementText(), Qt::ISODate);
                if (t.isValid())
                    ws.setStartDate(t);
                else
                    doc.raiseError("Invalid start date");
            }
            else if (doc.name() == QStringLiteral("extensions"))
            {
                parse_metadata_extensions(ws, doc);
            }
            else
            {
                doc.skipCurrentElement();
            }
        default:
            break;
        }
    }
}

static void parse_trkpt_extensions(QXmlStreamReader& doc, QGeoPositionInfo& pt)
{
    while(true)
    {
        switch (doc.readNext())
        {
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::EndDocument:
            return;
        case QXmlStreamReader::StartElement:
            if (doc.name() == QStringLiteral("groundspeed") && doc.namespaceUri() == "http://sportstracker.meumeu.org/")
            {
                bool ok;
                pt.setAttribute(QGeoPositionInfo::GroundSpeed, QLocale::c().toDouble(doc.readElementText(), &ok));
                if (!ok)
                {
                    doc.raiseError("Invalid ground speed");
                }
            }
            else
            {
                doc.skipCurrentElement();
            }
        default:
            break;
        }
    }
}

static QGeoPositionInfo parse_trkpt(QXmlStreamReader& doc)
{
    QGeoPositionInfo pt;
    QGeoCoordinate coord;

    bool ok = false;
    const auto & attr = doc.attributes();

    if (attr.hasAttribute(QStringLiteral("lat")))
        coord.setLatitude(QLocale::c().toDouble(attr.value(QStringLiteral("lat")), &ok));
    if (!ok)
    {
        doc.raiseError("Invalid latitude");
        doc.skipCurrentElement();
    }

    if (attr.hasAttribute(QStringLiteral("lon")))
        coord.setLongitude(QLocale::c().toDouble(attr.value(QStringLiteral("lon")), &ok));
    if (!ok)
    {
        doc.raiseError("Invalid longitude");
        doc.skipCurrentElement();
    }
    while(true)
    {
        switch (doc.readNext())
        {
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::EndDocument:
            pt.setCoordinate(coord);
            return pt;

        case QXmlStreamReader::StartElement:
            if (doc.name() == QStringLiteral("ele"))
            {
                coord.setAltitude(QLocale::c().toDouble(doc.readElementText(), &ok));
                if (!ok)
                {
                    doc.raiseError("Invalid altitude");
                }
            }
            else if (doc.name() == QStringLiteral("time"))
            {
                pt.setTimestamp(QDateTime::fromString(doc.readElementText(), Qt::ISODate));
            }
            else if (doc.name() == QStringLiteral("hdop"))
            {
                pt.setAttribute(QGeoPositionInfo::HorizontalAccuracy, QLocale::c().toDouble(doc.readElementText(), &ok));
                if (!ok)
                {
                    doc.raiseError("Invalid hdop");
                }
            }
            else if (doc.name() == QStringLiteral("vdop"))
            {
                pt.setAttribute(QGeoPositionInfo::VerticalAccuracy, QLocale::c().toDouble(doc.readElementText(), &ok));
                if (!ok)
                {
                    doc.raiseError("Invalid vdop");
                }
            }
            else if (doc.name() == QStringLiteral("extensions"))
            {
                parse_trkpt_extensions(doc, pt);
            }
            else
            {
                doc.skipCurrentElement();
            }
        default:
            break;
        }
    }
    pt.setCoordinate(coord);
    return pt;

}

static void parse_trkseg(QXmlStreamReader &doc, gpx::TrackSegment & trkseg)
{
    while(true)
    {
        switch (doc.readNext())
        {
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::EndDocument:
            return;
        case QXmlStreamReader::StartElement:
            if (doc.name() == QStringLiteral("trkpt"))
            {
                trkseg.push_back(parse_trkpt(doc));
            }
            else
            {
                doc.skipCurrentElement();
            }
        default:
            break;
        }
    }
}

static gpx::TrackSegment parse_trkseg(QXmlStreamReader& doc)
{
    gpx::TrackSegment trkseg;
    parse_trkseg(doc, trkseg);

    for(ssize_t i = 1; i < trkseg.size(); ++i)
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

    while(true)
    {
        switch (doc.readNext())
        {
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::EndDocument:
            return trk;
        case QXmlStreamReader::StartElement:
            if (doc.name() == QStringLiteral("trkseg"))
            {
                trk.push_back(parse_trkseg(doc));
            }
            else
            {
                doc.skipCurrentElement();
            }
        default:
            break;
        }
    }

    return trk;
}

static void parse_gpx(gpx& ws, QXmlStreamReader& doc)
{
    while(true)
    {
        switch (doc.readNext())
        {
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::EndDocument:
            return;
        case QXmlStreamReader::StartElement:
            if (doc.name() == QStringLiteral("metadata"))
            {
                parse_metadata(ws, doc);
            }
            else if (doc.name() == QStringLiteral("trk"))
            {
                ws.setTrack(parse_trk(doc));
            }
            else
            {
                doc.skipCurrentElement();
            }
        default:
            break;
        }
    }
}

void gpx::load(const QString &filename)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + filename;
    QString cachepath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QDir::separator() + filename + ".bin";

    QFileInfo cachefileinfo(cachepath);

    if (cachefileinfo.exists() && cachefileinfo.lastModified() > QFileInfo(path).lastModified())
    {
        QFile file(cachepath);
        file.open(QIODevice::ReadOnly);
        if (deserialize(&file))
            return;
    }

    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader doc(&file);

    while(true)
    {
        switch (doc.readNext())
        {
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::EndDocument:
        {
            QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)).mkpath(".");
            QSaveFile cachefile(cachepath);
            cachefile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            serialize(&cachefile);
            cachefile.commit();
            return;
        }
        case QXmlStreamReader::StartElement:
            if (doc.name() == QStringLiteral("gpx"))
            {
                parse_gpx(*this, doc);
            }
        default:
            break;
        }
    }

    if (doc.hasError())
    {
        throw std::runtime_error(doc.errorString().toStdString());
    }
}
