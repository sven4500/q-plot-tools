#ifndef AXESPOLAR_H_
#define AXESPOLAR_H_

#include <QLine>
#include <abstractpainter.h>

class AxesPolar: public AbstractPainter
{
    Q_OBJECT

signals:
    pointSelected(int x, int y);

protected:
    struct ViewRegionPolar
    {
        ViewRegionPolar():
            _minRho(0.0), _maxRho(1.0)
        {

        }

        double spanRho()const
        {
            return std::abs(_maxRho - _minRho);
        }

        double _minRho;
        double _maxRho;

    };

    struct PolarPoint
    {
        PolarPoint():
            _rho(0.0), _phi(0.0)
        {

        }

        PolarPoint(double rho, double phi):
            _rho(rho), _phi(phi)
        {

        }

        double _rho;
        double _phi;

    };

    AxesPolar(QWidget* parent = nullptr):
        AbstractPainter(parent), _numStepsPhi(24)
    {
        // todo: add some space for tick labels (use QFontMetrics)
        setContentsMargins(10, 10, 10, 10);
        addToRenderQueue(reinterpret_cast<PaintFunc>(&drawGrid));
    }

    virtual ~AxesPolar()
    {

    }

    void drawGrid(QPainter& painter)
    {
        QRect const& rect = contentsRect();

        int const rad = std::min(rect.width(), rect.height()) / 2;
        double const stepPhi = (2 * M_PI) / _numStepsPhi;

        QVector<QLine> const majorLines{
            QLine(QPoint(rect.center()), toPixel(PolarPoint(_viewRegion._maxRho, M_PI * 0.0))),
            QLine(QPoint(rect.center()), toPixel(PolarPoint(_viewRegion._maxRho, M_PI * 0.5))),
            QLine(QPoint(rect.center()), toPixel(PolarPoint(_viewRegion._maxRho, M_PI * 1.0))),
            QLine(QPoint(rect.center()), toPixel(PolarPoint(_viewRegion._maxRho, M_PI * 1.5)))
        };

        QVector<QLine> minorLines(_numStepsPhi);

        for(int i = 0; i < _numStepsPhi; ++i)
        {
            PolarPoint const p(_viewRegion._maxRho, stepPhi * i);

            QPoint const p1 = rect.center();
            QPoint const p2 = toPixel(p);

            minorLines[i].setP1(p1);
            minorLines[i].setP2(p2);
        }

        // todo: draw concentric circles

        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter.drawLines(minorLines);

        painter.setPen(QPen(Qt::darkGray, 1, Qt::SolidLine));
        painter.drawLines(majorLines);
        painter.drawEllipse(rect.center(), rad, rad);
    }

    QPoint toPixel(PolarPoint const& point)const
    {
        QRect const& rect = contentsRect();
        int const rad = std::min(rect.width(), rect.height()) / 2;
        double const normRho = (point._rho - _viewRegion._minRho) / _viewRegion.spanRho();
        QPoint const pixel(rad * normRho * std::cos(point._phi), rad * normRho * std::sin(point._phi));
        return rect.center() + pixel;
    }

    PolarPoint toPoint(QPoint const& pixel)const
    {
        Q_UNUSED(pixel);
        return PolarPoint(0.0, 0.0);
    }

    ViewRegionPolar _viewRegion;

private:
    double _stepRho;
    int _numStepsPhi;

};

#endif
