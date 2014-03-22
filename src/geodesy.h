#ifndef GEODESY_H
#define GEODESY_H

#include <tuple>

class QGeoCoordinate;

typedef std::tuple<double, double, double> vector3;
vector3 wgs84_to_ecef(double lat, double lon, double alt);
vector3 wgs84_to_ecef(const QGeoCoordinate& coord);
QGeoCoordinate ecef_to_wgs84(double x, double y, double z);

double distance(const vector3& a, const vector3& b);


#endif // GEODESY_H
