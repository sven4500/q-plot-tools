#ifndef __GRADIENTMAP_H
#define __GRADIENTMAP_H

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QImage>
#include <QSize>
//#include <QColor>
#include <QMap>

// Абстрактный базовый класс для того чтобы его ребёнок мог стать шаблоном.
// Данный метод имеет большое ограничение - он не позволяет шаблонам детей
// этого класса иметь сигналы и слоты.
class CGradientMapBase: public QWidget
{
    Q_OBJECT

public:
    CGradientMapBase(QWidget* parent = nullptr): QWidget(parent)
    {

    }

protected:
    virtual ~CGradientMapBase()
    {

    }
};

// CGradientMap должен был стать шаблоном, однако Qt не поддерживает шаблонные классы,
// неследованные от QWidget. Это связано с определёнными ограничениями препроцессора moc.
// Какая жалость!
// todo: сигнализировать если выбран узел. Возвращать его координаты и значение.
// void itemSelected(double value, int ix = 0, int iy = 0);
template<typename ty>
class CGradientMap: public CGradientMapBase
{
public:
    CGradientMap(QWidget* parent = nullptr);
    virtual ~CGradientMap();

    void clear();

//    void hasDynamicRange(bool m_hasDynamicRange);

    // Если true, то при передвижении курсора, виджет показывает небольшое сообщение
    // со значением карты в данной точке. setMouseTracking также должен быть включен.
//    void showHint(bool isHintShown);

    // adjustContent вписывает лишь общую часть целого рисунка. adjustAllContent вписывает
    // рисунок полностью. Может привести к выпаданию данных в некоторых скан-линиях.
//    void adjustContent();
//    void adjustAllContent();

    // Метод add добавляет вектор данных в вкарту. Каждый вектор характеризуется своим
    // дескриптором id. Метод add замещает существующий дескриптор.
    void add(int id, ty const* addr, int size, int bias = 0);
    void remove(int id);

    void swap(int id1, int id2);

    void setXPosition(int x);
    void setYPosition(int y);

    void setPosition(int x, int y);

    void setXResolution(float resolution);
    void setYResolution(float resolution);

    void setResolution(float xResolution, float yResolution);

    void setCoolColor(int red, int green ,int blue);
    void setCoolColor(QColor const& color);

    void setWarmColor(int red, int green ,int blue);
    void setWarmColor(QColor const& color);

    void setMin(ty const& min);
    void setMax(ty const& max);

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
    struct MemDescConst
    {
        ty const* addr;
        int size;
        int bias;
//        int id;
    };

    struct Color
    {
        int red;
        int green;
        int blue;
    };

    // Метод для обновления разности холодного и тёплого цветов.
    void updateDifferenceColor();

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    virtual void wheelEvent(QWheelEvent* event);

    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

    // Думаю здесь использовать QMap более уместно вместо QVector. QVector приводит к фрагментации
    // памяти при каждом добавлении очередного вектора данных. QMap в свою очередь хранит данные
    // в разных областях памяти. Добавление и удаление здесь быстрее.
    QMap<int, MemDescConst> map;
    QImage image;

    QRect rubberband;
//    QString hint;

    Color coolColor;
    Color warmColor;
    Color differenceColor;

    float min;
    float max;
    float peakToPeak;

    // x и y обозначают пиксель верхнего левого угла картинки. Возникает ошибка округления
    // при изменении размера рабочей области окна, когда x и y имеют тип int.
    float x;
    float y;

    // Точка на которую указывал указатель мыши перед нажатием кнопки мыши.
    // Используются только методами обработки событий мыши.
    float xLastPosition;
    float yLastPosition;

    // Разрешение задаёт количество узлов помещающехся в рабочую область окна.
    float xResolution;
    float yResolution;

    bool isRubberbandShown;
//    bool isHintShown;
//    bool isDynamicRangeShown;

//    bool hasDynamicRange;
//    bool showHint;
};

template<typename ty>
CGradientMap<ty>::CGradientMap(QWidget* parent): CGradientMapBase(parent)
{
    x = 0.0;
    y = 0.0;

    xResolution = 1.0;
    yResolution = 1.0;

//    min = 0.0;
//    max = 1.0;
//    peakToPeak = 1.0;

    setCoolColor(0, 0, 0);
    setWarmColor(255, 255, 255);

    isRubberbandShown = false;
//    isHintShown = false;
}

