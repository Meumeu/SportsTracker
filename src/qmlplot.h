#ifndef QMLPLOT_H
#define QMLPLOT_H

#include <QQuickPaintedItem>
#include <QRect>
#include <QBrush>
#include <QPen>
#include "qmlplotdata.h"

class QmlPlot : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int paddingLeft READ paddingLeft WRITE setPaddingLeft NOTIFY paddingLeftChanged)
    Q_PROPERTY(int paddingRight READ paddingRight WRITE setPaddingRight NOTIFY paddingRightChanged)
    Q_PROPERTY(int paddingTop READ paddingTop WRITE setPaddingTop NOTIFY paddingTopChanged)
    Q_PROPERTY(int paddingBottom READ paddingBottom WRITE setPaddingBottom NOTIFY paddingBottomChanged)
    Q_PROPERTY(float xmin READ xmin WRITE setXmin NOTIFY xminChanged)
    Q_PROPERTY(float xmax READ xmax WRITE setXmax NOTIFY xmaxChanged)
    Q_PROPERTY(float y1min READ y1min WRITE setY1min NOTIFY y1minChanged)
    Q_PROPERTY(float y1max READ y1max WRITE setY1max NOTIFY y1maxChanged)
    Q_PROPERTY(float y2min READ y2min WRITE setY2min NOTIFY y2minChanged)
    Q_PROPERTY(float y2max READ y2max WRITE setY2max NOTIFY y2maxChanged)
    Q_PROPERTY(float xtick READ xtick NOTIFY xtickChanged)
    Q_PROPERTY(float y1tick READ y1tick NOTIFY y1tickChanged)
    Q_PROPERTY(float y2tick READ y2tick NOTIFY y2tickChanged)
    Q_PROPERTY(int tickLength READ tickLength WRITE setTickLength NOTIFY tickLengthChanged)
    Q_PROPERTY(QBrush background READ background WRITE setBackground NOTIFY backgroundChanged)
    Q_PROPERTY(QPen xPen READ xPen WRITE setXPen NOTIFY xPenChanged)
    Q_PROPERTY(QPen yPen READ yPen WRITE setYPen NOTIFY yPenChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor textColour READ textColour WRITE setTextColour NOTIFY textColourChanged)
    Q_PROPERTY(bool gesturing READ gesturing NOTIFY gesturingChanged)

    int _paddingLeft, _paddingRight, _paddingTop, _paddingBottom;
    float _xmin, _xmax, _xtick;
    float _y1min, _y1max, _y1tick;
    float _y2min, _y2max, _y2tick;
    QBrush _background;
    QPen _xPen, _yPen;
    int _tickLength;
    QFont _font;
    QColor _textColour;
    bool _gesturing;

    int _width, _height;
    float _dataxmin, _dataxmax;
    float _datay1min, _datay1max;
    float _datay2min, _datay2max;

    void resetTick();
public:
    explicit QmlPlot(QQuickItem * parent = 0);

    void paint(QPainter * painter);

    int paddingLeft() const { return _paddingLeft; }
    int paddingRight() const { return _paddingRight; }
    int paddingTop() const { return _paddingTop; }
    int paddingBottom() const { return _paddingBottom; }
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
    int tickLength() const { return _tickLength; }
    QFont font() const { return _font; }
    QColor textColour() const { return _textColour; }
    bool gesturing() const { return _gesturing; }

    void setPaddingLeft(int paddingLeft);
    void setPaddingRight(int paddingRight);
    void setPaddingTop(int paddingTop);
    void setPaddingBottom(int paddingBottom);
    void setXmin(float xmin);
    void setXmax(float xmax);
    void setY1min(float y1min);
    void setY1max(float y1max);
    void setY2min(float y2min);
    void setY2max(float y2max);
    void setBackground(QBrush background);
    void setXPen(QPen xPen);
    void setYPen(QPen yPen);
    void setTickLength(int tickLength);
    void setFont(QFont font);
    void setTextColour(QColor textColour);

    Q_INVOKABLE void resetAxes();

private:
    void setGesturing(bool gesturing);

protected:
    void touchEvent(QTouchEvent *event);

signals:
    void paddingLeftChanged(int paddingLeft);
    void paddingRightChanged(int paddingRight);
    void paddingTopChanged(int paddingTop);
    void paddingBottomChanged(int paddingBottom);
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
    void tickLengthChanged(int tickLength);
    void fontChanged(QFont font);
    void textColourChanged(QColor textColour);
    void gesturingChanged(bool gesturing);

public slots:

};

#endif // QMLPLOT_H
