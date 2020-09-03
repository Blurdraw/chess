#include "mainwindow.h"

#include <QApplication>
#include "singleGame.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SingleGame sgame;
    sgame.resize(600,500);
    sgame.show();
    return a.exec();
}
