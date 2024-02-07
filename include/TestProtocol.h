#ifndef _TEST_PROTOCOL_H_
#define _TEST_PROTOCOL_H_
#include "ConnectedObject.h"
#include <map>
typedef std::vector< MacAddr_t> MacAddrList_t;
ostream& operator<<(ostream& os, const MacAddrList_t& table);


class ForewardingTable {
    std::map<MacAddr_t, MacAddrList_t> ftable;


    friend ostream& operator<<(ostream& os, const ForewardingTable& table)
    {
        os << "F_TABLE[";
        for (const auto& p : table.ftable)
        {
            std::cout << endl << p.first << "<-";// << p.second << std::endl;
            for (const auto& node : p.second) {
                os << node << ",";
            }            
        }
        os<<    "]";
        return os;
    }

public:
    void addF(MacAddr_t dest, MacAddr_t fnode) 
    {
        auto search = ftable.find(dest);

        if ( search == ftable.end()) {
            MacAddrList_t nodes;
            nodes.push_back(fnode);
            ftable.emplace(dest, nodes);
        }
        else {
            MacAddrList_t& tab = search->second;
            auto addrIt = find(tab.begin(), tab.end(), fnode);
            if( addrIt == tab.end()){
                search->second.push_back(fnode);
            }
        }
    }
    void removeF(MacAddr_t dest, MacAddr_t fnode) {
        // remove a foreward node to dest
        auto search = ftable.find(dest);

        if (search != ftable.end()) 
        {
            MacAddrList_t& tab = search->second;
            for (std::vector<MacAddr_t>::iterator it = tab.begin(); it != tab.end(); it++)
            {
                if( *it == fnode ){
                    tab.erase(it);
                    break;
                }
            }
            
        }        
    }
};

class TestProtocol : public  ProtocolStackElement {

    SimpleConnectedObject* node;
    ForewardingTable ft;
    MacAddrList_t neighborhood;

    friend ostream& operator<<(ostream& os, const TestProtocol& protocol)
    {
        os << "TestProtoco{\n";
        os << "nodeId: " << protocol.node->getId() << endl;
        os << protocol.ft << endl;
        os << protocol.neighborhood << endl;
        os << "}";
        return os;
    }


public:
    TestProtocol(SimpleConnectedObject* node = nullptr);
    int receiveFromUp(DataFrame* frame) { return 0; };
    int sendToUp(DataFrame* frame) { return 0; };

    int receiveFromDown(DataFrame* frame);
    int sendToDown(DataFrame* frame);


    void addNeighborhood(MacAddr_t addr) {
        auto search = find(neighborhood.begin(), neighborhood.end(), addr);
        if (search == neighborhood.end())
            neighborhood.push_back(addr);
    }

    void removeNeighborhood(MacAddr_t addr) {
        auto search = find(neighborhood.begin(), neighborhood.end(), addr);
        if (search != neighborhood.end())
            neighborhood.erase(search);
    }
};
#define TEST_PROTOCOL   7

#define TEST_PROTOCOL_REQ_DISC   'R'
#define TEST_PROTOCOL_RESP_DISC  'r'
#define ADDR_NOBODY    0x00
#define ADDR_BROADCAST 0xFF


/* TestProtocol frame definition
0 prrotocol
1-2 local source
3-4 local destination
5-6 network source
7-8 network destination
9 Time to live TTL
10 request type
11..15 data
*/
class TestFrame : public DataFrame {
    friend ostream& operator<<(ostream& os, const TestFrame& frame)
    {
        os << "F[" << frame.getLocalSrcNode() << "," << frame.getLocalDestNode() << ",";
        os << frame.getSrcNode() << "," << frame.getDestNode() << "](type: " << frame.data[10] <<", ttl="<< frame.getTTL()<< ")";
        return os;
    }
public:
    TestFrame(MacAddr_t src, MacAddr_t dest, byte ttl = 16) : DataFrame(len = 16)
    {        
        data = new byte[len];
        // next node
        MacAddr_t localSrcNode = 0;
        MacAddr_t localDestNode = 0;
        int i = 0;
        data[i++] = TEST_PROTOCOL;
        //next node
        data[i++] = (localSrcNode >> 8) & 0xFF;
        data[i++] = (localSrcNode) & 0xFF;

        data[i++] = (localDestNode >> 8) & 0xFF;
        data[i++] = (localDestNode) & 0xFF;

        //source
        data[i++] = (src >> 8) & 0xFF;
        data[i++] = (src) & 0xFF;
        //dest
        data[i++] = (dest >> 8) & 0xFF;
        data[i++] = (dest) & 0xFF;

        data[i++] = ttl; // [9] TTL Time to live
        data[i++] = 0; //type
        memcpy(&data[i], "ABCD", 4);
        cout << *this << endl;
    }
    MacAddr_t getLocalSrcNode() const
    {
        MacAddr_t addr;
        addr = (data[1] << 8) | (data[2]);
        return addr;
    }
    MacAddr_t setLocalSrcNode(MacAddr_t addr) {
        data[1] = (addr << 8);
        data[2] = (addr & 0xFF);
        return addr;
    }
    MacAddr_t getLocalDestNode() const
    {
        MacAddr_t addr;
        addr = (data[3] << 8) | (data[4]);
        return addr;
    }
    MacAddr_t setLocalDestNode(MacAddr_t addr) {
        data[3] = (addr << 8);
        data[4] = (addr & 0xFF);
        return addr;
    }
    MacAddr_t getSrcNode() const
    {
        MacAddr_t addr;
        addr = (data[5] << 8) | (data[6]);
        return addr;
    }
    MacAddr_t getDestNode() const
    {
        MacAddr_t addr;
        addr = (data[7] << 8) | (data[8]);
        return addr;
    }
    unsigned short int getTTL() const { return data[9];  }
    void setTTL(byte ttl) { data[9] = ttl; }
    void decrementTTL() { 
        if(data[9]>0){
            data[9] = data[9]-1;
        }
    }
    byte getReqType() const  {
        return data[10];
    }
    void setReqType(byte t) {
        data[10] = t;
    }
};


#endif //! _TEST_PROTOCOL_H_