#ifndef QMLPLOT_H
#define QMLPLOT_H

#include <QQuickPaintedItem>
#include "qmlplotdata.h"

class QmlPlot : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QmlPlotData> data READ data)

    QQmlListProperty<QmlPlotData> _data;
    QList<QmlPlotData *> _data2;

    static void append(QQmlListProperty<QmlPlotData> * prop, QmlPlotData * value);
    /*static QmlPlotData * at(QQmlListProperty<QmlPlotData> * prop, int index);
    static int count(QQmlListProperty<QmlPlotData> * prop);
    static void clear(QQmlListProperty<QmlPlotData> * prop);*/

public:
    explicit QmlPlot(QQuickItem * parent = 0);

    void paint(QPainter * painter);
    QQmlListProperty<QmlPlotData>& data();

signals:

public slots:

};

#endif // QMLPLOT_H
