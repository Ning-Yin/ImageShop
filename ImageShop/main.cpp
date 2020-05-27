#include "ImageShop.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageShop w;
    w.show();
    return a.exec();
}
