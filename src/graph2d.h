#pragma once
#ifndef GRAPH2D_H
#define GRAPH2D_H

#include <QFontMetrics>
#include <QColor>
#include <QMap>
#include <QPoint>
#include <QPointF>
#include <axes2d.h>

template<typename ty>
class Graph2D: public Axes2D
{
public:
    Graph2D(QWidget* parent = nullptr);
    virtual ~Graph2D();

    int addCurve(QVector<ty> const& y, QColor const& color = defaultColor);
    int addCurve(QVector<ty> const& x, QVector<ty> const& y, QColor const& color = defaultColor);
    int addCurve(QVector<QPointF> const& xy, QColor const& color = defaultColor);

    void removeCurve(int id);

    void setColor(int id, QColor const& color);
    void setStyle(int id, Qt::PenStyle style);
    void setWidth(int id, int width);

protected:
    /*!
     * \brief The Curve2D struct
     */
    struct Curve2D
    {
        Curve2D():
            _pen(Qt::black, 1, Qt::SolidLine)
        {

        }

        QVector<QPointF> _points;
        QPen _pen;

    };

    static double niceNumber(double number, bool round);

    void drawGrid(QPainter& painter);
    void drawCurves(QPainter& painter);

    int getNonexistingId()const;

    static QColor const defaultColor;
    static int const _numTicks = 10;

    QFontMetrics _metrics;

    QMap<int, Curve2D> _curves;

    double _stepX;
    double _stepY;

};

template<typename ty>
QColor const Graph2D<ty>::defaultColor = QColor(255, 127, 39);

template<typename ty>
Graph2D<ty>::Graph2D(QWidget* parent):
    Axes2D(parent), _metrics(font())
{
    double const rangeX = niceNumber(_viewRegion.spanX(), false);
    double const rangeY = niceNumber(_viewRegion.spanY(), false);

    _stepX = niceNumber(rangeX / (_numTicks - 1), true);
    _stepY = niceNumber(rangeY / (_numTicks - 1), true);

    int const contentsLeft = 10 * _metrics.averageCharWidth();
    int const contentsTop = 3 * _metrics.lineSpacing();
    int const contentsRight = 5 * _metrics.averageCharWidth();
    int const contentsBottom = 3 * _metrics.lineSpacing();

    setContentsMargins(contentsLeft, contentsTop, contentsRight, contentsBottom);

    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawGrid));
    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawCurves));
}

template<typename ty>
Graph2D<ty>::~Graph2D()
{

}

template<typename ty>
int Graph2D<ty>::getNonexistingId()const
{
    int counter = 0;

    // Ключи в карте отсортированы в порядке возрастания. Кривые могут быть
    // добавлены и удалены во время жизни объекта. Если кривая была удалена,
    // образуется пустое место. Функция ищет пустое место.

    for(auto iter = _curves.cbegin(); iter != _curves.cend(); ++iter)
    {
        if(iter.key() != counter)
        {
            return counter;
        }

        ++counter;
    }

    return counter;
}

template<typename ty>
int Graph2D<ty>::addCurve(QVector<ty> const& y, QColor const& color)
{
    int const id = getNonexistingId();

    if(id >= 0)
    {
        Curve2D& curve = _curves[id];

        auto const size = y.size();
        curve._points.resize(size);

        for(auto i = 0; i < size; ++i)
        {
            curve._points[i].setX(i);
            curve._points[i].setY(y[i]);
        }

        curve._pen.setColor(color);
    }

    return id;
}

template<typename ty>
int Graph2D<ty>::addCurve(QVector<ty> const& x, QVector<ty> const& y, QColor const& color)
{
    int const id = getNonexistingId();

    if(id >= 0)
    {
        Curve2D& curve = _curves[id];

        auto const size = std::min(x.size(), y.size());
        curve._points.resize(size);

        for(auto i = 0; i < size; ++i)
        {
            curve._points[i].setX(x[i]);
            curve._points[i].setY(y[i]);
        }

        curve._pen.setColor(color);
    }

    return id;
}

template<typename ty>
int Graph2D<ty>::addCurve(QVector<QPointF> const& xy, QColor const& color)
{
    int const id = getNonexistingId();

    if(id >= 0)
    {
        Curve2D& curve = _curves[id];

        curve._points = xy;
        curve._pen.setColor(color);
    }

    return id;
}

