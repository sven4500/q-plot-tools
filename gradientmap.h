#ifndef __GRADIENTMAP_H
#define __GRADIENTMAP_H

#include <QWidget>
#include <QImage>
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
class CGradientMap: public QWidget
{
    Q_OBJECT

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
    // дескриптором. Метод add замещает ранее существующий id.
    void add(int id, double const* addr, int size, int bias = 0);
    void remove(int id);
    void swap(int id1, int id2);

    void setXPosition(float x);
    void setYPosition(float y);

    void setPosition(float x, float y);

    void setXResolution(float resolution);
    void setYResolution(float resolution);

    void setResolution(float xResolution, float yResolution);

    void setCoolColor(int red, int green ,int blue);
    void setCoolColor(QColor const& color);

    void setWarmColor(int red, int green ,int blue);
    void setWarmColor(QColor const& color);

    void setMin(double min);
    void setMax(double max);

//    double localMin()const;
//    double localMax()const;

    double globalMin()const;
    double globalMax()const;

    // Данные добавляются в карту по-строчно. Чтобы избежать ненужной перерисовки
    // каждый раз когда добваляется очередной вектор
    /*virtual*/ void updateGradientImage();

    // Возвращает значение для заданного пикселя карты [x,y]. Переменная hasValue возвращает
    // false если в данной точке нет данных. Здесь наверное лучше возвращать bool вместо значения
    // NaN для отсутствующих величин. Данная функция оказывется также полезной когда нужно.
//    inline bool value(int x, int y, double* value)const;

private:
    struct MemDesc
    {
        double const* addr;
        int size;
        int bias;
//        int id;
    };

    // Метод для вычисления базы цвета. Базой цвета я называю разность цветов warmColor - coolColor.
    void rebaseColor();

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    virtual void wheelEvent(QWheelEvent* event);

    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

    // Думаю здесь использовать QMap более уместно вместо QVector. QVector приводит к фрагментации
    // памяти при каждом добавлении очередного вектора данных. QMap в свою очередь хранит данные
    // в разных областях памяти. Добавление и удаление здесь быстрее.
    QMap<int, MemDesc> map;
    QImage image;

    QRect rubberband;
//    QString hint;

    int redCool;
    int greenCool;
    int blueCool;

    int redWarm;
    int greenWarm;
    int blueWarm;

    int redBase;
    int greenBase;
    int blueBase;

    double min;
    double max;
    double peakToPeak;

    // x и y обозначают пиксель верхнего левого угла картинки. Возникает ошибка округления
    // при изменении размера рабочей области окна, когда x и y имеют тип int.
    float x;
    float y;

    // Точка на которую указывал указатель мыши перед нажатием кнопки мыши.
    int xLastPosition;
    int yLastPosition;

    // Разрешение задаёт количество узлов помещающехся в рабочую область окна.
    float xResolution;
    float yResolution;

    bool isRubberbandShown;
//    bool isHintShown;
//    bool isDynamicRangeShown;

//    bool hasDynamicRange;
//    bool showHint;
};

#endif
