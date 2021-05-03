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
    static double niceStep(double min, double max, int numTicks);

    void drawGrid(QPainter& painter);
    void drawGridLabels(QPainter& painter);
    void drawCurves(QPainter& painter);

    static int const _numTicks = 10;

    QMap<int, Curve2D> _curves;

    // Эти поля хранят шаг по осям для экономии аремени.
    double _stepX;
    double _stepY;

};

template<typename ty>
Graph2D<ty>::Graph2D(QWidget* parent):
    Graph2DBase(parent)
{
    _stepX = niceStep(_viewRegion._minX, _viewRegion._maxX, _numTicks);
    _stepY = niceStep(_viewRegion._minY, _viewRegion._maxY, _numTicks);

    QFontMetrics const metrics(font());

    int const contentsLeft = 10 * metrics.averageCharWidth();
    int const contentsTop = 3 * metrics.lineSpacing();
    int const contentsRight = 5 * metrics.averageCharWidth();
    int const contentsBottom = 3 * metrics.lineSpacing();

    setContentsMargins(contentsLeft, contentsTop, contentsRight, contentsBottom);

    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawGrid));
    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawGridLabels));
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

    //painter.setClipRect(contentsRect());

    for(int i = 0; i < _numTicks; ++i)
    {
        double const x = _viewRegion._minX + _stepX * i;
        double const y = 0.0;

        QPoint const pixel = toPixel(QPointF(x, y));

        painter.setPen(lightPen);
        painter.drawLine(pixel.x(), contentsRect().top(), pixel.x(), contentsRect().bottom());
    }

    for(int i = 0; i < _numTicks; ++i)
    {
        double const x = 0.0;
        double const y = _viewRegion._minY + _stepY * i;

        QPoint const pixel = toPixel(QPointF(x, y));

        painter.setPen(lightPen);
        painter.drawLine(contentsRect().left(), pixel.y(), contentsRect().right(), pixel.y());
    }

    painter.setPen(darkPen);
    painter.drawRect(contentsRect().adjusted(0, 0, -1, -1));
}

template<typename ty>
void Graph2D<ty>::drawGridLabels(QPainter& painter)
{
    QFontMetrics const metrics(font());

    for(int i = 0; i < _numTicks; ++i)
    {
        double const x = _viewRegion._minX + _stepX * i;
        double const y = 0.0;

        QPoint const pixel = toPixel(QPointF(x, y));

        QString const number = QString::number(x);
        int const lineWidth = metrics.width(number);

        QRect const rect = {
            pixel.x() - lineWidth / 2,
            contentsRect().bottom() + metrics.xHeight(),
            lineWidth,
            metrics.lineSpacing()
        };

        painter.drawText(rect, Qt::AlignLeft | Qt::AlignTop, number);
    }

    for(int i = 0; i < _numTicks; ++i)
    {
        double const x = 0.0;
        double const y = _viewRegion._minY + _stepY * i;

        QPoint const pixel = toPixel(QPointF(x, y));

        QString const number = QString::number(y);
        int const lineWidth = metrics.width(number);

        QRect const rect {
            contentsRect().left() - lineWidth - metrics.averageCharWidth(),
            pixel.y() - metrics.lineSpacing() / 2,
            lineWidth,
            metrics.lineSpacing()
        };

        painter.drawText(rect, Qt::AlignRight | Qt::AlignTop, number);
    }
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

template<typename ty>
double Graph2D<ty>::niceStep(double min, double max, int numTicks)
{
    return niceNumber(std::abs(max - min) / (numTicks - 1), true);
}

#endif
