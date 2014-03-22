#include "gpsfilter.h"

#include <QGeoPositionInfo>
#include <vector>
#include <math.h>

#include "Eigen/Dense"
#include "Eigen/LU"
#include "geodesy.h"

namespace
{
std::vector<float> regression(int order, const std::vector<float>& x, const std::vector<float>& y, const std::vector<float>& weight, float dx = 0, float dy = 0)
{
    assert(x.size() == y.size());
    assert(x.size() == weight.size());

    Eigen::MatrixXf A(order+1, order+1);
    Eigen::VectorXf Y(order+1);

    int N = x.size();

    for(int i = 0; i <= order; ++i)
    {
        Y(i) = 0;

        for(int j = 0; j <= order; ++j)
        {
            A(i, j) = 0;
            for(int k = 0; k < N; ++k)
            {
                A(i, j) += weight[k] * pow((x[k] + dx), i+j);
            }
        }

        for(int j = 0; j < N; ++j)
        {
            Y(i) += weight[j] * pow((x[j] + dx), i) * (y[j] + dy);
        }
    }

    Eigen::VectorXf X = A.inverse() * Y;

    std::vector<float> out;
    for(int i = 0; i <= order; ++i)
    {
        out.push_back(X(i));
    }

    return out;
}

float eval_poly(const std::vector<float>& coefs, float x)
{
    float ret = 0;
    float tmp = 1;
    for(size_t i = 0; i < coefs.size(); ++i)
    {
        ret += tmp * coefs[i];
        tmp *= x;
    }

    return ret;
}


float eval_poly_diff(const std::vector<float>& coefs, float x)
{
    float ret = 0;
    float tmp = 1;
    for(size_t i = 1; i < coefs.size(); ++i)
    {
        ret += tmp * coefs[i] * i;
        tmp *= x;
    }

    return ret;
}
}

GpsFilter::GpsFilter()
{
}

QVector<QGeoPositionInfo> GpsFilter::operator ()(const QVector<QGeoPositionInfo>& in)
{
    QVector<QGeoPositionInfo> out;

    if (in.empty())
        return out;

    QDateTime t0 = in.front().timestamp();
    QDateTime t1 = in.back().timestamp();
    double duration = t0.msecsTo(t1) * 0.001;
    float dt = 1;
    float window_size = 15;

    std::vector<float> t, weight;
    std::vector<double> x, y, z;

    for(const QGeoPositionInfo& i: in)
    {
        auto pos = wgs84_to_ecef(i.coordinate());
        t.push_back(t0.msecsTo(i.timestamp()) * 0.001);
        x.push_back(std::get<0>(pos));
        y.push_back(std::get<1>(pos));
        z.push_back(std::get<2>(pos));
        weight.push_back(1 / (1 + i.attribute(QGeoPositionInfo::HorizontalAccuracy)));
    }

    std::vector<float> t_, x_, y_, z_, w_;
    for(float cur_t = 0; cur_t <= duration; cur_t += dt)
    {
        auto idx_1 = std::find_if(t.begin(), t.end(), [cur_t, window_size](float u){return u >= cur_t - window_size;}) - t.begin();
        auto idx_2 = std::find_if(t.begin() + idx_1, t.end(), [cur_t, window_size](float u){return u > cur_t + window_size;}) - t.begin();

        t_.clear();
        x_.clear();
        y_.clear();
        z_.clear();
        w_.clear();

        double cx = x[(idx_1 + idx_2) / 2];
        double cy = y[(idx_1 + idx_2) / 2];
        double cz = z[(idx_1 + idx_2) / 2];

        for(auto i = idx_1; i < idx_2; ++i)
        {
            t_.push_back(t[i] - cur_t);
            x_.push_back(x[i] - cx);
            y_.push_back(y[i] - cy);
            z_.push_back(z[i] - cz);
            w_.push_back(weight[i]);
        }

        std::vector<float> a;
        a = regression(2, t_, x_, w_);
        double pos_x = eval_poly(a, 0) + cx;
        float vel_x = eval_poly_diff(a, 0);

        a = regression(2, t_, y_, w_);
        double pos_y = eval_poly(a, 0) + cy;
        float vel_y = eval_poly_diff(a, 0);

        a = regression(2, t_, z_, w_);
        double pos_z = eval_poly(a, 0) + cz;
        float vel_z = eval_poly_diff(a, 0);

        float vel = sqrt(vel_x * vel_x + vel_y * vel_y + vel_z * vel_z);

        QGeoPositionInfo pt(ecef_to_wgs84(pos_x, pos_y, pos_z), t0.addMSecs(cur_t * 1000));
        pt.setAttribute(QGeoPositionInfo::GroundSpeed, vel);

        out.push_back(pt);
    }


    return out;
}
