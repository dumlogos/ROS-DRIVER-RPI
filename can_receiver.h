#ifndef CAN_RECEIVER_H
#define CAN_RECEIVER_H

#include "can_handler.h"

#include <QObject>

class CAN_Handler;

class CAN_Receiver : public QObject
{
    Q_OBJECT
private:
    CAN_Handler *parentCAN_Handler;
    CAN_Struct *CAN_comData;

    struct can_filter CAN_Filter[10];
public:
    explicit CAN_Receiver(CAN_Handler *parent);

    void CAN_FilterInit();
    bool dataReceive();

public slots:

    void receiveAll();
    void compareVars();

signals:
    void AngleSignal(double angle, double timeStamp);
    void VelocitySignal(double velocity, double timeStamp);
    void CleanPlotSignal();


};

#endif // CAN_RECEIVER_H
