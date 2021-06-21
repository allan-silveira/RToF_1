/*
 * RToFApp.h
 *
 *  Created on: 17 de dez de 2020
 *      Author: allan
 */

#ifndef RTOFAPP_H_
#define RTOFAPP_H_

#define BUILDING_DLL
#define OPP_DLLIMPORT


#include <vector>

#include <Eigen/Dense>

#include "inet/common/INETDefs.h"
#include "Listener.h"

#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/common/INETDefs.h"
#include "inet/physicallayer/common/packetlevel/Radio.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/common/scheduler/RealTimeScheduler.h"


using namespace inet;

/**
 * UDP application. See NED for more info.
 */
class INET_API RToFApp : public ApplicationBase, public UdpSocket::ICallback
{
  protected:
    enum SelfMsgKinds { START = 1, SEND, STOP };
    int aux = 0;
    const char* arqName = nullptr;
    Coord realPosition;
    double minMax_x, minMax_y, mL_x, mL_y;
    // parameters
    std::vector<L3Address> destAddresses;
    std::vector<std::string> destAddressStr;
    int localPort = -1, destPort = -1;
    //simtime_t startTime;
    //simtime_t stopTime;
    //bool dontFragment = false;
    bool isReceiver;
    int position;
    const char *packetName = nullptr;

    // state
    UdpSocket socket;
    cMessage *selfMsg = nullptr;


    // statistics
    int numSent = 0;
    int numReceived = 0;
    simtime_t broadcastTime;
    std::vector<double> xVector;
    std::vector<double> yVector;
    std::vector<double> di;
    std::vector<double> timeFlight;

    //Listener
    simtime_t IniTime;
    simsignal_t transmissionStarted;
    Listener *listener;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;



    // chooses random destination address
    virtual L3Address chooseDestAddr();
    virtual void sendPacket();
    virtual void processPacket(Packet *msg);

    virtual void setSocketOptions();

    virtual void processStart();
    //virtual void processSend();
    virtual void processStop();


    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    virtual double distanceCalc(simtime_t finalT, simtime_t iniT, simtime_t overhead, simtime_t backoff);
    virtual void saveXPoints(const char *local);
    virtual void saveYPoints(const char *local);
    virtual const char* ConvertDoubleToString(double value1, double value2);
    virtual omnetpp::simtime_t Calibration(simtime_t StartT, simtime_t EndT, simtime_t backoffTime);
    virtual void minMax();
    virtual void mL();


    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;

  public:
    RToFApp() {}
    ~RToFApp();
    void setIniTime(simtime_t iniTime);
};

#endif /* RTOFAPP_H_ */
