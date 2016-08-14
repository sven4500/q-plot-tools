//#include <QThread>
#include <QTimer>
#include <QKeyEvent>
#include "MainWindow.h"
#include <stdlib.h>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
//    double* const addr = new double[4];
//    addr[0] = 0.25;
//    addr[1] = 0.75;
//    addr[2] = 0.50;
//    addr[3] = 1.0;

    gmap = new CGradientMap<float>();
    gmap->setMin(0.0f);
    gmap->setMax(1.0f);
    gmap->setResolution(1000.0, 1000.0);
//    gmap->add(0, addr, 2);
//    gmap->add(1, addr + 2, 2);
    gmap->setCoolColor(32, 128, 192);
    gmap->setWarmColor(192, 96, 0);
    gmap->setMouseTracking(true);
//	gmap->setMouseTracknig(true);
    gmap->show();

    vect.resize(1000, nullptr);
    for(int j = 0; j < 1000; ++j)
    {
        vect[j] = new float[1000];
        for(int i = 0; i < 1000; ++i)
            vect[j][i] = float(rand()) / float(RAND_MAX);
        gmap->add(j, vect[j], 1000);
    }

    gmap->updateGradientImage();
    gmap->update();

//    double min = gmap->globalMin(),
//            max = gmap->globalMax();

    label = new QLabel(this);

    QTimer* const timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFps);
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    for(int j = 0; j < 100; ++j)
        delete[] vect[j];
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

    for(int i = 0; i < 100; ++i)
        gmap->swap(rand() % 1000, rand() % 1000);

    gmap->updateGradientImage();
    gmap->update();

    ++frames;
}
