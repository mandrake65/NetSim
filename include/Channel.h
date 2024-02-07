#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "Globals.h"
#include <list>
#include <queue>
#include <mutex>
#include "DataFrame.h"
#include "DataReceiver.h"
class ConnectedObject;

class Channel : public ActiveObject {
    std::mutex ch_mutex;
    int chId;
    std::queue<DataFrame> dataQueue;
    std::list<SimpleConnectedObject*> receivers;
public:
    Channel();
    void addReceiver(SimpleConnectedObject* receiver); // add a subscriber
    void removeReceiver(SimpleConnectedObject* receiver); // remove a subscriber    
    void receiveData(DataFrame frame); // get data to deliver to subscribers    
    void sendData(); // sen data to subscriber
    void reset();


    int getId() { return chId; }
    void process(int sleepTime);
};


#endif // !_CHANNEL_H_