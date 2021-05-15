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
    GraphPolar<float>* _graph;

};

#endif
