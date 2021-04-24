#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <graph2d.h>

class MainWidget: public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget* parent = nullptr):
        QWidget(parent)
    {
        _graph = new Graph2D<int>(this);

        QGridLayout* const layout = new QGridLayout(this);
        //layout->setMargin(0);
        //layout->setSpacing(0);

        layout->addWidget(_graph, 0, 0);

        setLayout(layout);
    }

    virtual ~MainWidget()
    {

    }

private:
    Graph2D<int>* _graph;

};

#endif
