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
#include "Backoff_m.h"
#include "Location_m.h"
#include <fstream>

using namespace inet;

Define_Module(RToFApp);

RToFApp::~RToFApp()
{
    cancelAndDelete(selfMsg);
}

simtime_t backoffTest;

void RToFApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    std::vector<double> xVector;
    std::vector<double> yVector;

    if (stage == INITSTAGE_LOCAL) {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);

        aux = par("aux");

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

    listener = new Listener();
    host->subscribe("transmissionStarted", listener);

    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddr, destPort);

    //broadcastTime = simTime();

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

    cModule *host = getSystemModule()->getSubmodule("host1");
    host->unsubscribe("transmissionStarted", listener);

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

        auto loc = packet->addTagIfAbsent<location>();
        loc->setXPosition(x);
        loc->setYPosition(y);

        packet->setName(ConvertDoubleToString(x, y));

        packet->setTimestamp(signalTimeTag->getStartTime());
        std::cout << "-------------------------" <<  endl;
        std::cout << "host : " << host << endl;

        //alterado agora std::cout << "T-E-S-T Backoff: " << pk->getTag<backoff>()->getBackoffTime() <<endl;

//        std::cout << "T-E-S-T start T : " << signalTimeTag->getStartTime() <<endl;
//        std::cout << "T-E-S-T end T: " << signalTimeTag->getEndTime() <<endl;

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
        std::cout << "-------------------------" <<  endl;
    }else{
        //saveTime(pk->getTimestamp());
        auto signalTimeTag = pk->getTag<SignalTimeInd>();

        std::cout << "-------------" << endl;
        std::cout << "Broadcast time: " << IniTime << endl;
        broadcastTime = IniTime;
        //std::cout << "-T-E-S-T::" << pk->getCreationTime() << endl;

        cModule *host = getContainingNode(this);
        std::cout << "host : " << host << endl;

        auto l3Addresses = pk->getTag<L3AddressInd>();
        L3Address hostName = l3Addresses->getSrcAddress();
        //EV << "host sender = " << hostName << endl;
        std::cout << "host sender = " << hostName << endl;
        //std::cout << "position host sender = " << pk->getFullName() << endl;

        auto xyLocation = pk->getTag<location>();
        savePoints(pk->getFullName());
        std::cout << "Get tag Xposition: " << xyLocation->getXPosition() << "   Get tag Yposition: " <<  xyLocation->getYPosition() << endl;


        auto backoffTime = pk->getTag<backoff>(); //getting backoffTime do CSMA


        if(aux == 1){
            auto overhead = Calibration(broadcastTime, pk->getArrivalTime(), backoffTime->getBackoffTime() );//overhead from environment with two hosts is 0.001182000001
            std::cout << "overhead: " << overhead << endl;
        }

        auto endTime = signalTimeTag->getEndTime();
        EV << "endTime = " << endTime << endl;
        std::cout << "endTime = " << endTime << endl;


        backoffTest += backoffTime->getBackoffTime();

        std::cout << "TESTE do CSMA: " << backoffTime->getBackoffTime() << " TESTE do cancel Time: " << backoffTime->getInitialBackoffTime() << endl;

        auto dist = distanceCalc(pk->getArrivalTime(), broadcastTime, 0.001142,  backoffTime->getBackoffTime());
        std::cout << "Distance between hosts getting auto backoff= " << dist << endl;

        auto dist2 = distanceCalc(pk->getArrivalTime(), broadcastTime, 0.001142, backoffTime->getInitialBackoffTime());
        std::cout << "Distance between with getting difference time on cancelbackoff hosts= " << dist2 << endl;


//        if(hostName.str() == "10.0.0.2"){
////            std::cout << "--BACKOFF-- = " << backoffTime->getBackoffTime() << endl;
//            auto dist = distanceCalc(pk->getArrivalTime(), broadcastTime, 0.001142, 0.001084149174);
//            std::cout << "Distance between hosts TEST EXTREME= " << dist << endl;
//        }//else{
////            std::cout << "--BACKOFF-- = " << backoffTime->getBackoffTime() << endl;
////            auto dist = distanceCalc(pk->getArrivalTime(), 0.001142 + 0.000824 + 0.000824, 0.00056, backoffTime->getInitialBackoffTime());
////            std::cout << "Distance between hosts= " << dist << endl;
////        }
//        //tests environment with 3 hosts
//        else if(hostName.str() == "10.0.0.4"){
//            auto dist = distanceCalc(pk->getArrivalTime(), broadcastTime, 0.001142, 0.00220824352);
//            std::cout << "Distance between TEST EXTREME= " << dist << endl;
//        }//else{
//            auto backoffTest2host = 0.00036 + 0.00028 + 0.000543;
//            auto dist = distanceCalc(pk->getArrivalTime(), 0.001142 + 0.00005 + 0.000546 + 0.000218 + 0.000080066713 + 0.000010066713, backoffTime->getBackoffTime(), backoffTime->getInitialBackoffTime());
//            std::cout << "Distance between hosts= " << dist << endl;
//        }

//        std::cout << "arrival = " << pk->getArrivalTime() << endl;
//        std::cout << "Distance between hosts= " << dist << endl;
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

double RToFApp::distanceCalc(simtime_t finalT, simtime_t iniT, simtime_t overhead, simtime_t backoff)
{
    double distance = ((299792458 * (finalT - iniT - overhead - backoff ).dbl())/2.0); // overhead, this was found through an environment with two hosts at a distance of 1m, thus calculating the distance, the result with verhead was subtracted of the real distance
    return distance;
}

//void RToFApp::MinMax(double di)
//{
//    double di = 11;
//
//}
//


void RToFApp::savePoints(const char *local){
    char loc[strlen(local) + 1];
    strcpy(loc, local);
    char x[0];
    char y[0];
    char aux[] = ",";
    int j = 0;
    int k = 0;
    int auxV = 0;
    std::cout << "----TESTE local: " << local << endl;
    while(loc[j] != aux[0] && j <= auxV){
        std::cout << "----TESTE LOCAL: " << loc[j] << "---AUX: "<< aux[0] << endl;
        x[j] = loc[j];
        j++;
        auxV++;
        std::cout << "--TESTE X while: " << x << "--TESTE J while: " << j << endl;
    }
    auxV = auxV + 2;
    std::cout << "--TESTE j: " << j << endl;
    std::cout << "--TESTE X 1: " << x << endl;
    std::cout << "--TESTE j: " << j << endl;
    for(int i = j+1; i<= strlen(loc); i++){
        y[k] = loc[i];
        k++;
        std::cout << "--TESTE X 2: " << x << endl;
    }
    std::cout << "--TESTE X 2: " << x << endl;
//        if(loc[i] == aux[0]){
//            y[k] = loc[i+1];
//            k++;
//        }
//        else{
//            x[j] = loc[i];
//            j++;
//        }
    std::cout << "----TESTE X: " << x << endl;
    std::cout << "----TESTE Y: " << y << endl;
    xVector.push_back(atof(x));
    yVector.push_back(atof(y));
    for (unsigned int i = 0; i < xVector.size(); i++)
    {
        std::cout << "VECTORRRR X: " << xVector[i] <<"," << endl;
        std::cout << "VECTORRRR Y: " << yVector[i] <<"," << endl;
    }
    std::cout << " " << endl;
}


const char* RToFApp::ConvertDoubleToString(double value1, double value2){
    std::stringstream ss ;
    ss << value1 << "," << value2;
    const char* str = ss.str().c_str();
    return str;
}

omnetpp::simtime_t RToFApp::Calibration(simtime_t StartT, simtime_t EndT, simtime_t backoffTime){
    auto overhead = (EndT - StartT - backoffTime) - ((2*sqrt(1)) / 299792458.0); //((2 * 1) / 299792458) here we have the calc of real time, so we subtract this real time from time with overhead and we find the overhead
    return overhead;
}

void RToFApp::setIniTime(simtime_t time)
{
    IniTime = time;
}
// namespace inet





