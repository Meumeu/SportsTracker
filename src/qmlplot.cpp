#include "qmlplot.h"
#include <QPainter>
#include <limits>

void QmlPlot::append(QQmlListProperty<QmlPlotData> * prop, QmlPlotData * value)
{
    qobject_cast<QmlPlot *>(prop->object)->_data2.append(value);
    qobject_cast<QmlPlot *>(prop->object)->update();
}

QmlPlot::QmlPlot(QQuickItem * parent) :
    QQuickPaintedItem(parent),
    _data(this, nullptr, &QmlPlot::append, nullptr, nullptr, nullptr)
{
}

QQmlListProperty<QmlPlotData>& QmlPlot::data()
{
    return _data;
}

void QmlPlot::paint(QPainter * painter)
{
    float padding = 20;
    QRect win = painter->window();
    painter->setClipRect(padding, padding, win.width() - padding, win.height() - padding);

    int w = win.width() - 2 * padding;
    int h = win.height() - 2 * padding;

    float xmin = std::numeric_limits<float>::max();
    float xmax = std::numeric_limits<float>::lowest();
    float ymin = std::numeric_limits<float>::max();
    float ymax = std::numeric_limits<float>::lowest();

    for(const QmlPlotData * i: _data2)
    {
        for(const QVector2D& j: i->data())
        {
            xmin = std::min(xmin, j.x());
            xmax = std::max(xmax, j.x());
            ymin = std::min(ymin, j.y());
            ymax = std::max(ymax, j.y());
        }
    }

    for(const QmlPlotData * i: _data2)
    {
        painter->setPen(i->colour());
        std::vector<QPoint> points(i->data().size());
        for(const QVector2D& j: i->data())
        {
            points.emplace_back(padding + w * (j.x() - xmin) / (xmax - xmin),
                                padding + h - h * (j.y() - ymin) / (ymax - ymin));
        }
        painter->drawPolyline(points.data(), points.size());
    }
}
