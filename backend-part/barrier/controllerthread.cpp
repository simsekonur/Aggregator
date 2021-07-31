#include "controllerthread.h"
#include <QCoreApplication>

ControllerThread::ControllerThread(int treeNum, QObject *parent): QObject(parent)
{
    currtreeNum = treeNum;
    realtreeNum = treeNum;
    aMutex = new QMutex();
}

ControllerThread::~ControllerThread(){

}

void ControllerThread::oneTreeLevelIsDone()
{
    aMutex->lock();
    currtreeNum--;
    if(currtreeNum == 0){
        currtreeNum = realtreeNum; //there can be synch issue
        emit levelIsDone();
    }
    aMutex->unlock();
}
