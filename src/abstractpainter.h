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

protected:
    typedef void (AbstractPainter::*PaintFunc)(QPainter& painter);

    AbstractPainter(QWidget* parent = nullptr):
        QWidget(parent)
    {
        PaintFunc const func = reinterpret_cast<PaintFunc>(&AbstractPainter::paintBackground);
        addToRenderQueue(func);
    }

    virtual ~AbstractPainter()
    {

    }

    virtual void paintEvent(QPaintEvent* event)
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

    void addToRenderQueue(PaintFunc func)
    {
        _renderQueue.append(func);
        //update();
    }

    void clearRenderQueue()
    {
        _renderQueue.clear();
        //update();
    }

    // Метод является базовой реализацией закрашивания фона в белый цвет.
    void paintBackground(QPainter& painter)
    {
        painter.fillRect(0, 0, width(), height(), Qt::white);
    }

private:

    QVector<PaintFunc> _renderQueue;

};

#endif
