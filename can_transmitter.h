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
    void dataTransmit(u_int8_t msg[8], u_int32_t msgId = 0x200, u_int32_t msgDLC = 0);
    void dataTransmit(u_int32_t msgId = 0x200, u_int32_t msgDLC = 0);


public slots:
    void transmitAngle(float angle);
    void transmitVelocity(float velocity);
    void transmitStartBreak(DriverState state);
    void transmitClearPlot();
    void transmitRatio(float ratio, CAN_ID id);


signals:

};

#endif // CAN_TRANSMITTER_H
