#include "treeThread.h"


TreeThread::TreeThread(Tree tree,ControllerThread* ct, QObject *parent): QThread(parent)
{
    aTree = tree;
    condition = g_cond_var;
    mutex = g_mutex;
    act = ct;
    connect(this, &TreeThread::resultReady,act,&ControllerThread::oneTreeLevelIsDone);
}


void TreeThread::run(){
    for(int i=0;i<aTree.level();i++){
        mutex->lock();
        condition->wait(mutex);
        aTree.calculateLevel(i);
        emit resultReady();
        mutex->unlock();
    }
}
