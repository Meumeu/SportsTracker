#include "qmlplot.h"
#include <QPainter>
#include <limits>
#include <iostream>

static constexpr float xtick_sizes[] = {1, 5, 15, 60, 300, 900, 3600, 14400, 43200, 86400};

constexpr float round_xtick_size(float x, int n = 0)
{
    return n == sizeof(xtick_sizes) / sizeof(xtick_sizes[0]) ? xtick_sizes[n-1] : x < xtick_sizes[n] ? xtick_sizes[n] : round_xtick_size(x, n+1);
}

QmlPlot::QmlPlot(QQuickItem * parent) :
    QQuickPaintedItem(parent)
{
}

void QmlPlot::setPadding(QMargins padding)
{
    _padding = padding;
    emit paddingChanged(padding);
    update();
}

void QmlPlot::setXmax(float xmax)
{
    _xmax = xmax;
    emit xmaxChanged(xmax);
    update();
}

void QmlPlot::resetXmax()
{
    float xmax = std::numeric_limits<float>::lowest();

    for(const QmlPlotData * i: findChildren<QmlPlotData *>())
    {
        for(const QVector2D& j: i->data())
        {
            xmax = std::max(xmax, j.x());
        }
    }

    setXmax(xmax);
}

void QmlPlot::setXmin(float xmin)
{
    _xmin = xmin;
    emit xminChanged(xmin);
    update();
}

void QmlPlot::resetXmin()
{
    float xmin = std::numeric_limits<float>::max();

    for(const QmlPlotData * i: findChildren<QmlPlotData *>())
    {
        for(const QVector2D& j: i->data())
        {
            xmin = std::min(xmin, j.x());
        }
    }

    setXmin(xmin);
}

void QmlPlot::setXtick(float xtick)
{
    _xtick = xtick;
    emit xtickChanged(xtick);
    update();
}

void QmlPlot::resetXtick()
{
    float xsize = _xmax - _xmin;
    float xtick = round_xtick_size(xsize / 10);
    setXtick(xtick);
}

void QmlPlot::setXoffset(float xoffset)
{
    _xoffset = xoffset;
    emit xoffsetChanged(xoffset);
    update();
}

void QmlPlot::paint(QPainter * painter)
{
    std::cerr << "Painting" << std::endl;
    float padding = 20;
    QRect win = painter->window();
    painter->setClipRect(padding, padding, win.width() - padding, win.height() - padding);

    int w = win.width() - 2 * padding;
    int h = win.height() - 2 * padding;

    _xmin = std::numeric_limits<float>::max();
    _xmax = std::numeric_limits<float>::lowest();
    float ymin = std::numeric_limits<float>::max();
    float ymax = std::numeric_limits<float>::lowest();

    for(const QmlPlotData * i: findChildren<QmlPlotData *>())
    {
        for(const QVector2D& j: i->data())
        {
            _xmin = std::min(_xmin, j.x());
            _xmax = std::max(_xmax, j.x());
            ymin = std::min(ymin, j.y());
            ymax = std::max(ymax, j.y());
        }
    }

    std::cerr << "xmin: " << _xmin << std::endl;
    std::cerr << "xmax: " << _xmax << std::endl;
    std::cerr << "ymin: " << ymin << std::endl;
    std::cerr << "ymax: " << ymax << std::endl;


    for(const QmlPlotData * i: findChildren<QmlPlotData *>())
    {
        std::cerr << "painting " << i << std::endl;
        painter->setPen(i->colour());
        std::vector<QPoint> points(i->data().size());
        for(const QVector2D& j: i->data())
        {
            points.emplace_back(padding + w * (j.x() - _xmin) / (_xmax - _xmin),
                                padding + h - h * (j.y() - ymin) / (ymax - ymin));
        }

        painter->drawPolyline(points.data(), points.size());
    }
}
