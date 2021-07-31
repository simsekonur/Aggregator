#include "forest.h"

using namespace std;
Forest::Forest(QObject *parent):QObject(parent)
{
    level = 0;
}

Forest::Forest(int level, std::vector<Tree>& treesToBeCalculated,QObject *parent):QObject(parent)
{
    this->level = level;
    trees = treesToBeCalculated;
    this->ct = new ControllerThread(treesToBeCalculated.size());
    connect(ct,&ControllerThread::levelIsDone,this,&Forest::leveldone);
    mutex = new QMutex();
    condition = new QWaitCondition();

}
Forest::~Forest(){
    delete ct;
    delete mutex;
    delete condition;
}
void Forest::calculate()
{
    vector<TreeThread*> treethreads;
    for(int i=0;i<trees.size();i++){
        treethreads.push_back(new TreeThread(trees[i], ct));
    }
    for(TreeThread* t: treethreads){
        QObject::connect(t, &QThread::started, this, &Forest::canStart);
        QObject::connect(t, &QThread::finished, this, &Forest::processEnded);
        t->start();
    }
//    emit leveldone();

    /*for(int i=0;i<level;i++){

        threading will be added.
        for(Tree x: trees)
            x.calculateLevel(i);
    }*/
}

void Forest::processEnded(){
    static int endNum = trees.size();
    QMutex mutexForEndNumber;
    mutexForEndNumber.lock();
    endNum--;
    mutexForEndNumber.unlock();
    if(endNum==0){
        qDebug() << "this is the end";
        QCoreApplication::quit();
    }

}

std::vector<Tree> Forest::getTrees()
{
    return trees;
}

void Forest::setTrees(std::vector<Tree> &trees)
{
    this->trees = trees;
}

void Forest::leveldone()
{
    condition->wakeAll();
}

void Forest::canStart(){
    static int startNum = trees.size();
    QMutex mutexForStartNumber;
    mutexForStartNumber.lock();
    startNum--;
    mutexForStartNumber.unlock();
    if(startNum==0){
        emit leveldone();
    }
}
