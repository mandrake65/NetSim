#ifndef _COMM_OBJECT_H_
#define _COMM_OBJECT_H_
#include "ConnectedObject.h"
#include "ActiveObject.h"


class CommObject : public virtual ConnectedObject, public virtual ActiveObject {

public:
	virtual void process(int sleepTime) = 0;
};

#endif // !_COMM_OBJECT_H_
