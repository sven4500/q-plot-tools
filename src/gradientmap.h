#pragma once
#ifndef GRADIENTMAP_H
#define GRADIENTMAP_H

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QImage>
#include <QSize>
#include <QMap>
#include <axes2d.h>

// GradientMap должен быть шаблоном, однако Qt не поддерживает шаблонные
// классы, неследованные напрямую от QWidget. Это связано с определёнными
// ограничениями репроцессора moc.
template<typename ty>
class GradientMap: public Axes2D
{
public:
    GradientMap(QWidget* parent = nullptr);
    virtual ~GradientMap();

    void clear();

//    void hasDynamicRange(bool m_hasDynamicRange);

    // Если true, то при передвижении курсора, виджет показывает небольшое сообщение
    // со значением карты в данной точке. setMouseTracking также должен быть включен.
//    void showHint(bool isHintShown);

    // fitContent(false) вписывает только общую часть всего рисунка. Может привести к
    // маскированию хвостов некоторых скан-линий. fitContent(true) вписывает рисунок
    // целиком. Всегда видны скан-линии целиком.
    void fit(bool const bEntireFit = true);

    // Добавляет в карту очередной вектор данных, который характеризуется своим
    // дескриптором id. Замещает существующий дескриптор.
    void add(int id, ty const* addr, int size);
    void remove(int id);

    void swap(int id1, int id2);

    void setXPosition(int x);
    void setYPosition(int y);

    void setPosition(int x, int y);

    // Задаёт количество узлов карты помещающихся в рабочую область окна.
    void setHorizontalResolution(float x);
    void setVerticalResolution(float y);

    void setResolution(float x, float y);

    void setHorizontalRange(float min, float max);
    void setVerticalRange(float min, float max);

    void setCoolColor(int red, int green ,int blue);
    void setCoolColor(QColor const& color);

    void setWarmColor(int red, int green ,int blue);
    void setWarmColor(QColor const& color);

    void setMin(ty const& m_min);
    void setMax(ty const& m_max);

//    double localMin()const;
//    double localMax()const;

    ty globalMin()const;
    ty globalMax()const;

    // Данные добавляются в карту по-строчно. Чтобы избежать ненужной перерисовки
    // каждый раз когда добваляется очередной вектор
    /*virtual*/ void updateGradientImage();

    // Возвращает значение для заданного пикселя карты [x,y]. Переменная hasValue возвращает
    // false если в данной точке нет данных. Здесь наверное лучше возвращать bool вместо значения
    // NaN для отсутствующих величин. Данная функция оказывется также полезной когда нужно.
//    inline bool value(int x, int y, double* value)const;

private:
    // Описывает буфер строки: адрес буфера и его размер. Смещение
    // характеризует исключительно способ отрисовки строки.
    struct MemDesc
    {
        ty const* addr;
        int size;
        int bias;
//        int id;
    };

    // Примитивная структура для описания цвета. Использовать QColor не самое лучшее решение
    // из-за инкапсуляции его данных.
    struct Color
    {
        int red;
        int green;
        int blue;
    };

    // Метод для обновления разности холодного и тёплого цветов.
    void updateColorDifference();

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

    void drawAll(QPainter& painter);

    // Думаю здесь использовать QMap более уместно вместо QVector. QVector приводит к фрагментации
    // памяти при каждом добавлении очередного вектора данных. QMap в свою очередь хранит данные
    // в разных областях памяти. Добавление и удаление здесь быстрее.
    QMap<int, MemDesc> m_map;
    QImage m_image;

    QRect m_rubberband;
//    QString hint;

    Color m_coolColor;
    Color m_warmColor;

    Color m_colorDifference;

    float m_min;
    float m_max;
    float m_peakToPeak;

    // x и y обозначают пиксель верхнего левого угла картинки. Возникает ошибка округления
    // при изменении размера рабочей области окна, когда x и y имеют тип int?
    float m_x;
    float m_y;

    // Точка на которую указывал указатель мыши перед нажатием кнопки мыши.
    // Используются только методами обработки событий мыши.
//    float m_xLast;
//    float m_yLast;

    // Размер чередующихся чёрно-белых квадратов фонового рисунка.
    int m_tileSize;

