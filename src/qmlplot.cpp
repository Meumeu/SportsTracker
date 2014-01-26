#include "qmlplot.h"
#include <QPainter>
#include <limits>
#include <iostream>

static constexpr float xtick_sizes[] = {1, 5, 15, 60, 300, 900, 3600, 14400, 43200, 86400};

constexpr float round_xtick_size(float x, int n = 0)
{
    return n == sizeof(xtick_sizes) / sizeof(xtick_sizes[0]) ? xtick_sizes[n-1] : x < xtick_sizes[n] ? xtick_sizes[n] : round_xtick_size(x, n+1);
}

float round_ytick_size(float x)
{
    if (x <= 0 || isnan(x))
        return 0;

    float ret = 1;

    while(x < 1)
    {
        ret = ret / 10;
        x = x * 10;
    }

    while(x > 1)
    {
        ret = ret * 10;
        x = x / 10;
    }

    return ret;
}

QmlPlot::QmlPlot(QQuickItem * parent) :
    QQuickPaintedItem(parent),
    _background(QColor(255,255,255,40)),
    _xPen(Qt::gray),
    _yPen(Qt::gray),
    _tickLength(0),
    _textColour(Qt::white),
    _gesturing(false)
{
}

void QmlPlot::setPaddingLeft(int paddingLeft)
{
    _paddingLeft = paddingLeft;
    emit paddingLeftChanged(paddingLeft);
    update();
}

void QmlPlot::setPaddingRight(int paddingRight)
{
    _paddingRight = paddingRight;
    emit paddingRightChanged(paddingRight);
    update();
}

void QmlPlot::setPaddingTop(int paddingTop)
{
    _paddingTop = paddingTop;
    emit paddingTopChanged(paddingTop);
    update();
}

void QmlPlot::setPaddingBottom(int paddingBottom)
{
    _paddingBottom = paddingBottom;
    emit paddingBottomChanged(paddingBottom);
    update();
}

void QmlPlot::resetTick()
{
    _xtick = round_xtick_size((_xmax - _xmin) / 10);
    _y1tick = round_ytick_size((_y1max - _y1min) / 10);
    _y2tick = round_ytick_size((_y2max - _y2min) / 10);
    emit xtickChanged(_xtick);
    emit y1tickChanged(_y1tick);
    emit y2tickChanged(_y2tick);
}

void QmlPlot::resetAxes()
{
    _xmax = std::numeric_limits<float>::lowest();
    _xmin = std::numeric_limits<float>::max();
    _y1max = std::numeric_limits<float>::lowest();
    _y1min = std::numeric_limits<float>::max();
    _y2max = std::numeric_limits<float>::lowest();
    _y2min = std::numeric_limits<float>::max();

    for(const QmlPlotData * i: findChildren<QmlPlotData *>())
    {
        for(const QVector2D& j: i->data())
        {
            _xmax = std::max(_xmax, j.x());
            _xmin = std::min(_xmin, j.x());
            if (i->axisId() == QmlPlotData::Left)
            {
                _y1min = std::min(_y1min, j.y());
                _y1max = std::max(_y1max, j.y());
            }
            else
            {
                _y2min = std::min(_y2min, j.y());
                _y2max = std::max(_y2max, j.y());
            }
        }
    }

    resetTick();

    _y1min = std::floor(_y1min / _y1tick) * _y1tick;
    _y1max = std::ceil(_y1max / _y1tick) * _y1tick;
    _y2min = std::floor(_y2min / _y2tick) * _y2tick;
    _y2max = std::ceil(_y2max / _y2tick) * _y2tick;

    _dataxmin = _xmin;
    _dataxmax = _xmax;
    _datay1min = _y1min;
    _datay1max = _y1max;
    _datay2min = _y2min;
    _datay2max = _y2max;

    emit xminChanged(_xmin);
    emit xmaxChanged(_xmax);
    emit y1minChanged(_y1min);
    emit y1maxChanged(_y1max);
    emit y2minChanged(_y2min);
    emit y2maxChanged(_y2max);

    update();
}

void QmlPlot::setXmin(float xmin)
{
    _xmin = xmin;
    resetTick();
    emit xminChanged(xmin);
    update();
}

void QmlPlot::setXmax(float xmax)
{
    _xmax = xmax;
    resetTick();
    emit xmaxChanged(xmax);
    update();
}

