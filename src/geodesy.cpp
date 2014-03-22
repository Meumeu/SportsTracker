#include "geodesy.h"
#include <QGeoCoordinate>
#include <math.h>

vector3 wgs84_to_ecef(double lat, double lon, double alt)
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

vector3 wgs84_to_ecef(const QGeoCoordinate& coord)
{
    return wgs84_to_ecef(coord.latitude(), coord.longitude(), coord.altitude());
}

QGeoCoordinate ecef_to_wgs84(double x, double y, double z)
{
    double a = 6378137;
    double e2 = 6.69437999014E-3;

    double lon = atan2(y, x);

    double lat, alt;

    double p = sqrt(x * x + y * y);
    lat = atan(p / z);

    for(int i = 0; i < 8; ++i)
    {
        double N = a / sqrt(1 - e2 * sin(lat) * sin(lat));
        alt = p / cos(lat) - N;
        lat = atan(z / (p * (1 - e2  * N / (N + alt))));
    }

    QGeoCoordinate coord(lat * 180 / M_PI, lon * 180 / M_PI, alt);

    return coord;
}

double distance(const vector3& a, const vector3& b)
{
    double x = std::get<0>(a) - std::get<0>(b);
    double y = std::get<1>(a) - std::get<1>(b);
    double z = std::get<2>(a) - std::get<2>(b);

    return sqrt(x * x + y * y + z * z);
}
