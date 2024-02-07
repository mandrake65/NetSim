#ifndef _CONNECTED_OBJECT_H_
#define _CONNECTED_OBJECT_H_
#include "Globals.h"
#include <vector>
#include <algorithm>
#include <queue>
#include <mutex>
#include "DataFrame.h"
#include "ActiveObject.h"

class Channel;
class DataFrame;
namespace NetSim {
    class FullDuplexChannel;
}

class ProtocolStackElement;

typedef short int MacID_t;
class SimpleConnectedObject : public ActiveObject {
    MacID_t id;
    ProtocolStackElement* protocolStack;
    std::queue<DataFrame> dataQueue;
    std::mutex queue_mutex;
protected:
    std::vector<Channel*> channels;
public:
    SimpleConnectedObject(Channel* ch = nullptr);
    void setId(MacID_t id);
    MacID_t getId(void) {
        return id;
    }
    void setProtoStack(ProtocolStackElement* stack) {
        protocolStack = stack;
        
    }
    void addToCh(Channel* chPtr);
    int removeFromCh(Channel* chPtr);
    virtual int send(DataFrame* frame); // invia dati sul canale    
    virtual int receive(Channel* chPtr, DataFrame* frame); // riceve dati dal canale   

    void process(int sleepTime);

};

class ConnectedObject {
protected:
    Channel* txChPtr;
    Channel* rxChPtr;
public:
    ConnectedObject( Channel* tx = nullptr, Channel* rx = nullptr ) {
        txChPtr = tx;
        rxChPtr = rx;
    }
    //ConnectedObject(NetSim::FullDuplexChannel* fdCh);
    void setChannels(NetSim::FullDuplexChannel* fdCh);
    void setTxCh(Channel* chPtr) {
        txChPtr = chPtr;
    }
    void setRxCh(Channel* chPtr) {
        rxChPtr = chPtr;
    }
    
    virtual int send(DataFrame* frame);
    virtual int receive(DataFrame* frame);
};

class ProtocolStackElement {
protected:
    ProtocolStackElement* upConnection;
    ProtocolStackElement* downConnection;
public:
    ProtocolStackElement() {
        upConnection = nullptr;
        downConnection = nullptr;
    }

    int connectDownTo(ProtocolStackElement* elem); // collega l'elemento con un elemento sottostanre
    int connectUpo(ProtocolStackElement* elem); // collega l'element con un'altro sovrastante

    int insertBetween(ProtocolStackElement* upElem, ProtocolStackElement* downElem); // inserisce tra due elementi

    virtual int receiveFromUp(DataFrame* frame) = 0;
    virtual int sendToUp(DataFrame* frame) = 0;

    virtual int receiveFromDown(DataFrame* frame) = 0;
    virtual int sendToDown(DataFrame* frame) = 0;
};

#endif // !_CONNECTED_OBJECT_H_