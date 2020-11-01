#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("KM File Manager");
    w.setWindowIcon(QIcon("./icons/logo.png"));
    w.show();
    return a.exec();
}