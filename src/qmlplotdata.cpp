#include "qmlplotdata.h"

QmlPlotData::QmlPlotData(QObject *parent) :
    QObject(parent),
    _colour(Qt::white)
{
}

void QmlPlotData::setColour(QColor colour)
{
    _colour = colour;
    emit colourChanged(_colour);
}

QColor QmlPlotData::colour() const
{
    return _colour;
}

void QmlPlotData::setData(const std::vector<QVector2D>& data)
{
    _data = data;
    emit dataChanged(_data);
}

void QmlPlotData::setData(std::vector<QVector2D>&& data)
{
    _data = std::move(data);
    emit dataChanged(_data);
}

const std::vector<QVector2D>& QmlPlotData::data() const
{
    return _data;
}
