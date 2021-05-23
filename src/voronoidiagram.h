#ifndef VORONOIDIAGRAM_H_
#define VORONOIDIAGRAM_H_

#include <QVector>
#include <QPointF>
#include <axes2d.h>

template<typename ty>
class VoronoiDiagram: public Axes2D
{
public:
    VoronoiDiagram(QWidget* parent = nullptr);
    virtual ~VoronoiDiagram();

    void clear();

    void addPoint(ty const& x, ty const& y);
    void addPoints(QVector<ty> const& x, QVector<ty> const& y);
    void addPoints(ty const* x, ty const* y, int count);

    void removePoint(ty const& x, ty const& y);
    void removePoints(QVector<ty> const& x, QVector<ty> const& y);
    void removePoints(ty const* x, ty const* y, int count);

protected:
    void drawPoints(QPainter& painter);

    QVector<QPointF> _points;

};

template<typename ty>
VoronoiDiagram<ty>::VoronoiDiagram(QWidget* parent):
    Axes2D(parent)
{
    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawPoints));
}

template<typename ty>
VoronoiDiagram<ty>::~VoronoiDiagram()
{

}

template<typename ty>
void VoronoiDiagram<ty>::clear()
{
    _points.clear();
    update();
}

template<typename ty>
void VoronoiDiagram<ty>::addPoints(QVector<ty> const& x, QVector<ty> const& y)
{
    auto const count = std::min(x.count(), y.count());

    if(count > 0)
    {
        addPoints(&x[0], &y[0], count);
    }
}

template<typename ty>
void VoronoiDiagram<ty>::addPoints(ty const* x, ty const* y, int count)
{
    QVector<QPointF> vect(count);

    for(auto i = 0; i < count; ++i)
    {
        vect[i] = QPointF(x[i], y[i]);
    }

    _points.append(vect);
    update();
}

template<typename ty>
void VoronoiDiagram<ty>::drawPoints(QPainter& painter)
{
    QVector<QPointF> points(_points.size());

    for(auto i = 0; i < _points.size(); ++i)
    {
        points[i] = toPixel(_points[i]);
    }

    QPen const pen(Qt::black, 4, Qt::SolidLine/*, Qt::RoundCap, Qt::RoundJoin*/);

    painter.setPen(pen);
    painter.drawPoints(&points[0], points.size());
}

#endif