template<typename ty>
CGradientMap<ty>::~CGradientMap()
{

}

template<typename ty>
void CGradientMap<ty>::clear()
{
    map.clear();
}

template<typename ty>
void CGradientMap<ty>::add(int id, ty const* addr, int size, int bias)
{
    if(addr != nullptr && size > 0)
    {
        MemDescConst& memDesc = map[id];
        memDesc.addr = addr;
        memDesc.size = size;
        memDesc.bias = bias;
    }
}

template<typename ty>
void CGradientMap<ty>::remove(int id)
{
    map.remove(id);
}

template<typename ty>
void CGradientMap<ty>::swap(int id1, int id2)
{
    if(map.contains(id1) && map.contains(id2))
        std::swap(map[id1], map[id2]);
}

template<typename ty>
void CGradientMap<ty>::setXPosition(int x)
{
    CGradientMap::x = x;
}

template<typename ty>
void CGradientMap<ty>::setYPosition(int y)
{
    CGradientMap::y = y;
}

template<typename ty>
void CGradientMap<ty>::setPosition(int x, int y)
{
    CGradientMap::x = x;
    CGradientMap::y = y;
}

template<typename ty>
void CGradientMap<ty>::setXResolution(float resolution)
{
    CGradientMap::xResolution = resolution;
}

template<typename ty>
void CGradientMap<ty>::setYResolution(float resolution)
{
    CGradientMap::yResolution = resolution;
}

template<typename ty>
void CGradientMap<ty>::setResolution(float xResolution, float yResolution)
{
    CGradientMap::xResolution = xResolution;
    CGradientMap::yResolution = yResolution;
}

template<typename ty>
void CGradientMap<ty>::setMin(ty const& min)
{
    CGradientMap::min = static_cast<float>(min);
    CGradientMap::peakToPeak = max - CGradientMap::min;
}

template<typename ty>
void CGradientMap<ty>::setMax(ty const& max)
{
    CGradientMap::max = static_cast<float>(max);
    CGradientMap::peakToPeak = CGradientMap::max - min;
}

template<typename ty>
void CGradientMap<ty>::setCoolColor(int const red, int const green, int const blue)
{
    coolColor.red = red;
    coolColor.green = green;
    coolColor.blue = blue;
    updateDifferenceColor();
}

template<typename ty>
void CGradientMap<ty>::setCoolColor(QColor const& color)
{
    coolColor.red = color.red();
    coolColor.green = color.green();
    coolColor.blue = color.blue();
    updateDifferenceColor();
}

template<typename ty>
void CGradientMap<ty>::setWarmColor(int const red, int const green, int const blue)
{
    warmColor.red = red;
    warmColor.green = green;
    warmColor.blue = blue;
    updateDifferenceColor();
}

template<typename ty>
void CGradientMap<ty>::setWarmColor(QColor const& color)
{
    warmColor.red = color.red();
    warmColor.green = color.green();
    warmColor.blue = color.blue();
    updateDifferenceColor();
}

template<typename ty>
void CGradientMap<ty>::updateDifferenceColor()
{
    differenceColor.red = warmColor.red - coolColor.red;
    differenceColor.green = warmColor.green - coolColor.green;
    differenceColor.blue = warmColor.blue - coolColor.blue;
}

template<typename ty>
ty CGradientMap<ty>::globalMin()const
{
    ty min;

    if(map.empty())
        return min;

    typename QMap<int, MemDescConst>::const_iterator const end = map.cend();
    typename QMap<int, MemDescConst>::const_iterator iter = map.begin();

    // В карте нет пустых векторов, поэтому addr[0] возможно.
    min = iter->addr[0];

    while(iter != end)
    {
        MemDescConst const& memDesc = *iter;
        for(int i = 0; i < memDesc.size; ++i)
            if(memDesc.addr[i] < min)
                min = memDesc.addr[i];
        ++iter;
    }

    return min;
}

template<typename ty>
ty CGradientMap<ty>::globalMax()const
{
    ty max;

    if(map.empty())
        return max;

    typename QMap<int, MemDescConst>::const_iterator const end = map.cend();
    typename QMap<int, MemDescConst>::const_iterator iter = map.begin();

    max = iter->addr[0];

    while(iter != end)
    {
        MemDescConst const& memDesc = *iter;
        for(int i = 0; i < memDesc.size; ++i)
            if(memDesc.addr[i] > max)
                max = memDesc.addr[i];
        ++iter;
    }

    return max;
}

