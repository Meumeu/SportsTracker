#ifndef QMLPLOTDATA_H
#define QMLPLOTDATA_H

#include <QObject>
#include <QColor>
#include <QVector2D>
#include <vector>

class QmlPlotData : public QObject
{
    Q_OBJECT
    QColor _colour;
    std::vector<QVector2D> _data;

    Q_PROPERTY(QColor colour READ colour WRITE setColour NOTIFY colourChanged)
    Q_PROPERTY(std::vector<QVector2D> data READ data WRITE setData NOTIFY dataChanged)

public:
    explicit QmlPlotData(QObject *parent = 0);

    QColor colour() const;
    const std::vector<QVector2D>& data() const;

    void setColour(QColor colour);
    void setData(const std::vector<QVector2D>& data);
    void setData(std::vector<QVector2D>&& data);

signals:
    void colourChanged(QColor colour);
    void dataChanged(const std::vector<QVector2D>& data);

public slots:

};

#endif // QMLPLOTDATA_H
