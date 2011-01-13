#include <QtGui/QApplication>
#include "stegodialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StegoDialog w;
    w.show();
    return a.exec();
}
