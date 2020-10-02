#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readfilepath.h"
#include "QMessageBox"
#include "QFile"
#include "QDir"
#include "QTextStream"
#include "QDataStream"
#include "grafik.h"
#include "QtCore"

MainWindow::tPolar tpolar;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->filetext->setReadOnly(true);
    grafik *grf = new grafik(this);
    connect(this,SIGNAL(FileisOpen()),this,SLOT(SaveDataFile()));
    connect(ui->BuildGraph,SIGNAL(triggered()),grf,SLOT(show()));
    connect(this,SIGNAL(BuildGraf(MainWindow::tPolar&)),grf,SLOT(SendDataGraf(MainWindow::tPolar&)));

}

MainWindow::~MainWindow()
{
    for(int i = 0; i<tpolar.nfun;i++)
    {
        delete[] tpolar.data_y[i];
    }
    delete[] tpolar.data_y;
    delete ui;
}

void MainWindow::on_OpenFile_triggered()
{
    ReadFilePath *wnd = new ReadFilePath(this);
    wnd->show();
    connect(wnd,SIGNAL(FilePath(QString)),this,SLOT(ReadFile(QString)));
}
//чтение данных из файла
void MainWindow::ReadFile(const QString &path)
{
    QFile file(path);
    //QDataStream in(&file);


    if(!(file.open(QFile::ReadOnly | QFile::Text)))
    {
        QMessageBox::warning(this,"ошибка","неверный путь к файлу!");
    }
    else
    {
        QTextStream readfile(&file);
        ui->filetext->setText(readfile.readAll());
    }

    file.close();
    emit FileisOpen();
}
//функция для записи данных из файла в структуру
void MainWindow::SaveDataFile()
{
    QString DataFile = ui->filetext->toPlainText().trimmed();
    QStringList lst = DataFile.split("\n",QString::SkipEmptyParts);

    QStringList lst_dtx_name = lst[0].split(" ",QString::SkipEmptyParts);
    tpolar.name = lst_dtx_name[1].toStdString().c_str();

    QStringList lst_x_cor = lst[1].split(" ",QString::SkipEmptyParts);
    tpolar.num_of_points = lst_x_cor[1].toFloat();
    tpolar.init_x = lst_x_cor[2].toFloat();
    tpolar.samp_freq = lst_x_cor[3].toFloat();

    QStringList num_of_graf = lst[12].split(" ",QString::SkipEmptyParts);
    tpolar.nfun = num_of_graf[1].toInt();

    QStringList data_to_convert_to_log = lst[19].split(" ",QString::SkipEmptyParts);
    tpolar.dlog = data_to_convert_to_log[1].toFloat();
    tpolar.ref = data_to_convert_to_log[2].toFloat();


}


void MainWindow::on_BuildGraph_triggered()
{
    QString file_path = "C:\\Users\\HP\\Desktop\\Диплом\\Обработка\\spectr\\" + tpolar.name;
    QFile file_dtx(file_path);
    tpolar.data_y = new float *[tpolar.nfun];
    int i = 0,j = 0;

    if(!(file_dtx.open(QIODevice::ReadOnly)))
    {
        QMessageBox::warning(this,"ошибка","неправильный путь к файлу");
    }
    else
    {
        for(; i<tpolar.nfun; i++)  //считываем данные из dtx файла в двумерный массив структуры
        {
            tpolar.data_y[i] = new float [tpolar.num_of_points];
            j = 0;

            for(; j<tpolar.num_of_points && !(file_dtx.atEnd()); j++)
            {
                file_dtx.read((char*)&tpolar.data_y[i][j],sizeof(float));
                tpolar.data_y[i][j] = tpolar.dlog*log10f(tpolar.data_y[i][j] * tpolar.ref);  //перевод данных в децибелы
            }
        }
    }

    emit BuildGraf(tpolar);
}
