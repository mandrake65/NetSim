#include "TransmissionControl.h"


TrasmissionControl::TrasmissionControl(int window_size, int num_retry) {
    wsize = window_size;
    this->num_retry = num_retry;
    data_slot.resize(window_size);
    receiverPtr = nullptr;
};
void TrasmissionControl::setWindowSize(int size) {
    if (wsize != size) {
        wsize = size;
        data_slot.resize(wsize);
    }
}

void TrasmissionControl::setDataReceiver(DataReceiver* receiverPtr) {

    this->receiverPtr = receiverPtr;
}

void TrasmissionControl::reset() {
    id_counter = 0;
    window.clear();
}
void TrasmissionControl::receiveACK(CounterId_t id) {
    window.erase(id);
}

void TrasmissionControl::receiveNACK(CounterId_t id)
{
    auto search = window.find(id);
    if (search != window.end())
    {
        search->second->retransmit = true;
    }
}

int TrasmissionControl::receiveData(int len, byte* data)
{
    return putData(len, data);
}

int TrasmissionControl::putData(int len, byte* data)
{
    TRACE_CLASS();

    if (window.size() < wsize) {
        int pos = findEmptySlot();
        DataSlot* slotPtr = &data_slot[pos];
        TRACE_CLASS_MSG("slot:" << pos);
        slotPtr->setData(++id_counter, len, data);
        window.insert({ slotPtr->id, slotPtr });
        return OK;
    }

    return NOT_READY;
}

int TrasmissionControl::findEmptySlot()
{
    TRACE_CLASS();
    for (int i = 0; i < data_slot.size(); i++)
    {
        if (data_slot[i].id == 0)
            return i;
    }
    TRACE_CLASS_MSG("NOT_FOUND");
    return NOT_FOUND;
}


void TrasmissionControl::process(int sleepTime)
{
    std::chrono::milliseconds val(sleepTime);
    while (!reqToEnd)
    {
        //deve tener conto dei timeout di ogni messaggio inviato   
        for (auto item : window)
        {
            //TODO:
        }


        std::this_thread::sleep_for(val);
        TRACE_CLASS();
    }
}