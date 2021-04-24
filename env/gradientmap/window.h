#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <vector>
#include <gradientmap.h>

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

private slots:

    void updateFps();
    void catchPoint(int x, int y);

private:

    virtual void keyPressEvent(QKeyEvent* event);

    std::vector<float*> _fvect;
    std::vector<int*> _ivect;

    GradientMap<int>* _gmap;
    QLabel* _label;

    int _frames;

};

#endif
