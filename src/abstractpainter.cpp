#include "abstractpainter.h"

AbstractPainter::AbstractPainter(QWidget* parent): QWidget(parent)
{
    PaintFunc const func = reinterpret_cast<PaintFunc>(&AbstractPainter::paintBackground);
    addToRenderQueue(func);
}

AbstractPainter::~AbstractPainter()
{

}

void AbstractPainter::addToRenderQueue(PaintFunc const func)
{
    _renderQueue.append(func);
    //update();
}

void AbstractPainter::clearRenderQueue()
{
    _renderQueue.clear();
    //update();
}

void AbstractPainter::paintBackground(QPainter& painter)
{
    painter.fillRect(0, 0, width(), height(), Qt::white);
}

void AbstractPainter::paintEvent(QPaintEvent* const event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    for(int i = 0; i < _renderQueue.size(); ++i)
    {
        painter.save();
        (this->*_renderQueue[i])(painter);
        painter.restore();
    }
}