void QmlPlot::setY1min(float y1min)
{
    _y1min = y1min;
    resetTick();
    emit y1minChanged(y1min);
    update();
}

void QmlPlot::setY2min(float y2min)
{
    _y2min = y2min;
    resetTick();
    emit y2minChanged(y2min);
    update();
}

void QmlPlot::setY1max(float y1max)
{
    _y1max = y1max;
    resetTick();
    emit y1maxChanged(y1max);
    update();
}

void QmlPlot::setY2max(float y2max)
{
    _y2max = y2max;
    resetTick();
    emit y2maxChanged(y2max);
    update();
}

void QmlPlot::setXPen(QPen xPen)
{
    _xPen = xPen;
    emit xPenChanged(xPen);
    update();
}

void QmlPlot::setYPen(QPen yPen)
{
    _yPen = yPen;
    emit yPenChanged(yPen);
    update();
}

void QmlPlot::setTickLength(int tickLength)
{
    _tickLength = tickLength;
    emit tickLengthChanged(tickLength);
    update();
}

void QmlPlot::setBackground(QBrush background)
{
    _background = background;
    emit backgroundChanged(background);
    update();
}

void QmlPlot::setFont(QFont font)
{
    _font = font;
    emit fontChanged(font);
    update();
}

void QmlPlot::setTextColour(QColor textColour)
{
    _textColour = textColour;
    emit textColourChanged(textColour);
    update();
}

void QmlPlot::setGesturing(bool gesturing)
{
    if (gesturing != _gesturing)
    {
        _gesturing = gesturing;
        emit gesturingChanged(_gesturing);
    }
}

static QString time_to_string(float t, float tick)
{
    int h = t / 3600;
    int m = static_cast<int>(t / 60) % 60;
    int s = static_cast<int>(t) % 60;

    if (tick < 60)
    {
        return QString("%1:%2:%3").arg(h).arg(QString::number(m), 2, '0').arg(QString::number(s), 2, '0');
    }
    else if (tick < 3600)
    {
        return QString("%1:%2").arg(h).arg(QString::number(m), 2, '0');
    }
    else
    {
        return QString::number(h) + "h";
    }
}

static void compute_gesture(float pos1, float pos2, float newpos1, float newpos2, float posmin, float posmax, float& min, float& max)
{
    if (pos1 > pos2)
        std::swap(pos1, pos2);

    if (newpos1 > newpos2)
        std::swap(newpos1, newpos2);

    float val1 = (pos1 - posmin) / (posmax - posmin) * (max - min) + min;
    float val2 = (pos2 - posmin) / (posmax - posmin) * (max - min) + min;

    float tmp = (posmax - posmin) / (newpos1 - newpos2) * (val1 - val2);
    if (tmp > 0)
    {
        min = val1 - (newpos1 - posmin) / (posmax - posmin) * tmp;
        max = min + tmp;
    }
}

static void compute_gesture(float pos, float newpos, float posmin, float posmax, float& min, float& max)
{
    float val = (pos - posmin) / (posmax - posmin) * (max - min) + min;

    float tmp = max - min;
    min = val - (newpos - posmin) / (posmax - posmin) * tmp;
    max = min + tmp;
}

void QmlPlot::touchEvent(QTouchEvent *event)
{
    QVector<int> ids;
    switch(event->type())
    {
    case QEvent::TouchBegin:
        for(QTouchEvent::TouchPoint i: event->touchPoints())
        {
            ids.push_back(i.id());
        }
        grabTouchPoints(ids);
        setKeepTouchGrab(true);
        event->accept();
        setGesturing(true);
        break;

    case QEvent::TouchUpdate:
        if (event->touchPoints().count() == 1)
        {
            QPointF pt1 = event->touchPoints()[0].pos();
            QPointF pt1_prev = event->touchPoints()[0].lastPos();

            compute_gesture(pt1_prev.x(), pt1.x(), _paddingLeft, _width - _paddingLeft - _paddingRight, _xmin, _xmax);
            if (_xmin < _dataxmin)
            {
                _xmax = _dataxmin + _xmax - _xmin;
                _xmin = _dataxmin;
            }
            else if (_xmax > _dataxmax)
            {
                _xmin = _dataxmax + _xmin - _xmax;
                _xmax = _dataxmax;
            }
            emit xminChanged(_xmin);
            emit xmaxChanged(_xmax);

            update();
        }
        else if (event->touchPoints().count() == 2)
        {
            QPointF pt1 = event->touchPoints()[0].pos();
            QPointF pt2 = event->touchPoints()[1].pos();
            QPointF pt1_prev = event->touchPoints()[0].lastPos();
            QPointF pt2_prev = event->touchPoints()[1].lastPos();

            compute_gesture(pt1_prev.x(), pt2_prev.x(), pt1.x(), pt2.x(), _paddingLeft, _width - _paddingLeft - _paddingRight, _xmin, _xmax);
            _xmin = std::max(_xmin, _dataxmin);
            _xmax = std::min(_xmax, _dataxmax);
            emit xminChanged(_xmin);
            emit xmaxChanged(_xmax);

            resetTick();
            update();
        }
        break;

    case QEvent::TouchCancel:
    case QEvent::TouchEnd:
        setGesturing(false);
        break;

    default:
        break;
    }
}

