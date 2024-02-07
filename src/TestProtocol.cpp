#include "TestProtocol.h"

//************************************************************************
ostream& operator<<(ostream& os, const MacAddrList_t& table)
{
    os << "N_TABLE[";
    for (const auto& p : table)
    {
        std::cout << p << ",";
    }

    os << "]";
    return os;
}

//************************************************************************

TestProtocol::TestProtocol(SimpleConnectedObject* node) {
    this->node = node;
    node->setProtoStack(this);
}
int TestProtocol::receiveFromDown(DataFrame* _frame)
{
    if (_frame->data[0] == TEST_PROTOCOL)
    {
        TestFrame* frame = (TestFrame*)_frame;
        TRACE_CLASS_MSG("ID: " << node->getId() << *frame);    
        /*TODO
        * pacchetto che serve a trovare i vicini: TTL = 1, al quale si risponde con source locale di chi lo ha inviato
        * 
        */

        if (frame->getLocalSrcNode() == node->getId() || frame->getSrcNode() == node->getId()) {
            // discard packe sent fom local node
            TRACE_CLASS_MSG("ID: " << node->getId() << " " << "discard msg" << *frame);
            return 0;
        }        

        switch (frame->getReqType()) {
        case TEST_PROTOCOL_REQ_DISC: {
            if (frame->getDestNode() == ADDR_NOBODY) {
                TestFrame respFrame(node->getId(), frame->getSrcNode(), 1);
                respFrame.setReqType(TEST_PROTOCOL_RESP_DISC);
                sendToDown(&respFrame);
            }
            return 0;
        }
            break;
        case TEST_PROTOCOL_RESP_DISC: // risposta a una richiesta di discovery dei vicini, seve ad 
            addNeighborhood(frame->getSrcNode());
            return 0;
        }


        if(frame->getLocalSrcNode() != node->getId()){
            ft.addF(frame->getSrcNode(), frame->getLocalSrcNode());
        }

       
        if (frame->getDestNode() == node->getId()) {
            TRACE_CLASS_MSG("ID: " << node->getId() << " " << "OK to Destination" << *frame);
        }
        else
        if (frame->getLocalSrcNode() != node->getId() 
            || frame->getDestNode() == ADDR_BROADCAST ) 
        {
            if( frame->getTTL() > 0){
                frame->decrementTTL();
                TRACE_CLASS_MSG("ID: " << node->getId() << " " << "Foreward " << *frame);                
                sendToDown(frame);
            }
            else {
                TRACE_CLASS_MSG("ID: " << node->getId() << " " << "Dropped TTL exceded " << *frame);
            }
        }
    }
    return 0;
}
int TestProtocol::sendToDown(DataFrame* _frame)
{
    if (node != nullptr)
    {
        //TRACE_CLASS_MSG("ID:" << node->getId());
        MacID_t id = node->getId();
        TestFrame* frame = (TestFrame*)_frame;
        MacID_t destNode = frame->getDestNode();
        if (id != destNode)
        {
            frame->setLocalSrcNode(id);
            //TODO:             frame->setLocalDestNode();
            node->send(frame);
        }
    }
    return OK;
}