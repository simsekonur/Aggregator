#ifndef TREEWORKER_H
#define TREEWORKER_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <tree.h>
#include <controllerthread.h>



class TreeThread: public QThread
{
    Q_OBJECT

public:
    TreeThread(Tree tree,ControllerThread* ct, QObject *parent = 0);
private:
    Tree aTree;
    QWaitCondition* condition;
    QMutex* mutex;
    ControllerThread* act;
protected:
    void run() override;
signals:
    void resultReady();
};

#endif // TREEWORKER_H
