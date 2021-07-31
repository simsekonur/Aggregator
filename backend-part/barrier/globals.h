#ifndef GLOBALS_H
#define GLOBALS_H
#include <QMutex>
#include <QWaitCondition>
extern QMutex *g_mutex;
extern QWaitCondition *g_cond_var;
#endif // GLOBALS_H