    QPoint m_lastPos;

    // Задаёт количество узлов приходящихся на полную рабочую область окна.
    float m_horizontalResolution;
    float m_verticalResolution;

    bool m_bEntireFit;

    bool m_showRubberband;
    bool m_showHint;
    bool m_showDynamicRange;

};

template<typename ty>
GradientMap<ty>::GradientMap(QWidget* parent):
    Axes2D(parent)
{
    {
        PaintFunc const func = reinterpret_cast<PaintFunc>(&GradientMap::drawAll);
        addToRenderQueue(func);
    }

    setContentsMargins(10, 10, 10, 10);

    m_x = 0.0;
    m_y = 0.0;

    m_horizontalResolution = 1.0;
    m_verticalResolution = 1.0;

//    min = 0.0;
//    max = 1.0;
//    peakToPeak = 1.0;

    m_tileSize = 10;

    setCoolColor(0, 0, 0);
    setWarmColor(255, 255, 255);

    m_bEntireFit = true;

    m_showRubberband = false;
//    isHintShown = false;
}

template<typename ty>
GradientMap<ty>::~GradientMap()
{

}

template<typename ty>
void GradientMap<ty>::clear()
{
    m_map.clear();
    updateGradientImage();
}

template<typename ty>
void GradientMap<ty>::add(int id, ty const* addr, int size)
{
    if(addr != nullptr && size > 0)
    {
        // QMap тихо вставляет элемент при обращении через оператор квадратных скобок.
        // Поэтому здесь сразу получаем ссылку на только что вставленный элемент.
        MemDesc& memDesc = m_map[id];
        memDesc.addr = addr;
        memDesc.size = size;
    }
}

template<typename ty>
void GradientMap<ty>::remove(int id)
{
    m_map.remove(id);
}

template<typename ty>
void GradientMap<ty>::swap(int id1, int id2)
{
    if(m_map.contains(id1) && m_map.contains(id2))
        std::swap(m_map[id1], m_map[id2]);
}

template<typename ty>
void GradientMap<ty>::setXPosition(int x)
{
    m_x = x;
}

template<typename ty>
void GradientMap<ty>::setYPosition(int y)
{
    m_y = y;
}

template<typename ty>
void GradientMap<ty>::setPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

template<typename ty>
void GradientMap<ty>::setHorizontalResolution(float resolutionX)
{
    m_horizontalResolution = resolutionX;
}

template<typename ty>
void GradientMap<ty>::setVerticalResolution(float resolutionY)
{
    m_verticalResolution = resolutionY;
}

template<typename ty>
void GradientMap<ty>::setResolution(float horizontalResolution, float verticalResolution)
{
    m_horizontalResolution = horizontalResolution;
    m_verticalResolution = verticalResolution;
}

template<typename ty>
void GradientMap<ty>::setMin(ty const& min)
{
    GradientMap::m_min = static_cast<float>(min);
    GradientMap::m_peakToPeak = m_max - GradientMap::m_min;
}

template<typename ty>
void GradientMap<ty>::setMax(ty const& max)
{
    GradientMap::m_max = static_cast<float>(max);
    GradientMap::m_peakToPeak = GradientMap::m_max - m_min;
}

// Вписывает карту целиком в рабочую область окна.
template<typename ty>
void GradientMap<ty>::fit(bool const bEntireFit)
{
    if(m_map.empty())
        return;

    // Для чего здесь нужен typename?
    typename QMap<int, MemDesc>::const_iterator const end = m_map.cend();
    typename QMap<int, MemDesc>::const_iterator iter = m_map.begin();

    int maxSize = 0;

    if(bEntireFit /*== true*/)
    {
        maxSize = 0;
        while(iter != end)
        {
            maxSize = std::max(iter->size, maxSize);
            ++iter;
        }
    }
    else
    {
        maxSize = std::numeric_limits<int>::max();
        while(iter != end)
        {
            maxSize = std::min(iter->size, maxSize);
            ++iter;
        }
    }

    m_horizontalResolution = maxSize;
    m_verticalResolution = m_map.size();

    m_x = 0.0/*static_cast<float>(m_image.width()) * static_cast<float>(minX) / static_cast<float>(maxX - minX)*/;
    m_y = 0.0;

    updateGradientImage();
//    update();
}

