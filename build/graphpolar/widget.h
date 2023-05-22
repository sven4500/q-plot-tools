#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <graphpolar.h>

class Widget: public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr):
        QWidget(parent)
    {
        _graph = new GraphPolar<float>(this);

        QPalette pal = _graph->palette();
        pal.setColor(QPalette::Window, Qt::lightGray);

        // You can make a darker background to get more contrast.
        //_graph->setPalette(pal);

        // Let us see the difference between curves with different
        // discretization levels.
        addCurve(_graph, 10, Qt::darkMagenta);
        addCurve(_graph, 20, Qt::darkGreen);
        addCurve(_graph, 100, Qt::blue);
        addCurve(_graph, 2000, Qt::red);

        QGridLayout* const layout = new QGridLayout(this);
        //layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(_graph);

        setLayout(layout);

        resize(640, 480);
    }

    virtual ~Widget()
    {

    }

private:
    static void addCurve(GraphPolar<float>* graph, int size, QColor const& color)
    {
        QVector<float> rho(size);
        QVector<float> phi(size);

        float const stepRho = 1.0 / size;
        float const stepPhi = (4.0 * M_PI) / size;

        for(auto i = 0; i < size; ++i)
        {
            rho[i] = stepRho * i;
            phi[i] = stepPhi * i;
        }

        auto const id = graph->addCurve(rho, phi);
        graph->setColor(id, color);
    }

    GraphPolar<float>* _graph;

};

#endif
