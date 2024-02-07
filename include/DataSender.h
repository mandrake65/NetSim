
#include "Globals.h"
#include "ActiveObject.h"
#include "DataFrame.h"
#include "ConnectedObject.h"
#include "FullDuplexChannel.h"
class DataReceiver;
class Channel;
using namespace NetSim;

class DataSender : public virtual ActiveObject, public virtual ConnectedObject {
  
    virtual void receiveACK(CounterId_t id);
    virtual void receiveNACK(CounterId_t id);
public:
    virtual int senDataTo(DataFrame& frame, Channel* ch = nullptr);
    virtual int senDataTo(int len, byte* data, DataReceiver* receiver = nullptr);
    
    virtual void process(int sleepTime);
    virtual int send(DataFrame* Frame);
    virtual int receive(DataFrame* Frame);
};

class TestDataSender : public DataSender {

public:
    void process(int sleepTime);
};