template<typename ty>
void GradientMap<ty>::setCoolColor(int const red, int const green, int const blue)
{
    m_coolColor.red = red;
    m_coolColor.green = green;
    m_coolColor.blue = blue;
    updateColorDifference();
}

template<typename ty>
void GradientMap<ty>::setCoolColor(QColor const& color)
{
    m_coolColor.red = color.red();
    m_coolColor.green = color.green();
    m_coolColor.blue = color.blue();
    updateColorDifference();
}

template<typename ty>
void GradientMap<ty>::setWarmColor(int const red, int const green, int const blue)
{
    m_warmColor.red = red;
    m_warmColor.green = green;
    m_warmColor.blue = blue;
    updateColorDifference();
}

template<typename ty>
void GradientMap<ty>::setWarmColor(QColor const& color)
{
    m_warmColor.red = color.red();
    m_warmColor.green = color.green();
    m_warmColor.blue = color.blue();
    updateColorDifference();
}

template<typename ty>
void GradientMap<ty>::updateColorDifference()
{
    m_colorDifference.red = m_warmColor.red - m_coolColor.red;
    m_colorDifference.green = m_warmColor.green - m_coolColor.green;
    m_colorDifference.blue = m_warmColor.blue - m_coolColor.blue;
}

template<typename ty>
ty GradientMap<ty>::globalMin()const
{
    ty min;

    if(m_map.empty())
        return min;

    typename QMap<int, MemDesc>::const_iterator const end = m_map.cend();
    typename QMap<int, MemDesc>::const_iterator iter = m_map.begin();

    // В карте нет пустых векторов, поэтому addr[0] возможно всегда.
    min = iter->addr[0];

    while(iter != end)
    {
        MemDesc const& memDesc = *iter;
        for(int i = 0; i < memDesc.size; ++i)
            if(memDesc.addr[i] < min)
                min = memDesc.addr[i];
        ++iter;
    }

    return min;
}

template<typename ty>
ty GradientMap<ty>::globalMax()const
{
    ty max;

    if(m_map.empty())
        return max;

    typename QMap<int, MemDesc>::const_iterator const end = m_map.cend();
    typename QMap<int, MemDesc>::const_iterator iter = m_map.begin();

    max = iter->addr[0];

    while(iter != end)
    {
        MemDesc const& memDesc = *iter;
        for(int i = 0; i < memDesc.size; ++i)
            if(memDesc.addr[i] > max)
                max = memDesc.addr[i];
        ++iter;
    }

    return max;
}

