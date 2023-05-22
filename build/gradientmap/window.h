#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <vector>
#include <gradientmap.h>

class CustomWidget: public QWidget
{
    Q_OBJECT

public:
    CustomWidget(QWidget* parent = nullptr);
    virtual ~CustomWidget();

private slots:
    void updateFps();
    void catchPoint(int x, int y);

private:
    virtual void keyPressEvent(QKeyEvent* event);

    //std::vector<float*> _fvect;
    std::vector<int*> _ivect;

    //GradientMap<float>* _gmap;
    GradientMap<int>* _gmap;

    QTimer* _timer;

    QLabel* _FPSlabel;
    QLabel* _pointLabel;

    int _frames;

};

#endif
