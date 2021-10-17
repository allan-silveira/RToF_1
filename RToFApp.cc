/*
 * RToFApp.cc
 *
 *  Created on: 17 de dez de 2020
 *      Author: allan
 */

#define BUILDING_DLL
#define OPP_DLLIMPORT

#include <chrono>

#include <Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <algorithm>


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
    std::vector<double> di;

    if (stage == INITSTAGE_LOCAL) {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);
        minMax_x  = 0;
        minMax_y  = 0;
        mL_x = 0;
        mL_y = 0;

        aux = par("aux");

        arqName = par("arqName");
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
    if (!isReceiver){
        std::ifstream arq;
        std::ofstream myfile;

        arq.open(arqName);

        if(arq.is_open()){
            myfile.open (arqName,std::ios::app);

            for(int i = 0; i < xVector.size(); i++){
                myfile.precision(10);
                myfile<<std::fixed;
                myfile<<timeFlight[i]<<",";
            }
            for(int i = 0; i < xVector.size(); i++){
                myfile<<di[i]<<",";
            }

            myfile<<minMax_x<<","<<minMax_y<<",";
            myfile<<mL_x<<","<<mL_y<<",";
            myfile<<realPosition.x<<","<<realPosition.y<<",";

            myfile<<"\n";
        }else{
            myfile.open(arqName);

            for(int i = 0; i < xVector.size(); i++){
                myfile << "Overhead + backofftime " << i << "(s),";
            }

            for(int i = 0; i < xVector.size(); i++){
                myfile << "Distance host " << i << "(m),";
            }
            myfile << "Min max position X (m),";
            myfile << "Min max position Y (m),";
            myfile << "Likelihood position X (m),";
            myfile << "Likelihood position Y (m),";
            myfile << "Real position X (m),";
            myfile << "Real position Y (m)";

            myfile<<"\n";

            for(int i = 0; i < xVector.size(); i++){
                myfile.precision(10);
                myfile<<std::fixed;
                myfile<<timeFlight[i]<<",";
            }
            for(int i = 0; i < xVector.size(); i++){
                myfile<<di[i]<<",";
            }
            myfile<<minMax_x<<","<<minMax_y<<",";
            myfile<<mL_x<<","<<mL_y<<",";
            myfile<<realPosition.x<<","<<realPosition.y<<",";

            myfile<<"\n";
        }
        myfile.close();
        arq.close();
    }

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

        auto pos = mobility->getCurrentPosition();
        packet->setName(pos.str().c_str());

        packet->setTimestamp(signalTimeTag->getStartTime());
        std::cout << "-------------------------" <<  endl;
        std::cout << "host : " << host << endl;


        auto tags=payload->addTag<CreationTimeTag>();
        tags->setCreationTime(signalTimeTag->getEndTime());

        packet->insertAtBack(payload);

        emit(packetSentSignal, packet);
        socket.sendTo(packet, destAddr, destPort);

        numSent++;

        std::cout << "real position->> " << pos <<endl;
        std::cout << "-------------------------" <<  endl;
    }else{
        //saveTime(pk->getTimestamp());
        auto signalTimeTag = pk->getTag<SignalTimeInd>();

        std::cout << "-------------" << endl;
        std::cout << "Broadcast time: " << IniTime << endl;
        broadcastTime = IniTime;

        cModule *host = getContainingNode(this);
        std::cout << "Host that received the message: " << host << endl;

        auto l3Addresses = pk->getTag<L3AddressInd>();
        L3Address hostName = l3Addresses->getSrcAddress();
        std::cout << "Host that sent the message: " << hostName << endl;
        //std::cout << "position host sender = " << pk->getFullName() << endl;

        saveXPoints(pk->getFullName());
        saveYPoints(pk->getFullName());

        auto backoffTime = pk->getTag<backoff>(); //getting backoffTime do CSMA

        if(aux == 1){
            auto overhead = Calibration(broadcastTime, pk->getArrivalTime(), backoffTime->getBackoffTime() );//overhead from environment with two hosts is 0.001182000001
            std::cout << "overhead: " << overhead << endl;
        }

//        auto endTime = signalTimeTag->getEndTime();
//        std::cout << "endTime = " << endTime << endl;


        backoffTest += backoffTime->getBackoffTime();

        std::cout << "TESTE do CSMA: " << backoffTime->getBackoffTime() << " TESTE do cancel Time: " << backoffTime->getInitialBackoffTime() << endl;

        //auto measuredWithNoise = pk->getArrivalTime() + host->intuniform(-2,2)*0.0000000125;
        auto measuredWithNoise = pk->getArrivalTime();

        auto dist = distanceCalc(measuredWithNoise, broadcastTime, 0.001142,  backoffTime->getBackoffTime());
        std::cout << "Distance between hosts getting auto backoff = " << dist << endl;

        std::cout << " " << endl;
        std::cout << " " << endl;

        di.push_back(dist);

        for (unsigned int i = 0; i < yVector.size(); i++)
        {
            std::cout << "di: " << di[i] <<",   Time Flight: " << timeFlight[i] << endl;
        }

        minMax();
        mL();

        //getting the real position
        IMobility *mobility = check_and_cast<IMobility *>(host->getSubmodule("mobility"));
        realPosition = mobility->getCurrentPosition();

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
    timeFlight.push_back((overhead + backoff ).dbl());
    return distance;
}

