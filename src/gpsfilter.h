#ifndef GPSFILTER_H
#define GPSFILTER_H

#include <QVector>
class QGeoPositionInfo;

class GpsFilter
{
public:
    GpsFilter();

    QVector<QGeoPositionInfo> operator()(const QVector<QGeoPositionInfo>& in);
};

#endif // GPSFILTER_H
