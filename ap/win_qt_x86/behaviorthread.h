#ifndef BEHAVIORTHREAD_H
#define BEHAVIORTHREAD_H

#include <QThread>

class BehaviorThread : public QThread
{
public:
    BehaviorThread();
    // Passo 13: sobreescrever o método run
    void run();
};

#endif // BEHAVIORTHREAD_H
