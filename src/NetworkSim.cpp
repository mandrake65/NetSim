// NetworSim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <queue>
#include <list>
#include <algorithm>
#include "ActiveObject.h"
#include "DataFrame.h"
#include "Channel.h"
#include "TransmissionControl.h"
#include "CommObject.h"

#include "TestCommClass.h"
#include "TestProtocol.h"

using namespace std;
using namespace std::chrono_literals;


int connectCommObject( CommObject* node_A, CommObject* node_B, FullDuplexChannel fdCh)
{
    node_A->setTxCh(fdCh.txCh);
    node_A->setRxCh(fdCh.rxCh);
    node_B->setRxCh(fdCh.txCh);
    node_B->setTxCh(fdCh.rxCh);
    //fdCh.txCh->addReceiver(node_B);
    //fdCh.rxCh->addReceiver(node_A);
    return 0;
}

int main()
{
    std::cout << "TestDataSender->(Ch: one)->DataReceiver\n";
    std::cout << "DataReceiver->(Ch: two)->TestDataSender\n";
    const auto start = std::chrono::high_resolution_clock::now();

    //int test(void);
    //test();
    // CHANNELS *************************************
    Channel one;
    Channel two;
    Channel tree;

    // NODES ***************************************
    SimpleConnectedObject A(&one);
    SimpleConnectedObject B(&one);
    SimpleConnectedObject C(&two); 
    SimpleConnectedObject D(&two);
    SimpleConnectedObject E(&tree);

    // ATTACH PTOTOCOLS *********************************
    vector<TestProtocol*> stacks;
    stacks.push_back(new TestProtocol(&A));
    stacks.push_back(new TestProtocol(&B));
    stacks.push_back(new TestProtocol(&C));
    stacks.push_back(new TestProtocol(&D));
    stacks.push_back(new TestProtocol(&E));
    // CONNECT NODE TO CHANNELS ************************
    B.addToCh(&tree);
    C.addToCh(&tree);

    
    // START CHANNELS **********************************
    one.run(100);
    two.run(100);
    tree.run(100);
    // START NODES **************************************
    A.run(100);
    B.run(100);
    C.run(100);
    D.run(100);
    E.run(100);

    // SEND FIRAME ************************************
    for (int i = 0; i < 5; i++) {
        TestFrame frame(i+1, 0);
        frame.setReqType(TEST_PROTOCOL_REQ_DISC);
        stacks[i]->sendToDown(&frame);
    }
    //*************************************************
    std::chrono::milliseconds val(1000);
    while (1) {
        for (int i = 0; i < stacks.size(); i++) {
            TRACE_MSG(*(TestProtocol*)stacks[i] );
        }
        std::this_thread::sleep_for(val);
    }
    return 0;
    /*
    <graph>   <nodes>   </nodes>    <edges>    </edges> </graph> 
    */


    /*DataReceiver receiver;
    TestDataSender sender;
    TrasmissionControl tc(4,1);
    nder.setRxCh(&two);
    sender.setTxCh(&one);
    receiver.setRxCh(&one);
    receiver.setTxCh(&two);

    one.addReceiver(&receiver);
    //two.addReceiver(&sendseer);
    sender.run(1000);
    */
    
    TestCommProducer NodeA;
    TestCommConsumer NodeB;
    TestCommProducer NodeC;
    TestCommConsumer NodeD;

    FullDuplexChannel A_to_B(&one, &two);
#if 0
    one.addReceiver(&NodeB);
    

    FullDuplexChannel B_to_A(&two, &one);
    two.addReceiver(&NodeA);

    NodeA.setChannels(&A_to_B);
    NodeB.setChannels(&B_to_A);
#else
    connectCommObject(&NodeA, &NodeB, A_to_B);
#endif
    //TODO FUNZIONE che connect( NodeA, NodeB, fdCh );
    /*
    connect( NodeA, TP_Tx ); // coppia di canali di default/affidabili
    connect(TP_Tx, TP_Rx, fdChErrors);
    connect(TP_Rx, NodeB);
    */

    NodeA.run(500);
    //NodeB.run(500);

    while (true) {
        std::this_thread::sleep_for(1000ms);
    }
#if 0
    //reciver.run(500);
    tc.run(100);
    unsigned int data_gen = 0;
    byte data[8];
    for (int i = 0; i < 100; i++) 
    {        
        for (int i = 0; i < 8; i++)
            data[i] = data_gen++;
        DataFrame frame(8, data);

        tx.addReceiver(&receiver);
        tx.receiveData(frame);
        tx.sendData();
        while (sender.senDataTo(frame, &tx) != OK) {
            cout << "retry...\n";
            std::this_thread::sleep_for(500ms);
        }        
        std::this_thread::sleep_for(1000ms);       
    }
#endif
}

