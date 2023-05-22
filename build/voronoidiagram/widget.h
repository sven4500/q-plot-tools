#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <voronoidiagram.h>

class Widget: public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr):
        QWidget(parent)
    {
        // How many points will be generated.
        auto const numPoints = 10;

        // Let us generate some random points.
        QVector<float> x = generatePoints(numPoints);
        QVector<float> y = generatePoints(numPoints);

        _voronoi = new VoronoiDiagram<float>(this);
        _voronoi->addPoints(x, y);

        QGridLayout* const layout = new QGridLayout(this);
        //layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(_voronoi, 0, 0, 1, 1);

        setLayout(layout);

        resize(640, 480);
    }

    virtual ~Widget()
    {

    }

    static QVector<float> generatePoints(int numPoints)
    {
        QVector<float> vect(numPoints);

        for(auto i = 0; i < numPoints; ++i)
        {
            vect[i] = (float)rand() / RAND_MAX;
        }

        return vect;
    }

protected:
    VoronoiDiagram<float>* _voronoi;

};

#endif
