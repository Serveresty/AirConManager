#include "../includes/coolwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CoolWindow cw;
    cw.show();

    return a.exec();
}