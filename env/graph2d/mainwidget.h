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
        auto numPoints = 200;

        QVector<float> yy(numPoints);

        for(auto i = 0; i < numPoints; ++i)
        {
            yy[i] = std::sin((M_PI / numPoints) * i);
        }

        QVector<float> x(numPoints), y(numPoints);

        for(auto i = 0; i < numPoints; ++i)
        {
            x[i] = (M_PI / numPoints) * i;
            y[i] = std::sin(x[i]);
        }

        QVector<QPointF> xy(numPoints);

        for(auto i = 0; i < numPoints; ++i)
        {
            xy[i].setX((M_PI / numPoints) * i);
            xy[i].setY(std::cos(xy[i].x()));
        }

        _graph = new Graph2D<float>(this);
        _graph->addCurve(x, y);
        _graph->addCurve(yy, Qt::red);
        _graph->addCurve(xy, Qt::blue);

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
    Graph2D<float>* _graph;

};

#endif
