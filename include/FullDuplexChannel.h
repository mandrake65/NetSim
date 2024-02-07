#ifndef _FULL_DUPLEX_CHANNEL_H_
#define _FULL_DUPLEX_CHANNEL_H_
#include "Globals.h"
#include "Channel.h"
namespace NetSim {
    class FullDuplexChannel {
    public:
        Channel* txCh;
        Channel* rxCh;

        FullDuplexChannel() {
            txCh = nullptr;
            rxCh = nullptr;
        }
        FullDuplexChannel(Channel* tx, Channel* rx) {
            txCh = tx;
            rxCh = rx;
        }
    };
};
#endif // ! _FULL_DUPLEX_CHANNEL_H_
