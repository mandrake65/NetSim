#ifndef _DATA_RECEIVE_H_
#define _DATA_RECEIVE_H_

#include "Globals.h"
#include "ActiveObject.h"
#include "ConnectedObject.h"
#include "DataFrame.h"
#include "FullDuplexChannel.h"

class DataSender;
class DataReceiver : public virtual ActiveObject, public virtual ConnectedObject {
    //FullDuplexChannel* upCh;
public:
    DataReceiver(){}
    //void setUpChannel(FullDuplexChannel* fdCh) {
    //    upCh = fdCh;
    //}
    virtual int receiveData(int len, byte* data);
    void process(int sleepTime);

    int send(DataFrame* Frame);
    int receive(DataFrame* Frame);

};
#endif // !_DATA_RECEIVE_H_