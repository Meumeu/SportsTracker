#ifndef QMLPLOTDATA_H
#define QMLPLOTDATA_H

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
    std::vector<QVector2D> _data;
    AxisId _axisId;

    Q_ENUMS(AxisId)
    Q_PROPERTY(QColor colour READ colour WRITE setColour NOTIFY colourChanged)
    Q_PROPERTY(std::vector<QVector2D> data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(AxisId axisId READ axisId WRITE setAxisId NOTIFY axisIdChanged)

    QmlPlot * findPlot();
    void updatePlot();
    void resetPlotAxes();

public:
    explicit QmlPlotData(QObject *parent = 0);

    QColor colour() const { return _colour; }
    const std::vector<QVector2D>& data() const { return _data; }
    AxisId axisId() const { return _axisId; }

    void setColour(QColor colour);
    void setData(const std::vector<QVector2D>& data);
    void setData(std::vector<QVector2D>&& data);
    void setAxisId(AxisId axisId);

signals:
    void colourChanged(QColor colour);
    void dataChanged(const std::vector<QVector2D>& data);
    void axisIdChanged(AxisId axisId);

public slots:

};

#endif // QMLPLOTDATA_H
