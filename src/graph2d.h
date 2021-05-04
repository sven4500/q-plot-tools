#pragma once
#ifndef GRAPH2D_H
#define GRAPH2D_H

#include <QFontMetrics>
#include <QColor>
#include <QMap>
#include <QPoint>
#include <QPointF>
#include <graph2dbase.h>

template<typename ty>
class Graph2D: public Graph2DBase
{
public:
    Graph2D(QWidget* parent = nullptr);
    virtual ~Graph2D();

protected:
    /*!
     * \brief The Curve2D struct
     *
     * Структура кодирует.ровно одну кривую в 2D плоскости. Координаты каждой
     * точки храняться в массиве _memX и _memY.
     */
    struct Curve2D
    {
        Curve2D():
            _memX(nullptr), _memY(nullptr),
            _modifyX(1), _modifyY(1),
            _count(0)
        {
            _color = QColor(255, 127, 39);
        }

        QColor _color;

        // Если один из источников не задан будет сгенерирована
        // последовательность [0..count].
        ty const* _memX;
        ty const* _memY;

        // Расстояние между элементами.
        int _modifyX;
        int _modifyY;

        int _count;

    };

    static double niceNumber(double number, bool round);

    void drawGrid(QPainter& painter);
    void drawCurves(QPainter& painter);

    static int const _numTicks = 10;

    QFontMetrics _metrics;
    QMap<int, Curve2D> _curves;

    double _stepX;
    double _stepY;

};

template<typename ty>
Graph2D<ty>::Graph2D(QWidget* parent):
    Graph2DBase(parent), _metrics(font())
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
    Q_UNUSED(painter);
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
