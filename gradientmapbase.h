#pragma once
#ifndef GRADIENTMAPBASE_H
#define GRADIENTMAPBASE_H

#include <abstractpainter.h>

// Абстрактный базовый класс для того чтобы его ребёнок мог стать шаблоном. Данный метод имеет большое ограничение -
// он не позволяет шаблонам детей этого класса иметь сигналы и слоты. Однако взамен мы имеем очень мощный инструмент.
class GradientMapBase: public AbstractPainter
{

    Q_OBJECT

signals:

    // Выпадает когда пользователь щёлкает мышью по рабочей области окна. Возвращает индекс элемента карты. Возвращает
    // указатель на элемент данных, который можно привести к исходному типу внутри слота.
    void pointSelected(int ix, int iy/*, void* data*/);

protected:

    GradientMapBase(QWidget* parent = nullptr): AbstractPainter(parent)
    {

    }

    virtual ~GradientMapBase()
    {

    }

    /*void pointSelectedProxy(int ix, int iy)
    {
        emit pointSelected(ix, iy);
    }*/

};

#endif