template<typename ty>
void CGradientMap<ty>::updateGradientImage()
{
    // Самый большой метод данного класса. Мог бы быть меньше, если убрать ветвление насышения.
    // Однако такое ветвление помогает несколько повысить производительность.
    // backgroundTileSize размер чередующихся чёрно-белых квадратов фонового рисунка.
    int const width = image.width(),
            height = image.height(),
            backgroundTileSize = 8;

    // Фактор задаёт количество пикселей на один узел.
    float const xFactor = width / xResolution,
            yFactor = height / yResolution;

    typename QMap<int, MemDescConst>::const_iterator const begin = map.cbegin();

    for(int j = 0; j < height; ++j)
    {
        float const iy = (j - y) / yFactor;
        MemDescConst const* const memDesc = (iy >= 0.0 && iy < map.size()) ? &*(begin + iy) : nullptr;

        // Здесь есть интересный момент. Сообщество Qt рекомендует использовать структуру QRgb
        // вместо прямого обращения к отдельным каналам, т.к. на разных платформах может быть
        // разный порядок байт. Однако вызывать методы слишком накладно поэтому здесь мы
        // обращаемся к каналам напрямую через указатель на unsigned char. Формат изображения
        // всегда фиксированный поэтому худшее что может случиться это искажения цветов.
        // Можно пробовать совсем избавиться от вызова scanLine. Меньше вызовов - быстрее отрисовка.
        unsigned char* pixel = image.scanLine(j);

        for(int i = 0; i < width; ++i)
        {
            float const ix = (i - x) / xFactor;

            if(memDesc != nullptr && ix >= 0.0 && ix < memDesc->size)
            {
                float const value = static_cast<float>(memDesc->addr[static_cast<int>(ix)]);

                // Войти в режим назыщения если текущее значение лежит вне диапазона.
                if(value < min)
                {
                    *pixel++ = coolColor.blue;
                    *pixel++ = coolColor.green;
                    *pixel++ = coolColor.red;
                }
                else if(value > max)
                {
                    *pixel++ = warmColor.blue;
                    *pixel++ = warmColor.green;
                    *pixel++ = warmColor.red;
                }
                else
                {
                    float const factor = (value - min) / peakToPeak;
                    *pixel++ = coolColor.blue + differenceColor.blue * factor;
                    *pixel++ = coolColor.green + differenceColor.green * factor;
                    *pixel++ = coolColor.red + differenceColor.red * factor;
                }
            }
            else
            {
                int const grayscale = ((j / backgroundTileSize + (i / backgroundTileSize) % 2) % 2 == 0) ? 192 : 255;
                *pixel++ = grayscale;
                *pixel++ = grayscale;
                *pixel++ = grayscale;
            }

            ++pixel;
        }
    }
}

template<typename ty>
void CGradientMap<ty>::mousePressEvent(QMouseEvent* event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
        rubberband.setTopLeft(event->pos());
        rubberband.setBottomRight(event->pos());
        isRubberbandShown = true;
        setCursor(Qt::CrossCursor);
        break;
    case Qt::RightButton:
        xLastPosition = event->pos().x();
        yLastPosition = event->pos().y();
        setCursor(Qt::ClosedHandCursor);
        break;
    default:
        break;
    }
}

template<typename ty>
void CGradientMap<ty>::mouseMoveEvent(QMouseEvent* event)
{
    switch(event->buttons())
    {
    case Qt::LeftButton:
        // Показывать rubberband только когда его размер больше 4x4
        rubberband.setBottomRight(event->pos());
        update();
        break;
    case Qt::RightButton:
        if((event->modifiers() & Qt::ShiftModifier) == 0)
            x += event->pos().x() - xLastPosition;

        if((event->modifiers() & Qt::ControlModifier) == 0)
            y += event->pos().y() - yLastPosition;

        xLastPosition = event->pos().x();
        yLastPosition = event->pos().y();

        updateGradientImage();
        update();
        break;
    default:
        xLastPosition = event->pos().x();
        yLastPosition = event->pos().y();
        // Здесь код для получения значения ткущего пикселя...
        // по пустой строке paintEvent может понять седует ли перерисовывать подсказку
        update();
        break;
    }
}

