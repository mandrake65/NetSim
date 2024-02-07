#ifndef _TRANSMISSION_CONTROL_H_
#define _TRANSMISSION_CONTROL_H_

#include "Globals.h"
#include "DataFrame.h"
#include "DataSender.h"
#include "DataReceiver.h"
#include "ActiveObject.h"
#include "Channel.h"
#include <vector>
#include <map>
#include "DataSlot.h"

class TrasmissionControl :    public virtual ActiveObject 
{
    int wsize;
    int num_retry;
    CounterId_t id_counter;
    std::vector<DataSlot> data_slot;
    std::map<CounterId_t, DataSlot*> window;

    DataReceiver* receiverPtr;


    int putData(int len, byte* data);
    void receiveACK(CounterId_t id);
    void receiveNACK(CounterId_t id);

    FullDuplexChannel upCh;
    FullDuplexChannel downCh;

public:
    TrasmissionControl(int window_size, int num_retry);
    void setWindowSize(int size);
    void reset();

    void setDataReceiver(DataReceiver* receiverPtr);
    
    void setUpChannel(FullDuplexChannel fdCh) {
        upCh = fdCh;
    }
    void setDownChannel(FullDuplexChannel fdCh) {
        downCh = fdCh;
    }
    
    int receiveData(int len, byte* data);
    

    void process(int sleepTime);
private:
    int findEmptySlot();

};

#endif  // !_TRANSMISSION_CONTROL_H_
