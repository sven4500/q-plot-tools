#pragma once
#ifndef AXES2D_H
#define AXES2D_H

#include <QPoint>
#include <QPointF>
#include <QMouseEvent>
#include <abstractpainter.h>

class Axes2D: public AbstractPainter
{
    Q_OBJECT

signals:
    void pointSelected(int x, int y);

public:
    Axes2D(QWidget* parent = nullptr):
        AbstractPainter(parent)
    {
        setMouseTracking(true);
    }

    virtual ~Axes2D()
    {

    }

protected:
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

    virtual void mousePressEvent(QMouseEvent* event)
    {
        switch(event->button())
        {
        case Qt::RightButton:
            setCursor(Qt::ClosedHandCursor);
            _dragPoint = toPoint(event->pos());
            break;

        default:
            break;
        }
    }

    virtual void mouseReleaseEvent(QMouseEvent* event)
    {
        switch(event->button())
        {
        case Qt::RightButton:
            unsetCursor();
            break;

        default:
                break;
        }
    }

    virtual void mouseMoveEvent(QMouseEvent* event)
    {
        QPointF point = toPoint(event->pos());

        switch(event->buttons())
        {
        case Qt::RightButton:
            point -= _dragPoint;

            if((event->modifiers() & Qt::ShiftModifier) == 0)
            {
                _viewRegion._minX -= point.x();
                _viewRegion._maxX -= point.x();
            }

            if((event->modifiers() & Qt::ControlModifier) == 0)
            {
                _viewRegion._minY -= point.y();
                _viewRegion._maxY -= point.y();
            }

            update();
            break;

        default:
            break;
        }
    }

    virtual void wheelEvent(QWheelEvent* event)
    {
        if(!event->angleDelta().isNull())
        {
            QPointF const point = toPoint(event->pos());

            // angleDelta возвращает 1/8 долей градуса.
            auto const angle = event->angleDelta().y() / 8;

            double const factor = std::abs(angle > 0.0 ? angle / 30.0 : angle / 10.0);

            if((event->modifiers() & Qt::ShiftModifier) == 0)
            {
                double const deltaSpanX = _viewRegion.spanX() - _viewRegion.spanX() * factor;

                double const x1 = (point.x() - _viewRegion._minX) / _viewRegion.spanX();
                double const x2 = 1.0 - x1;

                _viewRegion._minX += deltaSpanX * x1;
                _viewRegion._maxX -= deltaSpanX * x2;
            }

            if((event->modifiers() & Qt::ControlModifier) == 0)
            {
                double const deltaSpanY = _viewRegion.spanY() - _viewRegion.spanY() * factor;

                double const y1 = (point.y() - _viewRegion._minY) / _viewRegion.spanY();
                double const y2 = 1.0 - y1;

                _viewRegion._minY += deltaSpanY * y1;
                _viewRegion._maxY -= deltaSpanY * y2;
            }

            update();
        }
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

        double const x = (pixel.x() - rect.left()) * kX + _viewRegion._minX;
        double const y = (rect.bottom() - pixel.y()) * kY + _viewRegion._minY;

        return QPointF(x, y);
    }

    ViewRegion2D _viewRegion;
    QPointF _dragPoint;

};

#endif
