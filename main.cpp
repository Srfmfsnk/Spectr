#include "mainwindow.h"
#include "grafik.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Спектральный анализ");
    w.show();
    return a.exec();
}
