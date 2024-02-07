#include "ConnectedObject.h"
#include "Channel.h"
#include "DataFrame.h"

int ConnectedObject::send(DataFrame* frame) // invia dati su TX
{
    TRACE_CLASS();
    if (txChPtr) {
        txChPtr->receiveData(*frame); // mette i dati nel canale
    }
    return 0;
}
int ConnectedObject::receive(DataFrame* frame)  // riceve dati, non fa nulla
{
    TRACE_CLASS();
    return 0;
}

void ConnectedObject::setChannels(NetSim::FullDuplexChannel* fdCh)
{
    TRACE_CLASS();
    this->rxChPtr = fdCh->rxCh;
    this->txChPtr = fdCh->txCh;
}

//************************************************************************

static int id_cnt;
SimpleConnectedObject::SimpleConnectedObject(Channel* ch)
{ 
    id = ++id_cnt;
    if (ch != nullptr) {
        channels.push_back(ch);
        ch->addReceiver(this);
    }

}
void SimpleConnectedObject::setId(MacID_t id) {
    this->id = id;
}
void SimpleConnectedObject::addToCh(Channel* chPtr) {

    if (std::find(channels.begin(), channels.end(), chPtr) == channels.end()) {
        channels.push_back(chPtr);
        chPtr->addReceiver(this);
    }

}
int SimpleConnectedObject::removeFromCh(Channel* chPtr) {

    for ( auto it = channels.begin(); it != channels.end();)
    {
        if (*it == chPtr) {
            (*it)->removeReceiver(this);
            it = channels.erase(it);
            return OK;
        }
        else
            ++it;
    }
    return NOT_FOUND;
}
int SimpleConnectedObject::send(DataFrame* frame) // invia dati sul canale
{
    //TRACE_CLASS_MSG("ID:"<<id);
    for (int i = 0; i < channels.size(); i++)
    {
        channels[i]->receiveData(*frame);
    }
    return OK;
}
int SimpleConnectedObject::receive(Channel* chPtr, DataFrame* frame) // riceve dati dal canale
{
    //TRACE_CLASS_MSG("ID:" << id <<  "From ch:" << chPtr->getId());
    std::lock_guard<std::mutex> guard(queue_mutex);
    this->dataQueue.emplace(*frame);
    return OK;
}


void SimpleConnectedObject::process(int sleepTime)
{
    std::chrono::milliseconds val(sleepTime);
    while (!reqToEnd) {
        if (this->protocolStack) 
        {
            if( !dataQueue.empty() ){
                protocolStack->receiveFromDown(&dataQueue.front());
                dataQueue.pop();
            }
        }
        std::this_thread::sleep_for(val);
        //TRACE_CLASS();
    }
    
}

