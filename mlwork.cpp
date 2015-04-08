#include "MLwork.h"
#include <iostream>
#include <fstream>
//#include <TCHAR.H>
#include <windows.h>
//#include <process.h>

using namespace std;

int initConfig(RCConfig *config){
    ifstream configFile;
    string line;

    configFile.open("LARCmaCS.cnf", ios::in);

    if(!configFile.is_open()) {
        cerr << "Can't open configuration file" << endl;
        return -1;
    }

    if(!config) {
        cerr << "Can't parse configuration file" << endl;
        return -2;
    }

    while(!configFile.eof()){
        getline(configFile, line);

        if(line.empty())
            continue;

        if(line.find("rfclient.name") != string::npos) {
            config->name = line.substr(line.find('=') + 1);

            if(config->name.size() > 16) {
                cerr << "[WARNING] rfclient.name value is too long." << endl << "It will be truncated up to 16 characters." << endl;
                config->name.resize(16);
            }
        }

        if(line.find("rfclient.file_of_matlab") != string::npos) {
            string fom = line.substr(line.find('=') + 1);

            if(fom.size() > 16) {
                cerr << "[WARNING] rfclient.file_of_matlab value is too long." << endl << "It will be truncated up to 16 characters." << endl;
                fom.resize(16);
            }
            char *str = new char[fom.length() + 1];
            strcpy(str, fom.c_str());
            config->file_of_matlab = str;
        }

        if(line.find("rfclient.RULE_AMOUNT") != string::npos){
            config->RULE_AMOUNT = atoi(line.substr(line.find('=') + 1).c_str());
        }

        if(line.find("rfclient.RULE_LENGTH") != string::npos){
            config->RULE_LENGTH = atoi(line.substr(line.find('=') + 1).c_str());
        }

        if(line.find("rfclient.BACK_AMOUNT") != string::npos){
            config->BACK_AMOUNT = atoi(line.substr(line.find('=') + 1).c_str());
        }

        if(line.find("rfclient.BACK_LENGTH") != string::npos){
            config->BACK_LENGTH = atoi(line.substr(line.find('=') + 1).c_str());
        }
    }

    configFile.close();

    return 0;
}

void MainAlgWorker::init(){
    RCConfig rcconfig;

    cout << "Initialization config file..." << endl;

    if(!initConfig(&rcconfig)) {
        cout << rcconfig.file_of_matlab << endl;
        cout << rcconfig.name << endl;
        cout << rcconfig.BACK_AMOUNT << endl;
        cout << rcconfig.BACK_LENGTH << endl;
        cout << rcconfig.RULE_AMOUNT << endl;
        cout << rcconfig.RULE_LENGTH << endl;

        cout << "...successful" << endl;
    }
    else {
        cerr << "...bad" << endl;
        char *str = new char[16];
        str = "Robofootball";
        rcconfig.name = str;
        str = "main";
        rcconfig.file_of_matlab=str;
        rcconfig.RULE_AMOUNT=5;
        rcconfig.RULE_LENGTH=5;
        rcconfig.BACK_AMOUNT=10;
        rcconfig.BACK_LENGTH=8;

    }

    MlData ptl(rcconfig);
    fmldata = ptl;

    MlData mtl(rcconfig);
    fmldata = mtl;

    run_matlab();
}

void MainAlgWorker::run_matlab()
{
    cout << "Work is started..." << endl;

    if (!(fmldata.ep = engOpen(NULL))) {
        cerr << "Can't open Matlab Engine" << endl;
        fmtlab = false;
        return;
    }

    m_buffer[255] = '\0';
    engOutputBuffer(fmldata.ep, m_buffer, 255);
    printf("Matlab Engine is opened\n");

    // Path_Of_Files
   // TCHAR CurrentPath[MAX_PATH];
   // char StringWithPath[MAX_PATH],StringPath[MAX_PATH];

    /*
    // Detecting Directory
    printf("Start to detect directory\n");
    GetCurrentDirectory(sizeof(CurrentPath), CurrentPath);

#ifdef UNICODE
    wcstombs(StringPath, CurrentPath, MAX_PATH - 1);
#else
    CharToOem(StringPath, CurrentPath);
#endif

    sprintf(StringWithPath, "cd %s;", StringPath);
    engEvalString(fmldata.ep, StringWithPath);
    printf("We are on %s\n",StringWithPath);
*/
    //-----create Rules-----
    char sendString[256];
    sprintf (sendString, "Rules=zeros(%d, %d)", fmldata.config.RULE_AMOUNT, fmldata.config.RULE_LENGTH);
    engEvalString(fmldata.ep, sendString);
//    engEvalString(fmldata.ep, "disp(1)");

    fmtlab = true;
}

