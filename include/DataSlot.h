#ifndef _DATA_SLOT_H_
#define _DATA_SLOT_H_
#include "Globals.h"

struct DataSlot {
    CounterId_t id;
    byte data[1024];
    int datalen;
    int numRetry;
    bool retransmit;

    DataSlot() {
        clear();
    }
    void clear() {
        id = 0;
        numRetry = 0;
        retransmit = false;
    }
    void setToRetrasmit(bool r) {
        retransmit = r;
    }
    void setData(CounterId_t id, int len, byte* data) {
        this->id = id;
        memcpy(this->data, data, len);
        numRetry = 0;
        retransmit = false;
        datalen = len;
    }
};
#endif //!_DATA_SLOT_H_