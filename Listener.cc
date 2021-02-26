/*
 * Listener.cc
 *
 *  Created on: 24 de fev de 2021
 *      Author: allan
 */
#define BUILDING_DLL
#define OPP_DLLIMPORT

#include "Listener.h"


#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "RToFApp.h"



using namespace inet;

Listener::Listener(){

}

Listener::~Listener()
{

}

void Listener::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details)
{
    Enter_Method_Silent("receiveSignal");
    EV << "Listener time: " << simTime();
    cModule *targetModule = getSystemModule()->getSubmodule("host1",0);
    cModule *appModule = targetModule->getSubmodule("app",0);
    RToFApp *app = check_and_cast<RToFApp*>(appModule);
    app->setIniTime(simTime());

}



