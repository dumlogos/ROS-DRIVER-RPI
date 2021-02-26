#ifndef CAN_TRANSMITTER_H
#define CAN_TRANSMITTER_H

#include "can_handler.h"

#include "CAN_ENUM.h"
#include <QDoubleValidator>

#include <QObject>

class CAN_Handler;

class CAN_Transmitter : public QObject
{
    Q_OBJECT
private:

    CAN_Handler *parentCAN_Handler;
    CAN_Struct *CAN_comData;

public:
    explicit CAN_Transmitter(CAN_Handler *parent = nullptr);
    bool dataTransmit(u_int8_t msg[8], u_int32_t msgId = 0x200, u_int32_t msgDLC = 0);
    bool dataTransmit(u_int32_t msgId = 0x200, u_int32_t msgDLC = 0);


public slots:
    bool transmitCommand(uint32_t cmndId, uint8_t* data = nullptr);
    bool transmitCommand(Device_ID device, RPiCommand rpiCommand, uint8_t* data = nullptr);
    bool transmitCommand(Device_ID device, ControllerCommand cntrCommand, uint8_t* data = nullptr);


    void transmitAngle(float position, Device_ID device = Device_ID::CAN_STM1) ;
    void transmitVelocity(float velocity, Device_ID device = Device_ID::CAN_STM1);
    void transmitStartBreak(DriverState state, Device_ID device = Device_ID::CAN_STM1);
    void transmitClearPlot();
    void transmitRatio(float ratio, Device_ID device, ControllerData ratioType);


signals:

};

#endif // CAN_TRANSMITTER_H
