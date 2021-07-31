#ifndef CONTROLLERTHREAD_H
#define CONTROLLERTHREAD_H
#include <QMutex>
#include <QThread>


class ControllerThread: public QObject
{
Q_OBJECT
public:
    ControllerThread(int treenum, QObject *parent = 0 );
    ~ControllerThread();

signals:
    void levelIsDone();
public slots:
    void oneTreeLevelIsDone();
private:
    int currtreeNum;
    int realtreeNum;
    QMutex* aMutex;
};

#endif // CONTROLLERTHREAD_H
