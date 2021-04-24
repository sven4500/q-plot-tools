#pragma once
#ifndef GRAPH2DBASE_H
#define GRAPH2DBASE_H

#include <abstractpainter.h>

class Graph2DBase: public AbstractPainter
{
    Q_OBJECT

public:
    Graph2DBase(QWidget* parent = nullptr):
        AbstractPainter(parent)
    {

    }

    virtual ~Graph2DBase()
    {

    }

};

#endif
