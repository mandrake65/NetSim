#include "Channel.h"

//******************************************************************************
static int channel_id_cnt;
Channel::Channel() {
    this->chId = ++channel_id_cnt;
    TRACE_CLASS_MSG("CH_ID:" << chId);
}
void Channel::addReceiver(SimpleConnectedObject* receiver) // add a subscriber
{
    std::lock_guard<std::mutex> guard(ch_mutex);
    TRACE_CLASS_MSG("CH_ID:"<<this->chId<<" ID: "<< receiver->getId());
    receivers.push_back(receiver);
}
void Channel::removeReceiver(SimpleConnectedObject* receiver) // remove a subscriber
{
    std::lock_guard<std::mutex> guard(ch_mutex);
    TRACE_CLASS();
    receivers.remove(receiver);
}

void Channel::receiveData(DataFrame frame) // get data to deliver to subscribers
{
    std::lock_guard<std::mutex> guard(ch_mutex);
    //TRACE_CLASS_MSG("CH_ID:"<< chId);
    //dataQueue.push(frame); 
    dataQueue.emplace(frame); //emplace_back
    
    
}
void Channel::sendData() // sen data to subscriber
{
    std::lock_guard<std::mutex> guard(ch_mutex);
    if(!dataQueue.empty())
    {
        //TRACE_CLASS_MSG("CH_ID:"<<chId);
        for (auto iter = receivers.begin(); iter != receivers.end(); ++iter)
        {
            DataFrame& framePtr = dataQueue.front();

            // qui il canale puo introdurre errori, droppare un messaggio, alterare i dati ....

            (*iter)->receive(this, &framePtr);
        }
        dataQueue.pop();
    }
}

void Channel::reset() 
{
    std::lock_guard<std::mutex> guard(ch_mutex);
    TRACE_CLASS();
    while (!dataQueue.empty())
        dataQueue.pop();

    receivers.clear();
}

void Channel::process(int sleepTime)
{
    std::chrono::milliseconds val(sleepTime);
    while (!reqToEnd) {
        //TRACE_CLASS();
        sendData();
        std::this_thread::sleep_for(val);
    }
}




