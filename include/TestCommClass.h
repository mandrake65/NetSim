#ifndef TEST_COMM_CLASS_H_
#define TEST_COMM_CLASS_H_
#include <queue>
#include "CommObject.h"
#include "ActiveObject.h"
#include "DataFrame.h"



class TestCommProducer : public CommObject
{
public:
	
	void process(int sleepTime);
	int receive(DataFrame* frame);
};

class TestCommConsumer : public CommObject
{
	std::queue<DataFrame> msg;
public:

	void process(int sleepTime);
	int receive(DataFrame* frame);
};
#endif //!TEST_COMM_CLASS_H_