void MainAlgWorker::stop_matlab()
{
    fmtlab = false;
}
#include <qdebug.h>
/*void MainAlgWorker::ChangeDirrectory(QString dir)
{
    char StringWithPath[MAX_PATH];
    sprintf(StringWithPath, "cd %s;", dir.toUtf8().data());
    engEvalString(fmldata.ep, StringWithPath);
    printf("Yes directory %s\n",StringWithPath);
}*/

void MainAlgWorker::Pause()
{
    engEvalString(fmldata.ep, "PAUSE();");
    //qDebug()<<"ML : PAUSE";
}

void MainAlgWorker::EvalString(QString s)
{
    engEvalString(fmldata.ep,s.toUtf8().data());
}
/*
void MainAlgWorker::addEvalString(QString s)
{
    saveEvalString+=s+"\n";
}*/

#include <QTime>
void MainAlgWorker::run()
{
    QTime qtimer;
    qtimer.start();
    Time_count++;
    timer = clock();
//    if(!shutdowncomp && fmtlab)
//        cout << "Packet is received!" << endl;

    engEvalString(fmldata.ep, "MOD_All();");
    engEvalString(fmldata.ep, fmldata.config.file_of_matlab);

    clock_t timer_c=clock()-timer;
    printf("%d ",qtimer.elapsed());

    if (timer_c>timer_max)
        timer_max=timer_c;
    timer_s=timer_s+timer_c;
    if (clock()-timer_m>CLOCKS_PER_SEC)
    {
        printf("\n---------------------------------\n");

        timer_m=clock();
        QString temp;
        QString ToStatus="Using Matlab: Count=";
        temp.setNum(Time_count);
        ToStatus=ToStatus+temp;

        ToStatus=ToStatus+" ~time=";
        temp.setNum(timer_s/Time_count);
        ToStatus=ToStatus+temp;

        ToStatus=ToStatus+" maxtime=";
        temp.setNum(timer_max);
        ToStatus=ToStatus+temp;

        ToStatus=ToStatus+" fulltime=";
        temp.setNum(timer_s);
        ToStatus=ToStatus+temp;

        timer_s=0;
        timer_max=0;
        Time_count=0;
        emit StatusMessage(ToStatus);

        engEvalString(fmldata.ep,"ispause=RP.Pause");
        mxArray *mxitpause=engGetVariable(fmldata.ep,"ispause");
        if (mxitpause!=0)
        {
            double *itpause=mxGetPr(mxitpause);
            if (itpause!=0)
            {              
                if ((*itpause)==0)
                {
                    mxArray *mxzMain_End=engGetVariable(fmldata.ep,"zMain_End");
                    if (mxzMain_End!=0)
                    {
                        double *zMain_End=mxGetPr(mxzMain_End);
                        if (zMain_End!=0)
                        {
                            if ((*zMain_End)==0)
                                emit UpdatePauseState("main br");
                            else emit UpdatePauseState("WORK");
                        }
                        else emit UpdatePauseState("-err-z");
                    }
                    else emit UpdatePauseState("-err-mz");
                }
                else emit UpdatePauseState("PAUSE");
            }
            else emit UpdatePauseState("-err-p"); //Ответ от матлаба сломан
        }
        else emit UpdatePauseState("-err-mp"); //Нет ответа от матлаб
    }
    emit mainAlgFree();
}
