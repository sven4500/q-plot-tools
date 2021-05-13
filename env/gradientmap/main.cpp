#include <QApplication>
#include <window.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    CustomWidget widget;
    widget.show();
    return app.exec();
}
