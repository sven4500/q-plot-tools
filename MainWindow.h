#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QLabel>
#include <stdlib.h>
#include "gradientmap.h"

//class CCustomThread: public QThread
//{
//    Q_OBJECT

//public:
//    virtual void run();
//};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateFps();

private:
    virtual void keyPressEvent(QKeyEvent* event);

    std::vector<double*> vect;
    CGradientMap* gmap;
    QLabel* label;

//    int seconds;
    int frames;
};

#endif // MAINWINDOW_H
