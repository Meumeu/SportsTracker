#include "qmlplotdata.h"
#include "qmlplot.h"

std::vector<QVector2D> QmlPlotData::_empty_data;

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

void QmlPlotData::setSource(QmlPlotDataSource * source)
{
    if (source != _source)
    {
        if (_source)
        {
            disconnect(_source, &QmlPlotDataSource::dataChanged, this, &QmlPlotData::sourceDataChanged);
            disconnect(_source, &QmlPlotDataSource::destroyed, this, &QmlPlotData::sourceDestroyed);
        }

        if (source)
        {
            connect(source, &QmlPlotDataSource::dataChanged, this, &QmlPlotData::sourceDataChanged);
            connect(source, &QmlPlotDataSource::destroyed, this, &QmlPlotData::sourceDestroyed);
        }

        _source = source;
        emit sourceChanged(source);
        emit dataChanged(data());

        resetPlotAxes();
    }
}

void QmlPlotData::sourceDataChanged(const std::vector<QVector2D> &data)
{
    emit dataChanged(data);

    resetPlotAxes();
}

void QmlPlotData::sourceDestroyed()
{
    setSource(nullptr);
}
