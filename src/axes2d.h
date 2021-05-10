#pragma once
#ifndef AXES2D_H
#define AXES2D_H

#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QMouseEvent>
#include <abstractpainter.h>

class Axes2D: public AbstractPainter
{
    Q_OBJECT

signals:
    void pointSelected(int x, int y);

public:
    Axes2D(QWidget* parent = nullptr):
        AbstractPainter(parent), _numTicks(10)
    {
        makeNiceStep();
        setMouseTracking(true);
    }

    virtual ~Axes2D()
    {

    }

    static double niceNumber(double number, bool round)
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
        if(event->button() == Qt::LeftButton)
        {
            setCursor(Qt::CrossCursor);
            _rubberband.setTopLeft(event->pos());
            _rubberband.setBottomRight(event->pos());
        }
        else
        if(event->button() == Qt::RightButton)
        {
            setCursor(Qt::ClosedHandCursor);
            _dragPoint = toPoint(event->pos());
        }
    }

    virtual void mouseReleaseEvent(QMouseEvent* event)
    {
        if(event->button() == Qt::LeftButton)
        {
            unsetCursor();

            QPointF const topLeft = toPoint(_rubberband.topLeft());
            QPointF const bottomRight = toPoint(_rubberband.bottomRight());

            _viewRegion._minX = std::min(topLeft.x(), bottomRight.x());
            _viewRegion._maxX = std::max(topLeft.x(), bottomRight.x());

            _viewRegion._minY = std::min(topLeft.y(), bottomRight.y());
            _viewRegion._maxY = std::max(topLeft.y(), bottomRight.y());

            // Выделяющий прямоугольник больше не будет отрисован.
            _rubberband.setTopLeft(event->pos());
            _rubberband.setBottomRight(event->pos());

            makeNiceStep();
            update();
        }
        else
        if(event->button() == Qt::RightButton)
        {
            unsetCursor();
            update();
        }
    }

    virtual void mouseMoveEvent(QMouseEvent* event)
    {
        if(event->buttons() == Qt::LeftButton)
        {
            if(contentsRect().adjusted(-1, -1, -1, -1).contains(event->pos()))
            {
                _rubberband.setBottomRight(event->pos());
            }

            update();
        }
        else
        if(event->buttons() == Qt::RightButton)
        {
            QPointF const distance = toPoint(event->pos()) - _dragPoint;

            if((event->modifiers() & Qt::ShiftModifier) == 0)
            {
                _viewRegion._minX -= distance.x();
                _viewRegion._maxX -= distance.x();
            }

            if((event->modifiers() & Qt::ControlModifier) == 0)
            {
                _viewRegion._minY -= distance.y();
                _viewRegion._maxY -= distance.y();
            }

            update();
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

            makeNiceStep();
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

    void drawRubberband(QPainter& painter)
    {
        if(std::abs(_rubberband.width()) > 4 && std::abs(_rubberband.height()) > 4)
        {
            painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
            painter.setPen(Qt::white);
            painter.drawRect(_rubberband);
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

    double _stepX;
    double _stepY;

    int _numTicks;

private:
    void makeNiceStep()
    {
        double const rangeX = niceNumber(_viewRegion.spanX(), false);
        double const rangeY = niceNumber(_viewRegion.spanY(), false);

        _stepX = niceNumber(rangeX / (_numTicks - 1), true);
        _stepY = niceNumber(rangeY / (_numTicks - 1), true);
    }

    QRect _rubberband;

    QPointF _dragPoint;

};

#endif
