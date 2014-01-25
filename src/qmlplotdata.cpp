#include "qmlplotdata.h"
#include "qmlplot.h"

QmlPlotData::QmlPlotData(QObject *parent) :
    QObject(parent),
    _colour(Qt::black)
{
}

QmlPlot * QmlPlotData::findPlot()
{
    for(QObject * obj = parent(); obj; obj = obj->parent())
    {
        QmlPlot * plot = qobject_cast<QmlPlot *>(obj);
        if (plot)
        {
            return plot;
        }
    }

    return nullptr;
}

void QmlPlotData::updatePlot()
{
    QmlPlot * plot = findPlot();

    if (plot) plot->update();
}

void QmlPlotData::resetPlotAxes()
{
    QmlPlot * plot = findPlot();

    if (plot) plot->resetAxes();
}

void QmlPlotData::setColour(QColor colour)
{
    _colour = colour;
    emit colourChanged(_colour);
    updatePlot();
}

void QmlPlotData::setAxisId(AxisId axisId)
{
    _axisId = axisId;
    emit axisIdChanged(axisId);
    resetPlotAxes();
}

void QmlPlotData::setData(const std::vector<QVector2D>& data)
{
    _data = data;
    emit dataChanged(_data);
    resetPlotAxes();
}

void QmlPlotData::setData(std::vector<QVector2D>&& data)
{
    _data = std::move(data);
    emit dataChanged(_data);
    resetPlotAxes();
}

