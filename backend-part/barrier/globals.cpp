#include "globals.h"

QMutex* g_mutex = new QMutex();
QWaitCondition* g_cond_var= new QWaitCondition();
