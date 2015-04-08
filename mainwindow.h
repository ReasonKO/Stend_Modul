#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "mlwork.h"
#include "remotecontrol.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    RemoteControl remotecontol;
    QTimer mltimer;
        MainAlg mainalg;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
        void on_pushButton_Pause_clicked();

        void on_pushButton_2_clicked();
private slots:
    void UpdateStatusBar(QString message);
    void UpdatePauseState(QString mess);

    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

    void on_spinBox_valueChanged(int arg1);
    void RCmessage(int L,int R,int K,int B);
    void on_pushButton_4_clicked();

signals:
    void runML();
signals:
    void MLEvalString(QString s);
    void MatlabPause();
    void MatlabChangeDirrectory(QString dir);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