template<typename ty>
void CGradientMap<ty>::mouseReleaseEvent(QMouseEvent* event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
        rubberband = rubberband.normalized();

        if(rubberband.width() > 4 && rubberband.height() > 4)
        {
            float const xFactor = float(rubberband.width()) / float(image.width()),
                    yFactor = float(rubberband.height()) / float(image.height());

            xResolution *= xFactor;
            yResolution *= yFactor;

            x = (x - rubberband.x()) / xFactor;
            y = (y - rubberband.y()) / yFactor;
        }
        else
        {
            // Сигнализировать выбор узла...
            // emit nodeSelected();
        }

        setCursor(Qt::ArrowCursor);

        isRubberbandShown = false;

        updateGradientImage();
        update();
        break;
    case Qt::RightButton:
        setCursor(Qt::ArrowCursor);
        break;
    case Qt::MidButton:
        // Вписать карту в рабочую облась при повторном щелчке вписать толькоцелую часть...
        // Нет смысла обрабатывать событие нажатия кнопки, только её отпускание
        break;
    default:
//        setCursor(Qt::ArrowCursor);
        break;
    }
}

template<typename ty>
void CGradientMap<ty>::wheelEvent(QWheelEvent* event)
{
    // Сообщество Qt настоятельно рекомендует принять событие если оно было обработано,
    // или отклонить его если оно не было обработано.
    if(event->angleDelta().y() == 0 || image.width() <= 0 || image.height() <= 0)
    {
        event->ignore();
        return;
    }

    // Прокрутка колеса от себя (положительные значения дельта угла) - приблизить.
    // Алгоритм не учитывает степень (силу) прокрутки. Каждая прокрутка увеличивает
    // или уменьшает изображение в два раза.
    if(event->angleDelta().y() > 0)
    {
        if((event->modifiers() & Qt::ShiftModifier) == 0)
        {
//            x = (x - (event->pos().x() - image.width() / 4.0)) / 0.5;
//            x -= image.width() / 2.0 - event->pos().x();
            x = (x - event->pos().x()) / 0.5;
            x += event->pos().x();
            xResolution *= 0.5;
        }

        if((event->modifiers() & Qt::ControlModifier) == 0)
        {
//            y = (y - (event->pos().y() - image.height() / 4.0)) / 0.5;
//            y -= image.height() / 2.0 - event->pos().y();
            y = (y - event->pos().y()) / 0.5;
            y += event->pos().y();
            yResolution *= 0.5;
        }
    }
    else
    {
        if((event->modifiers() & Qt::ShiftModifier) == 0)
        {
//            x = (x + image.width() / 2.0) / 2.0;
            x = (x - event->pos().x()) / 2.0;
            x += event->pos().x();
            xResolution *= 2.0;
        }

        if((event->modifiers() & Qt::ControlModifier) == 0)
        {
//            y = (y + image.height() / 2.0) / 2.0;
            y = (y - event->pos().y()) / 2.0;
            y += event->pos().y();
            yResolution *= 2.0;
        }
    }

    event->accept();

    updateGradientImage();
    update();
}

template<typename ty>
void CGradientMap<ty>::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.drawImage(0, 0, image);

    // Здесь код для рисования сетки...

    // Здесь код для рисования подписей координат...

    if(isRubberbandShown)
    {
//        painter.setPen(Qt::black);
        painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
        painter.setPen(QColor(0xFF, 0xFF, 0xFF));
        painter.drawRect(rubberband);
    }

//    if(isHintShown)
//    {
//        painter.drawText(xLastPosition, yLastPosition, "TEXT");
//    }
}

// todo: зделать чтобы угол карты смещался вслед за изменением размера
template<typename ty>
void CGradientMap<ty>::resizeEvent(QResizeEvent* event)
{
    QSize const oldSize = event->oldSize(),
            newSize = event->size();

    if(newSize.width() > 0 && newSize.height() > 0)
    {
        image = QImage(width(), height(), QImage::Format_RGB32);

        if(oldSize.width() > 0 && oldSize.height() > 0)
        {
            float const xFactor = x / oldSize.width(),
                    yFactor = y / oldSize.height();

            x = newSize.width() * xFactor;
            y = newSize.height() * yFactor;
        }

        updateGradientImage();
//        update();
    }
}

#endif
