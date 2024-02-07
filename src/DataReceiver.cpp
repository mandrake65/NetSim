#include "Globals.h"
#include "DataReceiver.h"
#include "DataSender.h"
#include "Channel.h"

int DataReceiver::receiveData(int len, byte* data) 
{
    TRACE_CLASS();
    if (txChPtr) {
        ACKFrame frame;
        txChPtr->receiveData(frame);
    }
    return OK;
}

void DataReceiver::process(int sleepTime)
{
    TRACE_CLASS();
    std::chrono::milliseconds val(sleepTime);
    while (!reqToEnd) {
        std::this_thread::sleep_for(val);
        TRACE_CLASS();
    }
}

int DataReceiver::send(DataFrame* Frame) { //TODO:
    TRACE_CLASS();
    return 0;
}
int DataReceiver::receive(DataFrame* Frame) { //TODO:
    TRACE_CLASS();
    return 0;
}