#include <stdio.h> // rand, srand
#include <time.h> // time
#include <QApplication>
#include <widget.h>

int main(int argc, char** argv)
{
    srand(time(nullptr) & 0xFFFF);
    QApplication app(argc, argv);
    Widget widget;
    widget.show();
    return app.exec();
}
