#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <vector>
#include "gradientmap.h"

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void updateFps();

private:
    virtual void keyPressEvent(QKeyEvent* event);

    std::vector<float*> vect;

    CGradientMap<float>* gmap;
    QLabel* label;

    int frames;
};

#endif
