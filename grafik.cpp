#include "grafik.h"
#include "ui_grafik.h"
#include "QMessageBox"

grafik::grafik(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::grafik)
{
    ui->setupUi(this);
    connect(ui->NumofGraf,SIGNAL(valueChanged(int)),this,SLOT(ChooseNumGraf(int)));
}

grafik::~grafik()
{
    delete tpolar;
    delete qsp;
    delete ui;
}

void grafik::ChooseNumGraf(int numgraf)
{
    ShowGraf(numgraf - 1);
}

void grafik::SendDataGraf(MainWindow::tPolar &tpolar1)
{
    tpolar = new MainWindow::tPolar(tpolar1);
    //qsp = new QSpinBox(ui->NumofGraf);
    qsp = (ui->NumofGraf);
    qsp->setMinimum(1);
    qsp->setMaximum(tpolar->nfun);
    ShowGraf();
}

void grafik::ShowGraf(int numgraf)
{
    QVector<double> x(tpolar->num_of_points), y(tpolar->num_of_points);

    for(int i = 0; i<tpolar->num_of_points; i++)
    {
        x[i] = tpolar->init_x + tpolar->samp_freq * i;
        y[i] = tpolar->data_y[numgraf][i];
    }

    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x,y);
    ui->widget->graph(0)->setPen(QPen(Qt::red));
    ui->widget->xAxis->setLabel("x,  мс");
    ui->widget->yAxis->setLabel("y,  Дб");
    ui->widget->xAxis->setRange(tpolar->init_x,(tpolar->init_x + tpolar->num_of_points * tpolar->samp_freq));

    float yMin = tpolar->data_y[numgraf][0], yMax = tpolar->data_y[numgraf][0];
    for(int i = 0; i<tpolar->num_of_points; i++)
    {
        if(yMin > tpolar->data_y[numgraf][i]){yMin = tpolar->data_y[numgraf][i];}
        if(yMax < tpolar->data_y[numgraf][i]){yMax = tpolar->data_y[numgraf][i];}

    }
     ui->widget->yAxis->setRange(yMin, yMax);
     ui->widget->replot();
}
