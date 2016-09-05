//#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include <QKeyEvent>
#include "MainWindow.h"
#include <stdlib.h>

int const& rowCount = 1000,
    columnCount = 1000;

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
    gmap = new CGradientMap<int>();
    gmap->setMargins(15, 15, 25, 25);
    gmap->setResolution(columnCount, rowCount);
    gmap->setCoolColor(32, 128, 192);
    gmap->setWarmColor(192, 96, 0);
    gmap->setMouseTracking(true);
    gmap->show();

    connect(gmap, &CGradientMap<int>::pointSelected, this, &MainWindow::catchPoint);

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

    gmap->setMin(0);
    gmap->setMax(RAND_MAX);

    ivect.resize(1000, nullptr);
    for(int j = 0; j < rowCount; ++j)
    {
        ivect[j] = new int[columnCount];
        for(int i = 0; i < columnCount; ++i)
            ivect[j][i] = rand();
        gmap->add(j, ivect[j], columnCount);
    }

    gmap->updateGradientImage();
    gmap->update();

    label = new QLabel(this);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFps);
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
        gmap->swap(rand() % rowCount, rand() % rowCount);

    gmap->updateGradientImage();
    gmap->update();

    ++frames;
}
