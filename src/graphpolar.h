#ifndef GRAPHPOLAR_H_
#define GRAPHPOLAR_H_

#include <QMap>
#include <QRegion>
#include <axespolar.h>

template<typename ty>
class GraphPolar: public AxesPolar
{
public:
    GraphPolar(QWidget* parent = nullptr);
    virtual ~GraphPolar();

    int addCurve(QVector<ty> const& rho, QVector<ty> const& phi);

    void setColor(int id, QColor const& color);
    void setStyle(int id, Qt::PenStyle style);
    void setWidth(int id, int width);

protected:
    struct CurvePolar
    {
        CurvePolar():
            _pen(Qt::red, 1, Qt::SolidLine)
        {

        }

        QVector<PolarPoint> _points;
        QPen _pen;

    };

    int uniqueId()const;

    void drawCurves(QPainter& painter);

private:
    QMap<int, CurvePolar> _curves;

};

template<typename ty>
GraphPolar<ty>::GraphPolar(QWidget* parent):
    AxesPolar(parent)
{
    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawCurves));
}

template<typename ty>
GraphPolar<ty>::~GraphPolar()
{

}

template<typename ty>
int GraphPolar<ty>::uniqueId()const
{
    int id = 0;

    // Key values are sorted ascending in ordered map.

    for(auto iter = _curves.cbegin(); iter != _curves.cend(); ++iter)
    {
        if(iter.key() != id)
        {
            return id;
        }

        ++id;
    }

    return id;
}

template<typename ty>
int GraphPolar<ty>::addCurve(QVector<ty> const& rho, QVector<ty> const& phi)
{
    auto const id = uniqueId();

    if(id >= 0)
    {
        auto const size = std::min(rho.size(), phi.size());

        CurvePolar& curve = _curves[id];
        curve._points.resize(size);

        for(auto i = 0; i < size; ++i)
        {
            curve._points[i]._rho = rho[i];
            curve._points[i]._phi = phi[i];
        }
    }

    return id;
}

template<typename ty>
void GraphPolar<ty>::setColor(int id, QColor const& color)
{
    if(_curves.contains(id))
    {
        _curves[id]._pen.setColor(color);
        update();
    }
}

template<typename ty>
void GraphPolar<ty>::drawCurves(QPainter& painter)
{
    auto const rad = std::min(contentsRect().width(), contentsRect().height()) / 2;

    QRect const rect(contentsRect().center() - QPoint(rad, rad), contentsRect().center() + QPoint(rad - 1, rad - 1));
    //painter.drawRect(rect);

    QRegion const region(rect, QRegion::Ellipse);
    painter.setClipRegion(region);

    // Leave polyline outside loop to avoid unnecessary memory reallocations.
    QVector<QPoint> polyline;

    for(auto const& curve : _curves)
    {
        // Make sure polyline has enough points to store entire curve.
        if(polyline.size() < curve._points.count())
        {
            polyline.resize(curve._points.count());
        }

        // Map each point to widget screen space.
        for(auto i = 0; i < curve._points.count(); ++i)
        {
            polyline[i] = toPixel(curve._points[i]);
        }

        // Draw polyline.
        painter.setPen(curve._pen);
        painter.drawPolyline(&polyline[0], curve._points.count());
    }
}

#endif
