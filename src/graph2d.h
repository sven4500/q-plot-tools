#pragma once
#ifndef GRAPH2D_H
#define GRAPH2D_H

#include <graph2dbase.h>

template<typename ty>
class Graph2D: public Graph2DBase
{
public:
    Graph2D(QWidget* parent = nullptr);
    virtual ~Graph2D();

protected:
    void drawGrid(QPainter&);
    void drawCurves(QPainter&);

};

template<typename ty>
Graph2D<ty>::Graph2D(QWidget* parent):
    Graph2DBase(parent)
{
    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawGrid));
    addToRenderQueue(reinterpret_cast<PaintFunc>(&drawCurves));

    setMouseTracking(true);
}

template<typename ty>
Graph2D<ty>::~Graph2D()
{

}

template<typename ty>
void Graph2D<ty>::drawGrid(QPainter& painter)
{
    Q_UNUSED(painter);
}

template<typename ty>
void Graph2D<ty>::drawCurves(QPainter& painter)
{
    Q_UNUSED(painter);
}

#endif
