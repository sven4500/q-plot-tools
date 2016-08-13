#include <QPainter>
#include <QMouseEvent>
#include <QSize>
//#include <stdlib.h>
#include "gradientmap.h"

// todo: нужен холодный и тёплый цвета по-умолчанию.
CGradientMap::CGradientMap(QWidget* parent): QWidget(parent)
{
    x = 0.0;
    y = 0.0;

    xResolution = 1.0;
    yResolution = 1.0;

    min = 0.0;
    max = 1.0;
    peakToPeak = 1.0;

    // Здесь происходит вызов rebaseColor дважды.
    setCoolColor(0, 0, 0);
    setWarmColor(255, 255, 255);

//    redCool = 0;
//    greenCool = 0;
//    blueCool = 0;

//    redWarm = 255;
//    greenWarm = 255;
//    blueWarm = 255;

//    redBase = 255;
//    greenBase = 255;
//    blueBase = 255;

    isRubberbandShown = false;
//    isHintShown = false;
}

CGradientMap::~CGradientMap()
{

}

void CGradientMap::add(int id, double const* addr, int size, int bias)
{
    if(addr != nullptr && size > 0)
    {
        MemDesc& memDesc = map[id];
        memDesc.addr = addr;
        memDesc.size = size;
        memDesc.bias = bias;
    }
}

void CGradientMap::remove(int id)
{
    map.remove(id);
}

void CGradientMap::swap(int const id1, int const id2)
{
    if(map.contains(id1) == true || map.contains(id2) == true)
        std::swap(map[id1], map[id2]);
}

void CGradientMap::clear()
{
    map.clear();
}

//void CGradientMap::adjustAllContent()
//{

//}

void CGradientMap::setXPosition(float x)
{
    CGradientMap::x = x;
}

void CGradientMap::setYPosition(float y)
{
    CGradientMap::y = y;
}

void CGradientMap::setPosition(float x, float y)
{
    CGradientMap::x = x;
    CGradientMap::y = y;
}

void CGradientMap::setXResolution(float resolution)
{
    xResolution = resolution;
}

void CGradientMap::setYResolution(float resolution)
{
    yResolution = resolution;
}

void CGradientMap::setResolution(float xResolution, float yResolution)
{
    CGradientMap::xResolution = xResolution;
    CGradientMap::yResolution = yResolution;
}

void CGradientMap::setMin(double min)
{
    CGradientMap::min = min;
    CGradientMap::peakToPeak = max - min;
}

void CGradientMap::setMax(double max)
{
    CGradientMap::max = max;
    CGradientMap::peakToPeak = max - min;
}

void CGradientMap::setCoolColor(int const red, int const green ,int const blue)
{
    redCool = red;
    greenCool = green;
    blueCool = blue;
    rebaseColor();
}

void CGradientMap::setCoolColor(QColor const& color)
{
    redCool = color.red();
    greenCool = color.green();
    blueCool = color.blue();
    rebaseColor();
}

void CGradientMap::setWarmColor(int const red, int const green ,int const blue)
{
    redWarm = red;
    greenWarm = green;
    blueWarm = blue;
    rebaseColor();
}

void CGradientMap::setWarmColor(QColor const& color)
{
    redWarm = color.red();
    greenWarm = color.green();
    blueWarm = color.blue();
    rebaseColor();
}

void CGradientMap::rebaseColor()
{
    redBase = redWarm - redCool;
    greenBase = greenWarm - greenCool;
    blueBase = blueWarm - blueCool;
}

//double CGradientMap::localMin()const
//{
//    float const xFactor = width / xResolution,
//            yFactor = height / yResolution;

//    float const iy = (j - y) / yFactor;
//}

double CGradientMap::globalMin()const
{
    if(map.empty())
        return std::numeric_limits<double>::min();

    QMap<int, MemDesc>::const_iterator const end = map.cend();
    QMap<int, MemDesc>::const_iterator iter = map.begin();
    double min = std::numeric_limits<double>::max();

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

double CGradientMap::globalMax()const
{
    if(map.empty())
        return std::numeric_limits<double>::max();

    QMap<int, MemDesc>::const_iterator const end = map.cend();
    QMap<int, MemDesc>::const_iterator iter = map.begin();
    double max = std::numeric_limits<double>::min();

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

//bool CGradientMap::value(int x, int y, double* value)const
//{
//    int const ix = (float(x) - CGradientMap::x) / xResolution,
//            iy = (float(y) - CGradientMap::y) / yResolution;
//    return (iy >= 0 && iy < map.size() && ix < map[iy].size - map[iy].bias && ix > map
//}

void CGradientMap::updateGradientImage()
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

    QMap<int, MemDesc>::const_iterator const begin = map.cbegin();

    for(int j = 0; j < height; ++j)
    {
        float const iy = (j - y) / yFactor;
        MemDesc const* const memDesc = (iy >= 0.0 && iy < map.size()) ? &*(begin + iy) : nullptr;

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
                double const value = memDesc->addr[int(ix)];

                // Войти в режим назыщения если текущее значение лежит вне диапазона.
                if(value < min)
                {
                    *pixel++ = blueCool;
                    *pixel++ = greenCool;
                    *pixel++ = redCool;
                }
                else if(value > max)
                {
                    *pixel++ = blueWarm;
                    *pixel++ = greenWarm;
                    *pixel++ = redWarm;
                }
                else
                {
                    float const factor = (value - min) / peakToPeak;
                    *pixel++ = blueCool + blueBase * factor;
                    *pixel++ = greenCool + greenBase * factor;
                    *pixel++ = redCool + redBase * factor;
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

void CGradientMap::mousePressEvent(QMouseEvent* event)
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

void CGradientMap::mouseMoveEvent(QMouseEvent* event)
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

void CGradientMap::mouseReleaseEvent(QMouseEvent* event)
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

void CGradientMap::wheelEvent(QWheelEvent* event)
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

void CGradientMap::paintEvent(QPaintEvent* /*event*/)
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
void CGradientMap::resizeEvent(QResizeEvent* event)
{
    QSize const& oldSize = event->oldSize(),
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




































