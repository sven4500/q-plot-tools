#ifndef AXESPOLAR_H_
#define AXESPOLAR_H_

#include <abstractpainter.h>

class AxesPolar: public AbstractPainter
{
    Q_OBJECT

signals:
    pointSelected(int x, int y);

protected:
    struct ViewRegionPolar
    {
        double _minR;
        double _maxR;

    };

    AxesPolar(QWidget* parent = nullptr):
        AbstractPainter(parent)
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
        painter.drawEllipse(rect.center(), rad, rad);
    }

private:
    double _step;

};

#endif