void RToFApp::minMax()
{
    // double xv[4], yv[4];
    // std::cout << "testing min max X0: "<< xv[0] << endl;
    double max_x, min_x, max_y, min_y;
    double x1, x2, y1, y2;

    for(int i = 0; i < xVector.size(); i++){
        if(i==0)
        {
            max_x = xVector[i] - di[i];
            min_x = xVector[i] + di[i];

            max_y = yVector[i] - di[i];
            min_y = yVector[i] + di[i];
        }
        else
        {
            if (max_x < xVector[i] - di[i])
              max_x = xVector[i] - di[i];

            if (min_x > xVector[i] + di[i])
              min_x = xVector[i] + di[i];

            if (max_y < yVector[i] - di[i])
              max_y = yVector[i] - di[i];

            if (min_y > yVector[i] + di[i])
              min_y = yVector[i] + di[i];
        }
    }
    minMax_x = (max_x + min_x)/2;
    minMax_y = (max_y + min_y)/2;
    std::cout << "testing min max X0: "<< minMax_x <<" Y0: " << minMax_y << endl;
}

void RToFApp::mL(){
    int numH = xVector.size();
    Eigen::MatrixXd A(numH - 1, 2);
    Eigen::MatrixXd b(numH - 1, 1);
    for(int i = 0; i < numH - 1; i++){
        A(i,0)=2*(xVector[numH - 1] - xVector[i]);
    }
    for(int i = 0; i < numH - 1; i++){
        A(i,1)=2*(yVector[numH - 1] - yVector[i]);
    }
//    std::cout << "testing matrix: "<< endl << A << endl;
    for(int i = 0; i < numH - 1; i++){
        b(i,0) = (-(pow(xVector[i],2)) - (pow(yVector[i],2)) + (pow(di[i],2))) - (-(pow(xVector[numH - 1],2)) - (pow(yVector[numH - 1],2)) + pow(di[numH - 1],2));
    }


    Eigen::MatrixXd A_i(2,2);
    Eigen::MatrixXd A_t(2,2);
    A_t = A.transpose();
//    std::cout << "Resultado A_t: " << endl << A_t <<endl;
    A_i = (A_t * A).inverse();
    A = A_i * A_t * b;
    mL_x = A(0,0);
    mL_y = A(1,0);
    std::cout << "Resultado ML x,y: " << endl << mL_x << ", " << mL_y <<endl;
}


void RToFApp::saveXPoints(const char *local){
    int i;
    char x[9];
    char aux[] = ",";
    int j = 0;
    std::cout << "----TESTE local: " << local << endl;
    for(i = 0;local[i+1] != aux[0];i++){
        x[i] = local[i+1];
    }

    x[i] = '\0';
//    std::cout << "----TESTE X: " << x << endl;
    xVector.push_back(atof(x));
//    for (i = 0; i < xVector.size(); i++)
//    {
//        std::cout << "VECTORRRR X: " << xVector[i] << "," << endl;
//    }
//    std::cout << " " << endl;

}

void RToFApp::saveYPoints(const char *local){
    char y[9];
    char aux[] = ",";
    int j = 0;
    int k = 0;
    std::cout << "----TESTE local: " << local << endl;
    for(int i = 0;local[i] != aux[0];i++){
       j = i;
    }

    for(int i = j+2; i< strlen(local); i++){
        y[k] = local[i];
        k++;
    }
    y[k] = '\0';

//    std::cout << "----TESTE Y: " << y << endl;
    yVector.push_back(atof(y));
//    for (unsigned int i = 0; i < yVector.size(); i++)
//    {
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

omnetpp::simtime_t RToFApp::Calibration(simtime_t StartT, simtime_t EndT, simtime_t backoffTime){
    auto overhead = (EndT - StartT - backoffTime) - ((2*sqrt(1)) / 299792458.0); //((2 * 1) / 299792458) here we have the calc of real time, so we subtract this real time from time with overhead and we find the overhead
    return overhead;
}

void RToFApp::setIniTime(simtime_t time)
{
    IniTime = time;
}
// namespace inet





