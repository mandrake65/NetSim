#ifndef _DATA_FRAME_H_
#define _DATA_FRAME_H_

#include <ostream>
#include <chrono>
#include "Globals.h"

using namespace std;

class DataFrame {
public:
    int len;
    byte* data;
    std::chrono::milliseconds duration;

    DataFrame(int len =0, byte* data=nullptr) {
        this->len = len;
        this->data = new byte[len];
        if( data != nullptr)
        memcpy(this->data, data, len);
    }
    DataFrame(const DataFrame& elem) {
        this->len = elem.len;
        this->data = new byte[len];
        memcpy(this->data, elem.data, elem.len);
    }

    ~DataFrame() {
        delete data;
    }
};

#define ACK  0x55
#define NACK 0x66

typedef unsigned short int MacAddr_t;

class ACKFrame : public DataFrame {
public:
    ACKFrame() {
        len = 1;
        data = new byte[1];
        data[0] = ACK;
    }
};
struct NACKFrame : public DataFrame {

    NACKFrame() {
        len = 1;
        data = new byte[1];
        data[0] = NACK;
    }
};
#endif // !_DATA_FRAME_H_