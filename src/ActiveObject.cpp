#include "ActiveObject.h"
#include "mytrace.h"

/*void ActiveObject::process(int sleepTime)
{
    TRACE_CLASS();
}*/
void ActiveObject::run(int sleepTime)
{
    reqToEnd = false;
    threadPtr = new std::thread(&ActiveObject::process, this, sleepTime);
    threadPtr->detach();
}
void ActiveObject::end() {
    TRACE_CLASS();
    reqToEnd = true;
}