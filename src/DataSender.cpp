#include "Globals.h"
#include "DataSender.h"
#include "DataReceiver.h"
#include "Channel.h"

int DataSender::senDataTo(DataFrame& frame, Channel* ch) {
    if (ch) {
        ch->receiveData(frame);
        return OK;
    }
    return NO_CONNECTION;
}
int DataSender::senDataTo(int len, byte* data, DataReceiver* receiver) {
    if (receiver) {
        return receiver->receiveData(len, data);
    }
    return NO_CONNECTION;
}

void DataSender::process(int sleepTime)
{
    std::chrono::milliseconds val(sleepTime);
    while (!reqToEnd) {
        std::this_thread::sleep_for(val);
        TRACE_CLASS();
    }
}
int DataSender::send(DataFrame* Frame) // TODO:
{
    return 0;
}
int DataSender::receive(DataFrame* Frame) //TODO:
{
    return 0;
}

void DataSender::receiveACK(CounterId_t id)  //TODO:
{

}
void DataSender::receiveNACK(CounterId_t id) //TODO:
{

}
//***************************************************************
void TestDataSender::process(int sleepTime)
{
    static int cnt = 0;
    std::chrono::milliseconds val(sleepTime);
    while (!reqToEnd) {
        if (txChPtr) {
            DataFrame frame(8);
            for (int i = 0; i < frame.len; i++) {
                frame.data[i] = cnt++;
            }

            txChPtr->receiveData(frame);
        }
        std::this_thread::sleep_for(val);
        TRACE_CLASS();
    }
}
