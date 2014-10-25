#include "qmlplotdatasource.h"

QmlPlotDataSource::QmlPlotDataSource(QObject *parent) :
    QObject(parent)
{
}

void QmlPlotDataSource::setData(std::vector<QVector2D> &&data)
{
    _data = std::move(data);
    emit dataChanged(_data);
}