// Заполняет каждый пиксель изображения с учётом интерполяции по ближайшему соседу.
// Считает только фрагмент изображения если включён режим увеличения.
template<typename ty>
void GradientMap<ty>::updateGradientImage()
{
    // Самый большой метод данного класса. Мог бы быть меньше, если убрать ветвление насышения.
    // Однако такое ветвление помогает несколько повысить производительность.
    // tileSize размер чередующихся чёрно-белых квадратов фонового рисунка.
    int const width = m_image.width(),
            height = m_image.height();

    // Фактор задаёт количество пикселей на один узел (плотность точек).
    float const factorX = width / m_horizontalResolution,
            factorY = height / m_verticalResolution;

    typename QMap<int, MemDesc>::const_iterator const begin = m_map.cbegin();

    for(int j = 0; j < height; ++j)
    {
        float const iy = (j - m_y) / factorY;
        MemDesc const* const memDesc = (iy >= 0.0 && iy < m_map.size()) ? &*(begin + iy) : nullptr;

        // Здесь есть интересный момент. Сообщество Qt рекомендует использовать структуру QRgb
        // вместо прямого обращения к отдельным каналам, т.к. на разных платформах может быть
        // разный порядок байт. Однако вызывать методы слишком накладно поэтому здесь мы
        // обращаемся к каналам напрямую через указатель на unsigned char. Формат изображения
        // всегда фиксированный поэтому худшее что может случиться это искажения цветов.
        // Можно пробовать совсем избавиться от вызова scanLine. Меньше вызовов - быстрее отрисовка.
        unsigned char* pixel = m_image.scanLine(j);

        for(int i = 0; i < width; ++i)
        {
            // Здесь индекс обязан быть плавающим иначе приближаясь к границе с отрицательной стороны
            // целый индекс станет 0 и появится дополнительный узел там где его быть не должно.
            float const ix = (i - m_x) / factorX;

            // Если указатель на MemDesc пустой значит нет строки.
            if(memDesc != nullptr && ix >= 0 && ix < memDesc->size)
            {
                float const value = static_cast<float>(memDesc->addr[static_cast<int>(ix)]);

                // Войти в режим назыщения если текущее значение лежит вне диапазона.
                if(value < m_min)
                {
                    *pixel++ = m_coolColor.blue;
                    *pixel++ = m_coolColor.green;
                    *pixel++ = m_coolColor.red;
                }
                else if(value > m_max)
                {
                    *pixel++ = m_warmColor.blue;
                    *pixel++ = m_warmColor.green;
                    *pixel++ = m_warmColor.red;
                }
                else
                {
                    float const factor = (value - m_min) / m_peakToPeak;
                    *pixel++ = m_coolColor.blue + m_colorDifference.blue * factor;
                    *pixel++ = m_coolColor.green + m_colorDifference.green * factor;
                    *pixel++ = m_coolColor.red + m_colorDifference.red * factor;
                }
            }
            else
            {
                int const greyShade = ((j / m_tileSize + (i / m_tileSize) % 2) % 2 == 0) ? 192 : 255;
                *pixel++ = greyShade;
                *pixel++ = greyShade;
                *pixel++ = greyShade;
            }

            ++pixel;
        }
    }

    update();
}

template<typename ty>
void GradientMap<ty>::mousePressEvent(QMouseEvent* event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
        // Попытаемся отправить сигнал через посредника.
//        pointSelectedProxy(event->pos().x(), event->pos().y());

        m_rubberband.setTopLeft(event->pos());
        m_rubberband.setBottomRight(event->pos());
        m_showRubberband = true;
        setCursor(Qt::CrossCursor);
        emit pointSelected(event->pos().x(), event->pos().y());
        break;

    case Qt::RightButton:
//        m_xLast = event->pos().x();
//        m_yLast = event->pos().y();
        m_lastPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        break;

    default:
        break;
    }
}

template<typename ty>
void GradientMap<ty>::mouseMoveEvent(QMouseEvent* event)
{
    switch(event->buttons())
    {
    case Qt::LeftButton:
        // Показывать rubberband только когда его размер больше 4x4
        m_rubberband.setBottomRight(event->pos());
        update();

        break;

    case Qt::RightButton:
        if((event->modifiers() & Qt::ShiftModifier) == 0)
            m_x += event->pos().x() - m_lastPos.x();

        if((event->modifiers() & Qt::ControlModifier) == 0)
            m_y += event->pos().y() - m_lastPos.y();

//        m_xLast = event->pos().x();
//        m_yLast = event->pos().y();

        m_lastPos = event->pos();

        updateGradientImage();
        update();

        break;

    default:
//        m_xLast = event->pos().x();
//        m_yLast = event->pos().y();
        m_lastPos = event->pos();
        // Здесь код для получения значения ткущего пикселя...
        // по пустой строке paintEvent может понять седует ли перерисовывать подсказку
        update();

        break;
    }
}

template<typename ty>
void GradientMap<ty>::mouseReleaseEvent(QMouseEvent* event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
        m_rubberband = m_rubberband.normalized();

        if(m_rubberband.width() > 4 && m_rubberband.height() > 4)
        {
            float const xFactor = float(m_rubberband.width()) / float(m_image.width()),
                    yFactor = float(m_rubberband.height()) / float(m_image.height());

            m_horizontalResolution *= xFactor;
            m_verticalResolution *= yFactor;

            m_x = (m_x - m_rubberband.x()) / xFactor;
            m_y = (m_y - m_rubberband.y()) / yFactor;
        }
        else
        {
            // Сигнализировать выбор узла...
            // emit nodeSelected();
        }

        setCursor(Qt::ArrowCursor);

        m_showRubberband = false;

        updateGradientImage();
        update();
        break;

    case Qt::RightButton:
        setCursor(Qt::ArrowCursor);
        break;

    case Qt::MidButton:
        fit(m_bEntireFit);
        m_bEntireFit = !m_bEntireFit;
        break;

    default:
//        setCursor(Qt::ArrowCursor);
        break;
    }
}

