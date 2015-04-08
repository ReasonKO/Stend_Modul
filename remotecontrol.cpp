#include "remotecontrol.h"
#include "ui_remotecontrol.h"

#define RC_TIMER_CD 10

#include <QDebug>
RemoteControl::RemoteControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteControl)
{
    ui->setupUi(this);
    qDebug()<<"START REMOTE CONTROL";
    connect(&timer,SIGNAL(timeout()),this,SLOT(RC_send()));
    connect(this,SIGNAL(destroyed()),this,SLOT(TimerStop()));
//    connect(this,SIGNAL()
    for(int i=0; i<256; i++)
        keys[i]=0;
    key_shift=0;

}

void RemoteControl::TimerStart()
{
    if (!timer.isActive())
    {
        qDebug()<<"<RemContril>: Start";
        timer.start(RC_TIMER_CD);
    }
}
void RemoteControl::TimerStop()
{
    if (timer.isActive())
    {
        qDebug()<<"<RemContril>: Stop";
        timer.stop();
    }
}

RemoteControl::~RemoteControl()
{
    qDebug()<<"destroyRM";
    delete ui;
}

void RemoteControl::keyPressEvent(QKeyEvent * key)
{
    if (key->key()<256)
        keys[key->key()]=1;
    if (key->key()==Qt::Key_Escape)
    {
        this->close();
    }
    if (key->key()==Qt::Key_Shift)
    {
        key_shift=1;
    }
}

void RemoteControl::keyReleaseEvent(QKeyEvent * key)
{
    int nkey=key->key();
    if (nkey<256)
        keys[nkey]=0;
    if (nkey==Qt::Key_Shift)
    {
        key_shift=0;
    }
}


void RemoteControl::RC_send(void)
{
    int L=0,R=0,S=0,K=0,B=0;
    if (keys['W'] )
    {
        L+=50;
        R+=50;
    }
    if (keys['S'])
    {
        L-=50;
        R-=50;
    }
    if (keys['A'])
    {
        L-=25;
        R+=25;
    }
    if (keys['D'])
    {
        L+=25;
        R-=25;
    }
    if (keys[' ']||keys['Q'])
        K=1;
    if (keys['E'])
        K=-1;
    if (keys['Z']||keys['B'])
        B=1;
    if (key_shift)
    {
        L=L*2;
        R=R*2;
        if (abs(L)>100) L=L/abs(L)*100;
        if (abs(R)>100) R=R/abs(R)*100;
    }
    ui->label_state->setText(" L=" +QString::number(L)+
                             " R=" +QString::number(R)+
                             " K=" +QString::number(K)+
                             " B=" +QString::number(B));
    //qDebug()<<L<<" "<<R<<" "<<K<<P<<S;
    emit RC_control(L,R,K,B);
}

void RemoteControl::closeEvent(QCloseEvent *)
{
    TimerStop();
}
