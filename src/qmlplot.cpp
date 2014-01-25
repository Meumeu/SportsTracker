#include "qmlplot.h"
#include <QPainter>
#include <limits>
#include <iostream>

static constexpr float xtick_sizes[] = {1, 5, 15, 60, 300, 900, 3600, 14400, 43200, 86400};

constexpr float round_xtick_size(float x, int n = 0)
{
    return n == sizeof(xtick_sizes) / sizeof(xtick_sizes[0]) ? xtick_sizes[n-1] : x < xtick_sizes[n] ? xtick_sizes[n] : round_xtick_size(x, n+1);
}

float round_ytick_size(float x)
{
    if (x <= 0 || isnan(x))
        return 0;

    float ret = 1;

    while(x < 1)
    {
        ret = ret / 10;
        x = x * 10;
    }

    while(x > 1)
    {
        ret = ret * 10;
        x = x / 10;
    }

    return ret;
}

QmlPlot::QmlPlot(QQuickItem * parent) :
    QQuickPaintedItem(parent),
    _background(QColor(255,255,255,40)),
    _xPen(Qt::gray),
    _yPen(Qt::gray)
{
}

void QmlPlot::setPadding(QMargins padding)
{
    _padding = padding;
    emit paddingChanged(padding);
    update();
}

void QmlPlot::resetTick()
{
    _xtick = round_xtick_size((_xmax - _xmin) / 10);
    _y1tick = round_ytick_size((_y1max - _y1min) / 10);
    _y2tick = round_ytick_size((_y2max - _y2min) / 10);
    emit xtickChanged(_xtick);
    emit y1tickChanged(_y1tick);
    emit y2tickChanged(_y2tick);
}

void QmlPlot::resetAxes()
{
    _xmax = std::numeric_limits<float>::lowest();
    _xmin = std::numeric_limits<float>::max();
    _y1max = std::numeric_limits<float>::lowest();
    _y1min = std::numeric_limits<float>::max();
    _y2max = std::numeric_limits<float>::lowest();
    _y2min = std::numeric_limits<float>::max();

    for(const QmlPlotData * i: findChildren<QmlPlotData *>())
    {
        for(const QVector2D& j: i->data())
        {
            _xmax = std::max(_xmax, j.x());
            _xmin = std::min(_xmin, j.x());
            if (i->axisId() == QmlPlotData::Left)
            {
                _y1min = std::min(_y1min, j.y());
                _y1max = std::max(_y1max, j.y());
            }
            else
            {
                _y2min = std::min(_y2min, j.y());
                _y2max = std::max(_y2max, j.y());
            }
        }
    }

    resetTick();

    _y1min = std::floor(_y1min / _y1tick) * _y1tick;
    _y1max = std::ceil(_y1max / _y1tick) * _y1tick;
    _y2min = std::floor(_y2min / _y2tick) * _y2tick;
    _y2max = std::ceil(_y2max / _y2tick) * _y2tick;

    emit xminChanged(_xmin);
    emit xmaxChanged(_xmax);
    emit y1minChanged(_y1min);
    emit y1maxChanged(_y1max);
    emit y2minChanged(_y2min);
    emit y2maxChanged(_y2max);

    update();
}

void QmlPlot::setXmin(float xmin)
{
    _xmin = xmin;
    resetTick();
    emit xminChanged(xmin);
    update();
}

void QmlPlot::setXmax(float xmax)
{
    _xmax = xmax;
    resetTick();
    emit xmaxChanged(xmax);
    update();
}

void QmlPlot::setY1min(float y1min)
{
    _y1min = y1min;
    resetTick();
    emit y1minChanged(y1min);
    update();
}

void QmlPlot::setY2min(float y2min)
{
    _y2min = y2min;
    resetTick();
    emit y2minChanged(y2min);
    update();
}

void QmlPlot::setY1max(float y1max)
{
    _y1max = y1max;
    resetTick();
    emit y1maxChanged(y1max);
    update();
}

void QmlPlot::setY2max(float y2max)
{
    _y2max = y2max;
    resetTick();
    emit y2maxChanged(y2max);
    update();
}

void QmlPlot::setXPen(QPen xPen)
{
    _xPen = xPen;
    emit xPenChanged(xPen);
    update();
}

void QmlPlot::setYPen(QPen yPen)
{
    _yPen = yPen;
    emit yPenChanged(yPen);
    update();
}

void QmlPlot::setBackground(QBrush background)
{
    _background = background;
    emit backgroundChanged(background);
    update();
}

void QmlPlot::paint(QPainter * painter)
{
    QRect win = painter->window();

    int w = win.width() - _padding.left() - _padding.right();
    int h = win.height() - _padding.top() - _padding.bottom();

    painter->setClipRect(_padding.left(), _padding.top(), w, h);

    painter->setBrush(_background);
    painter->setPen(Qt::NoPen);
    painter->drawRect(_padding.left(), _padding.top(), w, h);
    painter->setPen(Qt::SolidLine);

    if (_xtick > 0)
    {
        painter->setPen(_xPen);
        for(int i = _xmin / _xtick; i < _xmax / _xtick; ++i)
        {
            float x = i * _xtick;
            x = _padding.left() + w * (x- _xmin) / (_xmax - _xmin);
            painter->drawLine(x, _padding.top(), x, win.height() - _padding.bottom());
        }
    }

    if (_y1tick > 0)
    {
        painter->setPen(_yPen);
        for(int i = _y1min / _y1tick; i < _y1max / _y1tick; ++i)
        {
            float y = i * _y1tick;
            y = _padding.top() + h - h * (y - _y1min) / (_y1max - _y1min);
            painter->drawLine(_padding.left(), y, win.width() - _padding.right(), y);
        }
    }

    if (_y2tick > 0)
    {
        painter->setPen(_yPen);
        for(int i = _y2min / _y2tick; i < _y2max / _y2tick; ++i)
        {
            float y = i * _y2tick;
            y = _padding.top() + h - h * (y - _y2min) / (_y2max - _y2min);
            painter->drawLine(_padding.left(), y, win.width() - _padding.right(), y);
        }
    }

    for(const QmlPlotData * i: findChildren<QmlPlotData *>())
    {
        painter->setPen(i->colour());
        std::vector<QPoint> points(i->data().size());
        if (i->axisId() == QmlPlotData::Left)
        {
            for(const QVector2D& j: i->data())
            {
                points.emplace_back(_padding.left() + w * (j.x() - _xmin) / (_xmax - _xmin),
                                    _padding.top() + h - h * (j.y() - _y1min) / (_y1max - _y1min));
            }
        }
        else
        {
            for(const QVector2D& j: i->data())
            {
                points.emplace_back(_padding.left() + w * (j.x() - _xmin) / (_xmax - _xmin),
                                    _padding.top() + h - h * (j.y() - _y2min) / (_y2max - _y2min));
            }
        }

        painter->drawPolyline(points.data(), points.size());
    }
}
