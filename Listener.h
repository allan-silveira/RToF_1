/*
 * Listener.h
 *
 *  Created on: 24 de fev de 2021
 *      Author: allan
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/applications/base/ApplicationBase.h"


#define BUILDING_DLL
#define OPP_DLLIMPORT

using namespace inet;

class INET_API Listener : public cListener, public cSimpleModule
{
protected:
    virtual void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details) override;
    UdpSocket socket;

public:
    Listener();
    virtual ~Listener();
};

#endif /* LISTENER_H_ */
