
#include "Globals.h"
#include "TestCommClass.h"
#include "DataFrame.h"

void TestCommProducer::process(int sleepTime) {

	int cnt = 0;
	std::chrono::milliseconds val(sleepTime);
	while (!this->reqToEnd) 
	{
		TRACE_CLASS();
		DataFrame frame(8);

		for(int i=0; i< frame.len; i++){
			frame.data[i] = cnt++;
		}
		this->send(&frame);
		std::this_thread::sleep_for(val);
	}
}

int TestCommProducer::receive(DataFrame* frame)
{
	TRACE_CLASS();
	switch (frame->data[0]) {
	case ACK:
		TRACE_CLASS_MSG(" received ACK ");
		break;
	case NACK:
		TRACE_CLASS_MSG(" received NACK ");
		break;
	default:
		TRACE_CLASS_MSG(" received UNKNOW frame ");
	}
	return OK;
}
//**********************************************
// TestCommConsumer
//**********************************************

 
void TestCommConsumer::process(int sleepTime) {
	std::chrono::milliseconds val(sleepTime);
	while (!this->reqToEnd)
	{
		TRACE_CLASS_MSG(" noting to do ");
		
		std::this_thread::sleep_for(val);
	}
}
int TestCommConsumer::receive(DataFrame* frame)
{
	TRACE_CLASS();
	ACKFrame ack;
	TRACE_CLASS_MSG(" send ACK ");
	this->send(&ack);
	return OK;
}