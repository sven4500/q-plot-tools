#pragma once
#ifndef GRAPH2D_H
#define GRAPH2D_H

#include <QFontMetrics>
#include <QtMath> // qFabs, qPow
#include <QColor>
#include <QMap>
#include <graph2dbase.h>

template<typename ty>
class Graph2D: public Graph2DBase
{
public:
    Graph2D(QWidget* parent = nullptr);
    virtual ~Graph2D();

    double minX()const;
    double maxX()const;

    double minY()const;
    double maxY()const;

    void setLimX(double min, double max);
    void setLimY(double min, double max);

protected:
    /*!
     * \brief The ViewRegion2D struct
     *
     * Структура описывает область видимости 2D пространства. Задаёт видимый
     * отрезок по осям X и Y.
     */
    struct ViewRegion2D
    {
        ViewRegion2D():
            _minX(0.0), _maxX(1.0), _minY(0.0), _maxY(1.0)
        {

        }

        double _minX;
        double _maxX;

        double _minY;
        double _maxY;

    };

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
    static double preferredStep(double min, double max);

    void drawGrid(QPainter& painter);
    void drawCurves(QPainter& painter);

    QMap<int, Curve2D> _curves;

    ViewRegion2D _viewRegion;

    // Для повышения быстродействия храним шаг здесь.
    double _stepX;
    double _stepY;

};

template<typename ty>
Graph2D<ty>::Graph2D(QWidget* parent):
    Graph2DBase(parent)
{
    _stepX = preferredStep(_viewRegion._minX, _viewRegion._maxX);
    _stepY = preferredStep(_viewRegion._minY, _viewRegion._maxY);

    QFontMetrics const metrics(font());

    setContentsMargins(10 * metrics.averageCharWidth(), 3 * metrics.lineSpacing(), 5 * metrics.averageCharWidth(), 3 * metrics.lineSpacing());

    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawGrid));
    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawCurves));

    setMouseTracking(true);
}

template<typename ty>
Graph2D<ty>::~Graph2D()
{

}

template<typename ty>
double Graph2D<ty>::niceNumber(double number, bool round)
{
    // Graphic Gems Vol.1 p.62

    int exp;
    double f;
    double nf;

    exp = std::floor(std::log10(number));
    f = number / std::pow(10.0, exp);

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
double Graph2D<ty>::preferredStep(double min, double max)
{
    int const numTicks = 10;
    return niceNumber(std::abs(max - min) / (numTicks - 1), true);
}

template<typename ty>
void Graph2D<ty>::drawGrid(QPainter& painter)
{
    QPen const solidPen(Qt::black, 1, Qt::SolidLine);
    QPen const dashedPen(Qt::black, 1, Qt::DashLine);

    painter.setPen(solidPen);
    painter.drawRect(contentsRect());
}

template<typename ty>
void Graph2D<ty>::drawCurves(QPainter& painter)
{
    Q_UNUSED(painter);
}

#endif
