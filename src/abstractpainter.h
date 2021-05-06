#pragma once
#ifndef ABSTRACTPAINTER_H
#define ABSTRACTPAINTER_H

#include <QWidget>
#include <QVector>
#include <QPainter>

// Класс AbstractPainter реализует примитивную очередь отрисовки. Дети этого
// класса не должны переопределять метод paintEvent иначе цикл перерисовки не
// будет запущен. Дети этого класса могут добавить в очередь отрисовки свои
// методы посредством вызова addToRenderQueue. При этом указатель метода
// отрисовки нужно привести к типу PaintFunc.
class AbstractPainter: public QWidget
{
    Q_OBJECT

public:
    void stopRender()
    {
        _renderOut = false;
    }

    void resumeRender()
    {
        _renderOut = true;
        update();
    }

protected:
    typedef void (AbstractPainter::*PaintFunc)(QPainter& painter);

    AbstractPainter(QWidget* parent = nullptr):
        QWidget(parent), _renderOut(true)
    {
        QPalette pal = palette();
        pal.setColor(QPalette::Window, Qt::white);

        setPalette(pal);
    }

    virtual ~AbstractPainter()
    {

    }

    virtual void paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);

        if(_renderOut)
        {
            QPainter painter(this);

            paintBackground(painter);

            if(contentsRect().isValid())
            {
                for(auto paintFunc : _renderQueue)
                {
                    painter.save();
                    (this->*paintFunc)(painter);
                    painter.restore();
                }
            }
        }
    }

    void addToRenderQueue(PaintFunc func)
    {
        _renderQueue.append(func);
    }

    void removeFromRenderQueue(PaintFunc func)
    {
        _renderQueue.removeAll(func);
    }

    void clearRenderQueue()
    {
        _renderQueue.clear();
        //update();
    }

    void paintBackground(QPainter& painter)
    {
        painter.fillRect(0, 0, width(), height(), palette().window());
    }

private:
    QVector<PaintFunc> _renderQueue;
    bool _renderOut;

};

#endif
