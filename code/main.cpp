#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow workspace;
    workspace.showMaximized();
    //workspace.show();
    return a.exec();
}
