/*
 * RToFApp.cc
 *
 *  Created on: 17 de dez de 2020
 *      Author: allan
 */

#define BUILDING_DLL
#define OPP_DLLIMPORT

#include <chrono>

#include <iostream>
#include <iomanip>

#include <vector>

#include "inet/applications/base/ApplicationPacket_m.h"
#include "RToFApp.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/common/packet/printer/PacketPrinter.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/physicallayer/contract/packetlevel/SignalTag_m.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/physicallayer/common/packetlevel/Radio.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/physicallayer/base/packetlevel/TransmissionBase_m.h"
#include "inet/common/scheduler/RealTimeScheduler.h"
#include <fstream>

using namespace inet;

Define_Module(RToFApp);

RToFApp::~RToFApp()
{
    cancelAndDelete(selfMsg);
}

void RToFApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    broadcastTime = 1;
    std::vector<double> xVector;
    std::vector<double> yVector;

    if (stage == INITSTAGE_LOCAL) {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);



        localPort = par("localPort");
        destPort = par("destPort");
        isReceiver = par("isReceiver");
        //startTime = par("startTime");
        //stopTime = par("stopTime");
        packetName = par("packetName");
        //dontFragment = par("dontFragment");
        /*if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
            throw cRuntimeError("Invalid startTime/stopTime parameters");
        */
        selfMsg = new cMessage("sendTimer");
    }
}

void RToFApp::setSocketOptions()
{
//    int timeToLive = par("timeToLive");
//    if (timeToLive != -1)
//        socket.setTimeToLive(timeToLive);
//
//    int dscp = par("dscp");
//    if (dscp != -1)
//        socket.setDscp(dscp);
//
//    int tos = par("tos");
//    if (tos != -1)
//        socket.setTos(tos);
//
//    const char *multicastInterface = par("multicastInterface");
//    if (multicastInterface[0]) {
//        IInterfaceTable *ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
//        InterfaceEntry *ie = ift->findInterfaceByName(multicastInterface);
//        if (!ie)
//            throw cRuntimeError("Wrong multicastInterface setting: no interface named \"%s\"", multicastInterface);
//        socket.setMulticastOutputInterface(ie->getInterfaceId());
//    }
//
//    bool receiveBroadcast = par("receiveBroadcast");
//    if (receiveBroadcast)
//        socket.setBroadcast(true);
//
//    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");
//    if (joinLocalMulticastGroups) {
//        MulticastGroupList mgl = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this)->collectMulticastGroups();
//        socket.joinLocalMulticastGroups(mgl);
//    }
      socket.setCallback(this);
}

void RToFApp::processStart()
{
    socket.setOutputGate(gate("socketOut"));

    const char *localAddress = par("localAddress");

    //socket.bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);

    socket.bind(localPort);



    if(!isReceiver){
        socket.setBroadcast(false);
    }
    else{
        socket.setBroadcast(true);
    }

    setSocketOptions();

    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    while ((token = tokenizer.nextToken()) != nullptr) {
        if (strstr(token, "Broadcast") != nullptr)
            destAddresses.push_back(Ipv4Address::ALLONES_ADDRESS);
        else {
            L3Address result;
            L3AddressResolver().tryResolve(token, result);
            if (result.isUnspecified())
                EV_ERROR << "cannot resolve destination address: " << token << endl;
            destAddresses.push_back(result);
        }
    }
    if (destAddresses.empty()) {
        EV_ERROR << "Error sending - destAddresses empty" << endl;
    }
    if (isReceiver == false){
        sendPacket();
    }
}

L3Address RToFApp::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    if (destAddresses[k].isUnspecified() || destAddresses[k].isLinkLocal()) {
        L3AddressResolver().tryResolve(destAddressStr[k].c_str(), destAddresses[k]);
    }
    return destAddresses[k];
}

void RToFApp::sendPacket()
{
    std::ostringstream str;

    str << packetName << "-" << numSent;

    Packet *packet = new Packet(str.str().c_str());
    /*if(dontFragment)
        packet->addTag<FragmentationReq>()->setDontFragment(true);
    */
    const auto& payload = makeShared<ApplicationPacket>();
    payload->setChunkLength(B(4));
    payload->setSequenceNumber(numSent);
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());

    packet->insertAtBack(payload);
    L3Address destAddr = chooseDestAddr();

    cModule *host = getContainingNode(this);
    std::cout << "host: " << host << endl;

    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddr, destPort);

    numSent++;

}



//void RToFApp::processSend()
//{
//    sendPacket();
//    simtime_t d = simTime() + par("sendInterval");
//    /*if (stopTime < SIMTIME_ZERO || d < stopTime) {
//        selfMsg->setKind(SEND);
//        scheduleAt(d, selfMsg);
//    }
//    else {
//        selfMsg->setKind(STOP);
//        scheduleAt(stopTime, selfMsg);
//    }*/
//}

void RToFApp::processStop()
{
    socket.close();
}

void RToFApp::handleMessageWhenUp(cMessage *msg)
{
    /*
    if (msg->isSelfMessage()) {
        ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
            case START:
                processStart();
                break;

            case SEND:
                processSend();
                break;

            case STOP:
                processStop();
                break;

            default:
                throw cRuntimeError("Invalid kind %d in self message", (int)selfMsg->getKind());
        }
    }
    else*/
        socket.processMessage(msg);
}

void RToFApp::finish()
{
    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);
    ApplicationBase::finish();
}

