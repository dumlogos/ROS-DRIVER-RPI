#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

#include "can_struct.h"
#include "can_transmitter.h"
#include "can_receiver.h"

#include "UNISTD_CLOSE_OVERRIDE.h"
#include "CAN_ENUM.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <math.h>

#include <QWidget>
#include <QThread>
#include <QTimer>
#include <QDebug>

class CAN_Transmitter;
class CAN_Receiver;

class CAN_Handler : public QWidget
{
    Q_OBJECT

    friend class CAN_Transmitter;
    friend class CAN_Receiver;
private:

    QThread *transmitterThread;
    QThread *receiverThread;

    QString iface;
    CAN_Struct CAN_comData;

    QTimer *heartbeatTimer;
    uint8_t heartbeatRequests;

public:
    CAN_Transmitter *Transmitter;
    CAN_Receiver *Receiver;

    explicit CAN_Handler(QWidget *parent = nullptr);
    ~CAN_Handler();

    bool CANHandlerSetup();
    bool CANHandlerSetDown();
    QString getIface();


    CAN_Struct* getCAN_Struct();
    void Handle();

public slots:
    void HeartbeatTransmit();
    void HeartbeatReceived();
    bool CANSetInterface(CAN_IFace iface);

signals:
   void ReceiveAllCAN();
   void RatioQuery();

};

uint32_t toCanId(Device_ID device, ControllerCommand command);
uint32_t toCanId(Device_ID device, ControllerData dataType);
uint32_t toCanId(Device_ID device, RPiCommand command);

#endif // CAN_HANDLER_H
