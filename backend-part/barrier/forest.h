#ifndef FOREST_H
#define FOREST_H

#include "tree.h"
#include "treeThread.h"
#include "controllerthread.h"
#include <vector>
#include <iostream>
#include <QtCore>

class Forest: public QObject
{
    Q_OBJECT
public:
    Forest(QObject *parent = 0);
    Forest(int level,QObject *parent = 0);
    Forest(int level, std::vector<Tree>& treesToBeCalculated,QObject *parent = 0);
    ~Forest();
    void calculate();
    std::vector<Tree> getTrees();
    void setTrees(std::vector<Tree>& trees);

private:
    std::vector<Tree> trees;
    int level;
    ControllerThread* ct;
    QMutex* mutex;
    QWaitCondition* condition;

public slots:
    void leveldone();
    void canStart();
    void processEnded();
};

#endif // FOREST_H