void QmlPlot::paint(QPainter * painter)
{
    QRect win = painter->window();

    _width = win.width();
    _height = win.height();

    int w = win.width() - _paddingLeft - _paddingRight;
    int h = win.height() - _paddingTop - _paddingBottom;

    painter->setBrush(_background);
    painter->setFont(_font);
    painter->setPen(Qt::NoPen);
    painter->drawRect(_paddingLeft, _paddingTop, w, h);
    painter->setPen(Qt::SolidLine);

    if (_xtick > 0)
    {
        for(int i = std::ceil(_xmin / _xtick); i <= _xmax / _xtick; ++i)
        {
            float t = i * _xtick;
            float x = _paddingLeft + w * (t- _xmin) / (_xmax - _xmin);
            painter->setPen(_xPen);
            painter->drawLine(x, win.height() - _paddingBottom - _tickLength, x, win.height() - _paddingBottom);
            painter->setPen(_textColour);
            painter->drawText(x, win.height() - _paddingBottom, 0, 0,
                              Qt::AlignHCenter | Qt::AlignTop | Qt::TextDontClip,
                              time_to_string(t, _xtick));
        }
    }

    if (_y1tick > 0)
    {
        for(int i = std::ceil(_y1min / _y1tick); i <= _y1max / _y1tick; ++i)
        {
            float value = i * _y1tick;
            float y = _paddingTop + h - h * (value - _y1min) / (_y1max - _y1min);
            painter->setPen(_yPen);
            painter->drawLine(_paddingLeft, y, _paddingLeft + _tickLength, y);
            painter->setPen(_textColour);
            painter->drawText(_paddingLeft - _font.pixelSize()/4, y, 0, 0,
                              Qt::AlignVCenter | Qt::AlignRight | Qt::TextDontClip,
                              QString::number(value));
        }
    }

    if (_y2tick > 0)
    {
        for(int i = std::ceil(_y2min / _y2tick); i <= _y2max / _y2tick; ++i)
        {
            float value = i * _y2tick;
            float y = _paddingTop + h - h * (value - _y2min) / (_y2max - _y2min);
            painter->setPen(_yPen);
            painter->drawLine(win.width() - _paddingRight - _tickLength, y, win.width() - _paddingRight, y);
            painter->setPen(_textColour);
            painter->drawText(win.width() - _paddingRight + _font.pixelSize()/4, y, 0, 0,
                              Qt::AlignVCenter | Qt::AlignLeft | Qt::TextDontClip,
                              QString::number(value));
        }
    }

    painter->setClipRect(_paddingLeft, 0, w, h);
    for(const QmlPlotData * i: findChildren<QmlPlotData *>())
    {
        painter->setPen(i->colour());
        std::vector<QPoint> points;
        points.reserve(i->data().size());
        if (i->axisId() == QmlPlotData::Left)
        {
            for(const QVector2D& j: i->data())
            {
                points.emplace_back(_paddingLeft + w * (j.x() - _xmin) / (_xmax - _xmin),
                                    _paddingTop + h - h * (j.y() - _y1min) / (_y1max - _y1min));
            }
        }
        else
        {
            for(const QVector2D& j: i->data())
            {
                points.emplace_back(_paddingLeft + w * (j.x() - _xmin) / (_xmax - _xmin),
                                    _paddingTop + h - h * (j.y() - _y2min) / (_y2max - _y2min));
            }
        }

        painter->drawPolyline(points.data(), points.size());
    }
}
