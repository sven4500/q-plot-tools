#pragma once
#ifndef GRAPH2DBASE_H
#define GRAPH2DBASE_H

#include <QPoint>
#include <QPointF>
#include <abstractpainter.h>

class Graph2DBase: public AbstractPainter
{
    Q_OBJECT

signals:
    void pointSelected(int x, int y, void* point);

public:
    Graph2DBase(QWidget* parent = nullptr):
        AbstractPainter(parent)
    {
        setMouseTracking(true);
    }

    virtual ~Graph2DBase()
    {

    }

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

        double spanX()const
        {
            return std::abs(_maxX - _minX);
        }

        double spanY()const
        {
            return std::abs(_maxY - _minY);
        }

        double _minX;
        double _maxX;

        double _minY;
        double _maxY;

    };

    virtual void mouseMoveEvent(QMouseEvent* event)
    {
        Q_UNUSED(event)
    }

    double minX()const
    {
        return _viewRegion._minX;
    }

    double maxX()const
    {
        return _viewRegion._maxX;
    }

    double minY()const
    {
        return _viewRegion._minY;
    }

    double maxY()const
    {
        return _viewRegion._maxY;
    }

    void setLimX(double min, double max)
    {
        if(max > min)
        {
            _viewRegion._minX = min;
            _viewRegion._maxX = max;
        }
    }

    void setLimY(double min, double max)
    {
        if(max > min)
        {
            _viewRegion._minY = min;
            _viewRegion._maxY = max;
        }
    }

    QPoint toPixel(QPointF const& point)const
    {
        QRect const& rect = contentsRect();

        double const kX = _viewRegion.spanX() / rect.width();
        double const kY = _viewRegion.spanY() / rect.height();

        int const x = rect.left() + int((point.x() - _viewRegion._minX) / kX);
        int const y = rect.bottom() - int((point.y() - _viewRegion._minY) / kY);

        return QPoint(x, y);
    }

    QPointF toPoint(QPoint const& pixel)const
    {
        QRect const& rect = contentsRect();

        double const kX = _viewRegion.spanX() / rect.width();
        double const kY = _viewRegion.spanY() / rect.height();

        double const x = double(pixel.x() - rect.x()) * kX + _viewRegion._minX;
        double const y = double(rect.height() - pixel.y()) * kY + _viewRegion._minY;

        return QPointF(x, y);
    }

    ViewRegion2D _viewRegion;

};

#endif