void RToFApp::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    // process incoming packet
    processPacket(packet);
}

void RToFApp::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void RToFApp::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void RToFApp::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();
    char buf[100];
    sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}

void RToFApp::processPacket(Packet *pk)
{
    emit(packetReceivedSignal, pk);
    EV_INFO << "Received packet: " << UdpSocket::getReceivedPacketInfo(pk) << endl;
    std::cout << "Received packet: " << UdpSocket::getReceivedPacketInfo(pk) << endl;


    if(isReceiver){


        //getting address
        auto l3Addresses = pk->getTag<L3AddressInd>();
        L3Address destAddr = l3Addresses->getSrcAddress();

        auto signalTimeTag = pk->getTag<SignalTimeInd>();

        std::ostringstream str;
        str << packetName << "-" << numSent;

        Packet *packet = new Packet(str.str().c_str());
        const auto& payload = makeShared<ApplicationPacket>();
        payload->setChunkLength(B(4));
        payload->setSequenceNumber(numSent);

        //getting the real position of nodes
        cModule *host = getContainingNode(this);
        IMobility *mobility = check_and_cast<IMobility *>(host->getSubmodule("mobility"));
        auto real_position = mobility->getCurrentPosition();
        double x = mobility->getCurrentPosition().x;
        double y = mobility->getCurrentPosition().y;

        packet->setName(ConvertDoubleToString(x, y));

        packet->setTimestamp(signalTimeTag->getStartTime());

        std::cout << "T-E-S-T: " << signalTimeTag->getStartTime() <<endl;
        std::cout << "T-E-S-T: " << signalTimeTag->getEndTime() <<endl;

        //payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
        auto tags=payload->addTag<CreationTimeTag>();
        tags->setCreationTime(signalTimeTag->getEndTime());


        packet->insertAtBack(payload);

        emit(packetSentSignal, packet);
        socket.sendTo(packet, destAddr, destPort);



//        L3Address hostName = l3Addresses->getSrcAddress();
//        auto dist = distanceCalc( pk->getSendingTime());
//        std::cout << "-------------" << endl;
//        std::cout << "-------------" << endl;
//        std::cout << "host: " << host << " -- IP:" << hostName << endl;
//        std::cout << "real position = " << real_position << endl;
//        std::cout << "sending = " << pk->getSendingTime() << endl;
//        std::cout << "Distance between hosts= " << dist << endl;
//        std::cout << "-------------" << endl;
//        std::cout << "-------------" << endl;
        numSent++;

        std::cout << "real position->> X = " << x << " e Y = "<<y<<endl;
    }else{
        saveTime(pk->getTimestamp());
        auto signalTimeTag = pk->getTag<SignalTimeInd>();

        std::cout << "-------------" << endl;
        std::cout << "Broadcast time: " << broadcastTime << endl;
        std::cout << "-T-E-S-T::" << pk->getCreationTime() << endl;

        cModule *host = getContainingNode(this);
        std::cout << "host : " << host << endl;

        auto l3Addresses = pk->getTag<L3AddressInd>();
        L3Address hostName = l3Addresses->getSrcAddress();
        //V << "host sender = " << hostName << endl;
        std::cout << "host sender = " << hostName << endl;
        std::cout << "position host sender = " << pk->getFullName() << endl;


        auto endTime = signalTimeTag->getEndTime();
        EV << "endTime = " << endTime << endl;
        std::cout << "endTime = " << endTime << endl;
        auto dist = distanceCalc( pk->getCreationTime());

        std::cout << "arrival = " << pk->getArrivalTime() << endl;
        std::cout << "Distance between hosts= " << dist << endl;
        std::cout << "-------------" << endl;

    }
    delete pk;
    numReceived++;
}

void RToFApp::handleStartOperation(LifecycleOperation *operation)
{
    /*simtime_t start = std::max(startTime, simTime());
    if ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime)) {
        selfMsg->setKind(START);
        scheduleAt(start, selfMsg);
    }*/
    processStart();
}


void RToFApp::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void RToFApp::handleCrashOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.destroy();         //TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
}

double RToFApp::distanceCalc(simtime_t finalT)
{
    double distance = ((299792458 * (finalT - broadcastTime).dbl())/2) - 81842.34103; //81842.34103 overhead, this was found through an environment with two hosts at a distance of 1m, thus calculating the distance, the result with verhead was subtracted of the real distance
    return distance;
}

//void RToFApp::MinMax()
//{
//    double di = 11;
//
//}
//

void RToFApp::saveTime(simtime_t broad){
    if(broadcastTime > broad){
        broadcastTime = broad;
    }else{
        broadcastTime = broadcastTime;
    }
}

void RToFApp::savePoints(double xi, double yi){
    xVector.push_back(xi);
    yVector.push_back(yi);

    std::cout << "xVector = { ";
       for (int n : xVector) {
           std::cout << n << ", ";
       }
       std::cout << "}; \n";

//    for (unsigned int i = 0; i < xVector.size(); i++)
//    {
//        std::cout << "VECTORRRR X: " << xVector[i] <<"," << endl;
//        std::cout << "VECTORRRR Y: " << yVector[i] <<"," << endl;
//    }
//    std::cout << " " << endl;
}

const char* RToFApp::ConvertDoubleToString(double value1, double value2){
    std::stringstream ss ;
    ss << value1 << "," << value2;
    const char* str = ss.str().c_str();
    return str;
}

// namespace inet





