#ifndef GRAFIK_H
#define GRAFIK_H

#include <QDialog>
#include "mainwindow.h"
#include <QSpinBox>

namespace Ui {
class grafik;
}

class grafik : public QDialog
{
    Q_OBJECT

public:
    explicit grafik(QWidget *parent = nullptr);
    ~grafik();

private:
    Ui::grafik *ui;
    MainWindow::tPolar *tpolar;
    QSpinBox *qsp;
signals:
private slots:
    void ShowGraf(int numgraf = 0);
    void ChooseNumGraf(int numgraf);
    void SendDataGraf(MainWindow::tPolar &tpolar);
};

#endif // GRAFIK_H
