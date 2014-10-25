#ifndef QMLPLOTDATASOURCE_H
#define QMLPLOTDATASOURCE_H

#include <QObject>
#include <QVector2D>

class QmlPlotDataSource : public QObject
{
    Q_OBJECT
private:
    std::vector<QVector2D> _data;
    Q_PROPERTY(std::vector<QVector2D> data READ data NOTIFY dataChanged)

public:
    explicit QmlPlotDataSource(QObject *parent = 0);

    const std::vector<QVector2D>& data() const { return _data; }
    void setData(std::vector<QVector2D>&& data);

signals:
    void dataChanged(const std::vector<QVector2D>& data);

public slots:

};

#endif // QMLPLOTDATASOURCE_H
