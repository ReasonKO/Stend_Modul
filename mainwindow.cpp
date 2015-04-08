#include "mainwindow.h"
#include "ui_mainwindow.h"

#include"QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&mainalg.worker,SIGNAL(UpdatePauseState(QString)),this,SLOT(UpdatePauseState(QString)));
    connect(&mainalg.worker, SIGNAL(StatusMessage(QString)), this, SLOT(UpdateStatusBar(QString)));
    connect(this,SIGNAL(MatlabPause()),&mainalg.worker,SLOT(Pause()));
    connect(this,SIGNAL(runML()),&mainalg.worker,SLOT(run()));
    connect(&mltimer,SIGNAL(timeout()),&mainalg.worker,SLOT(run()));
    connect(this, SIGNAL(MLEvalString(QString)),&mainalg.worker,SLOT(EvalString(QString)));
    connect(&remotecontol,SIGNAL(RC_control(int,int,int,int)),this,SLOT(RCmessage(int,int,int,int)));
    mainalg.init();
    mainalg.start();
}

MainWindow::~MainWindow()
{
    qDebug()<<"destroyMW";
    delete ui;
}

#include <QFileDialog>


void MainWindow::UpdatePauseState(QString mess)
{
    ui->label_Pause->setText(mess);
}
void MainWindow::on_pushButton_Pause_clicked()
{
    emit MatlabPause();
}

void MainWindow::RCmessage(int L,int R,int K,int B)
{
    QString s="RP.Yellow(12).rul.left="+QString::number(L)+";"
            +"RP.Yellow(12).rul.right="+QString::number(R)+";"
            +"RP.Yellow(12).rul.kick="+QString::number(K)+";";
    //QString s="MOD_NGO(12,'Y',["
    //        +QString::number(L)+","
    //        +QString::number(R)+","
    //        +QString::number(K)+","
    //        +QString::number(B)+"]);";
    //qDebug()<<s;
    emit MLEvalString(s);
}
void MainWindow::on_pushButton_2_clicked()
{
    QString  dir = QFileDialog::getExistingDirectory();
    QString  s="cd "+dir;
    qDebug()<<"New Matlab directory = "<<s;
    emit MLEvalString(s);
}

void MainWindow::UpdateStatusBar(QString message)
{
    ui->StatusLabel->setText(message);
}

void MainWindow::on_pushButton_clicked()
{
    if (mltimer.isActive())
    {
        mltimer.stop();
        ui->pushButton->setText("Stop");        
        ui->StatusLabel->setText("STOP");
    }
        else
    {
        mltimer.start(((int)(1000/ui->spinBox->value())));
        ui->pushButton->setText("Work");
    }
//    emit runML();
}

void MainWindow::on_pushButton_3_clicked()
{
    remotecontol.hide();
    remotecontol.show();
    remotecontol.TimerStart();
//    focusNextChild();
//    focusNextPrevChild(1);
//    focusWidget();
//    remotecontol.setF
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    qDebug()<<"ML count:"<<(int)(1000/arg1);
    if (mltimer.isActive())
    {
        mltimer.stop();
        mltimer.start(((int)(1000/arg1)));
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    emit MLEvalString("Modul_INI();");
    emit MLEvalString("figure(100);");
}
