#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("KManager");
    w.setWindowIcon(QIcon(":/images/icons/logo.png"));
    w.show();
    return a.exec();
}
