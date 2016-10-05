//#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include <QKeyEvent>
#include "MainWindow.h"
#include <stdlib.h>

int const& rowCount = 10,
    columnCount = 10;

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
    m_map = new CGradientMap<int>();
    m_map->setMargins(15, 15, 25, 25);
    m_map->setResolution(columnCount, rowCount);
    m_map->setCoolColor(32, 128, 192);
    m_map->setWarmColor(192, 96, 0);
    m_map->setMouseTracking(true);
    m_map->show();

//    connect(gmap, &CGradientMap<int>::pointSelected, this, &MainWindow::catchPoint);

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

    m_map->setMin(0);
    m_map->setMax(RAND_MAX);

    ivect.resize(rowCount, nullptr);

    for(int j = 0; j < rowCount; ++j)
    {
        int const adjustedColumnCount = columnCount + rand() % 10;
        ivect[j] = new int[adjustedColumnCount];
        for(int i = 0; i < adjustedColumnCount; ++i)
            ivect[j][i] = rand();
        m_map->add(j, ivect[j], adjustedColumnCount/*, rand() % 20*/);
    }

    m_map->updateGradientImage();
//    m_map->update();

    label = new QLabel(this);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            this, &MainWindow::updateFps);
    timer->start(1000);
}

MainWindow::~MainWindow()
{
//    for(int j = 0; j < 100; ++j)
//        delete[] fvect[j];
}

void MainWindow::catchPoint(int x, int y)
{
    QMessageBox::about(nullptr, "", QString("x: %1\ny: %2").arg(x).arg(y));
}

void MainWindow::updateFps()
{
    label->setText(QString::number(frames));
    frames = 0;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() != Qt::Key_F5)
        return;

//    for(int j = 0; j < 180; ++j)
//        for(int i = 0; i < 180; ++i)
//            vect[j][i] = float(rand()) / float(RAND_MAX);

    for(int j = 0; j < rowCount / 2; ++j)
        m_map->swap(rand() % rowCount, rand() % rowCount);

    m_map->updateGradientImage();
    m_map->update();

    ++frames;
}