template<typename ty>
void Graph2D<ty>::removeCurve(int id)
{
    _curves.remove(id);
    update();
}

template<typename ty>
void Graph2D<ty>::setColor(int id, QColor const& color)
{
    if(_curves.contains(id))
    {
        _curves[id]._pen.setColor(color);
        update();
    }
}

template<typename ty>
void Graph2D<ty>::setStyle(int id, Qt::PenStyle style)
{
    if(_curves.contains(id))
    {
        _curves[id]._pen.setStyle(style);
        update();
    }
}

template<typename ty>
void Graph2D<ty>::setWidth(int id, int width)
{
    if(_curves.contains(id))
    {
        _curves[id]._pen.setWidth(width);
        update();
    }
}

template<typename ty>
void Graph2D<ty>::drawGrid(QPainter& painter)
{
    QPen darkPen(Qt::black, 1, Qt::SolidLine);
    QPen lightPen(Qt::gray, 1, Qt::DashLine);

    double x = std::floor(_viewRegion._minX / _stepX) * _stepX;
    double y = 0.0;

    for(int i = 0; i < _numTicks; ++i)
    {
        x += _stepX;

        QPoint const pixel = toPixel(QPointF(x, y));

        painter.setPen(lightPen);
        painter.drawLine(pixel.x(), contentsRect().top(), pixel.x(), contentsRect().bottom());

        if(true)
        {
            QString const number = QString::number(x);
            int const lineWidth = _metrics.width(number);

            QRect const rect = {
                pixel.x() - lineWidth / 2,
                contentsRect().bottom() + _metrics.xHeight(),
                lineWidth,
                _metrics.lineSpacing()
            };

            painter.setPen(darkPen);
            painter.drawText(rect, Qt::AlignLeft | Qt::AlignTop, number);
        }
    }

    x = 0.0;
    y = std::floor(_viewRegion._minY / _stepY) * _stepY;

    for(int i = 0; i < _numTicks; ++i)
    {
        y += _stepY;

        QPoint const pixel = toPixel(QPointF(x, y));

        painter.setPen(lightPen);
        painter.drawLine(contentsRect().left(), pixel.y(), contentsRect().right(), pixel.y());

        if(true)
        {
            QString const number = QString::number(y);
            int const lineWidth = _metrics.width(number);

            QRect const rect {
                contentsRect().left() - lineWidth - _metrics.averageCharWidth(),
                pixel.y() - _metrics.lineSpacing() / 2,
                lineWidth,
                _metrics.lineSpacing()
            };

            painter.setPen(darkPen);
            painter.drawText(rect, Qt::AlignRight | Qt::AlignTop, number);
        }
    }

    painter.setPen(darkPen);
    painter.drawRect(contentsRect().adjusted(0, 0, -1, -1));
}

template<typename ty>
void Graph2D<ty>::drawCurves(QPainter& painter)
{
    QVector<QPoint> polyline;

    painter.setClipRect(contentsRect().adjusted(1, 1, -1, -1));

    for(auto const& curve : _curves)
    {
        if(polyline.size() < curve._points.count())
        {
            polyline.resize(curve._points.count());
        }

        for(auto i = 0; i < curve._points.count(); ++i)
        {
            polyline[i] = toPixel(curve._points[i]);
        }

        painter.setPen(curve._pen);
        painter.drawPolyline(&polyline[0], curve._points.count());
    }
}

template<typename ty>
double Graph2D<ty>::niceNumber(double number, bool round)
{
    // Graphic Gems Vol.1 p.62

    int exp = std::floor(std::log10(number));
    double f = number / std::pow(10.0, exp);
    double nf;

    if(round)
    {
        if(f < 1.5)
            nf = 1.0;
        else
        if(f < 3.0)
            nf = 2.0;
        else
        if(f < 7.0)
            nf = 5.0;
        else
            nf = 10.0;
    }
    else
    {
        if(f <= 1.0)
            nf = 1.0;
        else
        if(f <= 2.0)
            nf = 2.0;
        else
        if(f <= 5.0)
            nf = 5.0;
        else
            nf = 10.0;
    }

    return nf * std::pow(10.0, exp);
}

#endif
