#ifndef QMLPLOTDATA_H
#define QMLPLOTDATA_H

#include "qmlplotdatasource.h"

#include <QObject>
#include <QColor>
#include <QVector2D>
#include <vector>

class QmlPlot;

class QmlPlotData : public QObject
{
    Q_OBJECT
public:
    enum AxisId {Left, Right};

private:
    QColor _colour;
    QmlPlotDataSource * _source;
    AxisId _axisId;

    Q_ENUMS(AxisId)
    Q_PROPERTY(QColor colour READ colour WRITE setColour NOTIFY colourChanged)
    Q_PROPERTY(std::vector<QVector2D> data READ data NOTIFY dataChanged)
    Q_PROPERTY(AxisId axisId READ axisId WRITE setAxisId NOTIFY axisIdChanged)
    Q_PROPERTY(QmlPlotDataSource * source READ source WRITE setSource NOTIFY sourceChanged)

    QmlPlot * findPlot();
    void updatePlot();
    void resetPlotAxes();
    void sourceDataChanged(const std::vector<QVector2D>& data);
    void sourceDestroyed();

    static std::vector<QVector2D> _empty_data;

public:
    explicit QmlPlotData(QObject *parent = 0);

    QColor colour() const { return _colour; }
    const std::vector<QVector2D>& data() const { return _source ? _source->data() : _empty_data; }
    AxisId axisId() const { return _axisId; }
    QmlPlotDataSource * source() const { return _source; }

    void setColour(QColor colour);
    void setAxisId(AxisId axisId);
    void setSource(QmlPlotDataSource * source);

signals:
    void colourChanged(QColor colour);
    void dataChanged(const std::vector<QVector2D>& data);
    void axisIdChanged(AxisId axisId);
    void sourceChanged(QmlPlotDataSource * source);

public slots:

};

#endif // QMLPLOTDATA_H
