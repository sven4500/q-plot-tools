#ifndef GRAPHPOLAR_H_
#define GRAPHPOLAR_H_

#include <axespolar.h>

template<typename ty>
class GraphPolar: public AxesPolar
{
public:
    GraphPolar(QWidget* parent = nullptr):
        AxesPolar(parent)
    {

    }

    virtual ~GraphPolar()
    {

    }

};

#endif