template<typename ty>
void GradientMap<ty>::wheelEvent(QWheelEvent* event)
{
    // Сообщество Qt настоятельно рекомендует принять событие если оно было обработано,
    // или отклонить его если оно не было обработано.
    if(event->angleDelta().y() == 0 || m_image.width() <= 0 || m_image.height() <= 0)
    {
//        event->ignore();
        return;
    }

    QRect const& margins = contentsRect();

    // Прокрутка колеса от себя (положительные значения дельта угла) - приблизить.
    // Алгоритм не учитывает степень (силу) прокрутки. Каждая прокрутка увеличивает
    // или уменьшает изображение в два раза.
    if(event->angleDelta().y() > 0)
    {
        if((event->modifiers() & Qt::ShiftModifier) == 0)
        {
//            x = (x - (event->pos().x() - image.width() / 4.0)) / 0.5;
//            x -= image.width() / 2.0 - event->pos().x();
            m_x = (m_x - event->pos().x() + margins.left()) / 0.5f;
            m_x += event->pos().x() - margins.left();
            m_horizontalResolution *= 0.5;
        }

        if((event->modifiers() & Qt::ControlModifier) == 0)
        {
//            y = (y - (event->pos().y() - image.height() / 4.0)) / 0.5;
//            y -= image.height() / 2.0 - event->pos().y();
            m_y = (m_y - event->pos().y() + margins.top()) / 0.5f;
            m_y += event->pos().y() - margins.top();
            m_verticalResolution *= 0.5;
        }
    }
    else if(event->angleDelta().y() < 0)
    {
        if((event->modifiers() & Qt::ShiftModifier) == 0)
        {
//            x = (x + image.width() / 2.0) / 2.0;
            m_x = (m_x - event->pos().x() + margins.left()) / 2.0f;
            m_x += event->pos().x() - margins.left();
            m_horizontalResolution *= 2.0;
        }

        if((event->modifiers() & Qt::ControlModifier) == 0)
        {
//            y = (y + image.height() / 2.0) / 2.0;
            m_y = (m_y - event->pos().y() + margins.top()) / 2.0f;
            m_y += event->pos().y() - margins.top();
            m_verticalResolution *= 2.0;
        }
    }

//    event->accept();

    updateGradientImage();
//    update();
}

template<typename ty>
void GradientMap<ty>::drawAll(QPainter& painter)
{
    QRect const& margins = contentsRect();

    painter.drawImage(margins.left(), margins.top(), m_image);

    // Здесь код для рисования сетки...

    // Здесь код для рисования подписей координат...

    if(m_showRubberband)
    {
//        painter.setPen(Qt::black);
        painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
        painter.setPen(QColor(0xFF, 0xFF, 0xFF));
        painter.drawRect(m_rubberband);
    }

//    if(isHintShown)
//    {
//        painter.drawText(xLastPosition, yLastPosition, "TEXT");
//    }
}

template<typename ty>
void GradientMap<ty>::resizeEvent(QResizeEvent* event)
{
    QRect const& margins = contentsRect();

    QSize const marginsSize = QSize(width() - margins.width(), height() - margins.height()),
        imageOldSize = event->oldSize() - marginsSize,
        imageSize = event->size() - marginsSize;

    if(imageSize.width() > 0 && imageSize.height() > 0)
    {
        m_image = QImage(imageSize.width(), imageSize.height(), QImage::Format_RGB32);

        if(imageOldSize.width() > 0 && imageOldSize.height() > 0)
        {
            m_x *= static_cast<float>(imageSize.width()) / static_cast<float>(imageOldSize.width());
            m_y *= static_cast<float>(imageSize.height()) / static_cast<float>(imageOldSize.height());
        }

        updateGradientImage();
//        update();
    }
}

#endif
