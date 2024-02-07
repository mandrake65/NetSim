#ifndef _ACTIVE_OBJECT_H_
#define _ACTIVE_OBJECT_H_
#include <thread>
class ActiveObject {
    std::thread* threadPtr;
public:

    bool reqToEnd;

    virtual void process(int sleepTime)=0;
    void run(int sleepTime);
    void end();
};
#endif