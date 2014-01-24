#ifndef QMLPLOT_H
#define QMLPLOT_H

#include <QQuickPaintedItem>
#include <QMargins>
#include "qmlplotdata.h"

class QmlPlot : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QMargins padding READ padding WRITE setPadding NOTIFY paddingChanged)
    Q_PROPERTY(float xmin READ xmin WRITE setXmin RESET resetXmin NOTIFY xminChanged)
    Q_PROPERTY(float xmax READ xmax WRITE setXmax RESET resetXmax NOTIFY xmaxChanged)
    Q_PROPERTY(float xtick READ xtick WRITE setXtick RESET resetXtick NOTIFY xtickChanged)
    Q_PROPERTY(float xoffset READ xoffset WRITE setXoffset NOTIFY xoffsetChanged)

    QMargins _padding;
    float _xmin, _xmax, _xtick, _xoffset;

public:
    explicit QmlPlot(QQuickItem * parent = 0);

    void paint(QPainter * painter);

    QMargins padding() const { return _padding; }
    float xmin() const { return _xmin; }
    float xmax() const { return _xmax; }
    float xtick() const { return _xtick; }
    float xoffset() const { return _xoffset; }

    void setPadding(QMargins padding);
    void setXmin(float xmin);
    void setXmax(float xmax);
    void setXtick(float xtick);
    void setXoffset(float xoffset);

    void resetXmin();
    void resetXmax();
    void resetXtick();

signals:
    void paddingChanged(QMargins padding);
    void xminChanged(float xmin);
    void xmaxChanged(float xmax);
    void xtickChanged(float xtick);
    void xoffsetChanged(float xoffset);
public slots:

};

#endif // QMLPLOT_H
