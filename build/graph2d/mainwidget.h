#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#define _USE_MATH_DEFINES

#include <QWidget>
#include <QGridLayout>
#include <QPointF>
#include <cmath>
#include <graph2d.h>

class MainWidget: public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget* parent = nullptr):
        QWidget(parent)
    {
        _graph = new Graph2D<float>(this);

        addCurve1(_graph);
        addCurve2(_graph);
        addCurve3(_graph);

        QGridLayout* const layout = new QGridLayout(this);
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(_graph, 0, 0);

        setLayout(layout);

        resize(640, 480);
    }

    virtual ~MainWidget()
    {

    }

private:
    static void addCurve1(Graph2D<float>* graph)
    {
        auto const numPoints = 200;
        QVector<float> y(numPoints);

        for(auto i = 0; i < numPoints; ++i)
        {
            y[i] = std::sin((M_PI / numPoints) * i);
        }

        auto const id = graph->addCurve(y, Qt::red);
        graph->setStyle(id, Qt::DashLine);
        graph->setWidth(id, 2);
    }

    static void addCurve2(Graph2D<float>* graph)
    {
        auto const numPoints = 200;
        QVector<float> x(numPoints), y(numPoints);

        for(auto i = 0; i < numPoints; ++i)
        {
            x[i] = (M_PI / numPoints) * i;
            y[i] = std::sin(x[i]);
        }

        auto const id = graph->addCurve(x, y);
        graph->setWidth(id, 3);
    }

    static void addCurve3(Graph2D<float>* graph)
    {
        auto const numPoints = 200;
        QVector<QPointF> xy(numPoints);

        for(auto i = 0; i < numPoints; ++i)
        {
            xy[i].setX((M_PI / numPoints) * i);
            xy[i].setY(std::cos(xy[i].x()));
        }

        auto const id = graph->addCurve(xy, Qt::blue);
        graph->setColor(id, Qt::green);
    }

    Graph2D<float>* _graph;

};

#endif
