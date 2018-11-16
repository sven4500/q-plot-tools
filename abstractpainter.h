#pragma once
#ifndef ABSTRACTPAINTER_H
#define ABSTRACTPAINTER_H

#include <QWidget>
#include <QVector>
#include <QPainter>

// Класс AbstractPainter реализует примитивную очередь отрисовки. Дети этого класса не должны переопределять метод
// paintEvent иначе цикл перерисовки не будет запущен. Дети этого класса могут добавить в очередь отрисовки свои методы
// посредством вызова addToRenderQueue. При этом указатель метода отрисовки нужно привести к типу PaintFunc.
class AbstractPainter: public QWidget
{

    Q_OBJECT

protected:

    AbstractPainter(QWidget* parent = nullptr);
    virtual ~AbstractPainter()=0;

    typedef void (AbstractPainter::*PaintFunc)(QPainter& painter);

    virtual void paintEvent(QPaintEvent* event);

    void addToRenderQueue(PaintFunc func);
    void clearRenderQueue();

    // Метод является базовой реализацией закрашивания фона в белый цвет.
    void paintBackground(QPainter& painter);

private:

    QVector<PaintFunc> _renderQueue;

};

#endif
