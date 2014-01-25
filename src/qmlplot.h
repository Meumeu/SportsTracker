#ifndef QMLPLOT_H
#define QMLPLOT_H

#include <QQuickPaintedItem>
#include <QMargins>
#include <QBrush>
#include <QPen>
#include "qmlplotdata.h"

class QmlPlot : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QMargins padding READ padding WRITE setPadding NOTIFY paddingChanged)
    Q_PROPERTY(float xmin READ xmin WRITE setXmin NOTIFY xminChanged)
    Q_PROPERTY(float xmax READ xmax WRITE setXmax NOTIFY xmaxChanged)
    Q_PROPERTY(float y1min READ y1min WRITE setY1min NOTIFY y1minChanged)
    Q_PROPERTY(float y1max READ y1max WRITE setY1max NOTIFY y1maxChanged)
    Q_PROPERTY(float y2min READ y2min WRITE setY2min NOTIFY y2minChanged)
    Q_PROPERTY(float y2max READ y2max WRITE setY2max NOTIFY y2maxChanged)
    Q_PROPERTY(float xtick READ xtick NOTIFY xtickChanged)
    Q_PROPERTY(float y1tick READ y1tick NOTIFY y1tickChanged)
    Q_PROPERTY(float y2tick READ y2tick NOTIFY y2tickChanged)
    Q_PROPERTY(QBrush background READ background WRITE setBackground NOTIFY backgroundChanged)
    Q_PROPERTY(QPen xPen READ xPen WRITE setXPen NOTIFY xPenChanged)
    Q_PROPERTY(QPen yPen READ yPen WRITE setYPen NOTIFY yPenChanged)

    QMargins _padding;
    float _xmin, _xmax, _xtick;
    float _y1min, _y1max, _y1tick;
    float _y2min, _y2max, _y2tick;
    QBrush _background;
    QPen _xPen, _yPen;

    void resetTick();
public:
    explicit QmlPlot(QQuickItem * parent = 0);

    void paint(QPainter * painter);

    QMargins padding() const { return _padding; }
    float xmin() const { return _xmin; }
    float xmax() const { return _xmax; }
    float xtick() const { return _xtick; }
    float y1min() const { return _y1min; }
    float y1max() const { return _y1max; }
    float y1tick() const { return _y1tick; }
    float y2min() const { return _y2min; }
    float y2max() const { return _y2max; }
    float y2tick() const { return _y2tick; }
    QBrush background() const { return _background; }
    QPen xPen() const { return _xPen; }
    QPen yPen() const { return _yPen; }

    void setPadding(QMargins padding);
    void setXmin(float xmin);
    void setXmax(float xmax);
    void setY1min(float y1min);
    void setY1max(float y1max);
    void setY2min(float y2min);
    void setY2max(float y2max);
    void setBackground(QBrush background);
    void setXPen(QPen xPen);
    void setYPen(QPen yPen);

    Q_INVOKABLE void resetAxes();

signals:
    void paddingChanged(QMargins padding);
    void xminChanged(float xmin);
    void xmaxChanged(float xmax);
    void y1minChanged(float y1min);
    void y1maxChanged(float y1max);
    void y2minChanged(float y2min);
    void y2maxChanged(float y2max);
    void xtickChanged(float xtick);
    void y1tickChanged(float y1tick);
    void y2tickChanged(float y2tick);
    void backgroundChanged(QBrush background);
    void xPenChanged(QPen xPen);
    void yPenChanged(QPen yPen);

public slots:

};

#endif // QMLPLOT_H
