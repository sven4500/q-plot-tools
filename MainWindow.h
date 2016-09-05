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
    void catchPoint(int x, int y);

private:
    virtual void keyPressEvent(QKeyEvent* event);

    std::vector<float*> fvect;
    std::vector<int*> ivect;

    CGradientMap<int>* gmap;
    QLabel* label;

    int frames;
};

#endif
