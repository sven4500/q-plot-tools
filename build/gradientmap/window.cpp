#include <QGridLayout>
#include <QTimer>
#include <QColor>
#include <QKeyEvent>
#include <cstdlib> // rand, srand
#include <ctime> // time
#include <window.h>

int const rowCount = 10, columnCount = 10;

CustomWidget::CustomWidget(QWidget* parent):
    QWidget(parent), _frames(0)
{
    // Create GradientMap widget with this parent. GradientMap is template and
    // any user type which converts to double can be specified.
    _gmap = new GradientMap<int>(this);
    _gmap->setResolution(columnCount, rowCount);

    // Set cool and warm color.
    _gmap->setCoolColor(QColor(32, 128, 192));
    _gmap->setWarmColor(QColor(192, 96, 0));

    // Set min and max values. Min value corresponds to cool color, while max
    // value corresponds to warm color. Everything what lies between is being
    // interpolated between cool and warm color.
    _gmap->setMin(0);
    _gmap->setMax(RAND_MAX);

    // Signal pointSelected fires whenever user clicks on GradientMap contents.
    connect(_gmap, &GradientMap<int>::pointSelected, this, &CustomWidget::catchPoint);

//    double* const addr = new double[4];
//    addr[0] = 0.25;
//    addr[1] = 0.75;
//    addr[2] = 0.50;
//    addr[3] = 1.0;
//    gmap->add(0, addr, 2);
//    gmap->add(1, addr + 2, 2);

//    gmap->setMin(0.0f);
//    gmap->setMax(1.0f);

//    fvect.resize(1000, nullptr);
//    for(int j = 0; j < 1000; ++j)
//    {
//        fvect[j] = new float[1000];
//        for(int i = 0; i < 1000; ++i)
//            fvect[j][i] = float(rand()) / float(RAND_MAX);
//        gmap->add(j, fvect[j], 1000);
//    }

    // Let us make rand a bit more "random".
    srand(time(nullptr) & 0xFFFF);

    _ivect.resize(rowCount, nullptr);

    for(int j = 0; j < rowCount; ++j)
    {
        int const adjustedColumnCount = columnCount + rand() % 10;

        _ivect[j] = new int[adjustedColumnCount];

        for(int i = 0; i < adjustedColumnCount; ++i)
            _ivect[j][i] = rand();

        _gmap->add(j, _ivect[j], adjustedColumnCount/*, rand() % 20*/);
    }

    _gmap->updateGradientImage();

    _FPSlabel = new QLabel(this);
    _pointLabel = new QLabel(this);

    // Create timer to measure frames per second.
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &CustomWidget::updateFps);

    _timer->start(1000);

    QGridLayout* const layout = new QGridLayout(this);
    //layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(_gmap, 0, 0, 10, 2);
    layout->addWidget(_FPSlabel, 10, 0);
    layout->addWidget(_pointLabel, 10, 1);

    setLayout(layout);

    resize(640, 480);
}

CustomWidget::~CustomWidget()
{
    for(auto line : _ivect)
        delete[] line;
}

void CustomWidget::catchPoint(int x, int y)
{
    QString const text = QString("x = %1, y = %2").arg(x).arg(y);
    _pointLabel->setText(text);
}

void CustomWidget::updateFps()
{
    // This method fires each 1 second. Simply print out number of frames drew
    // so far and start counting over.
    QString const text = QString("Press or hold F5 to swap lines. FPS: %1").arg(_frames);
    _FPSlabel->setText(text);

    _frames = 0;
}

void CustomWidget::keyPressEvent(QKeyEvent* event)
{
    if(event->key() != Qt::Key_F5)
        return;

//    for(int j = 0; j < 180; ++j)
//        for(int i = 0; i < 180; ++i)
//            vect[j][i] = float(rand()) / float(RAND_MAX);

    // Swap two random image lines.
    for(int j = 0; j < rowCount; ++j)
        _gmap->swap(rand() % rowCount, rand() % rowCount);

    _gmap->updateGradientImage();

    // When gradient map is updated, increment number of frames drew so far.
    ++_frames;
}
