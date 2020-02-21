#include "mainwindow.h"
#include "ExitWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/ico/TweeEditor.jpg"));
    w.showMaximized();
    return a.exec();
}
