#include "can_transmitter.h"

CAN_Transmitter::CAN_Transmitter(CAN_Handler *parent)
{
    parentCAN_Handler = parent;
    CAN_comData = parentCAN_Handler->getCAN_Struct();

}

void CAN_Transmitter::dataTransmit(u_int8_t msg[], u_int32_t msgId, u_int32_t msgDLC)
{
    CAN_comData->frame.can_dlc = msgDLC;
    CAN_comData->frame.can_id = msgId;
    for(u_int8_t i = 0; i < msgDLC; ++i)
        CAN_comData->frame.data[i] = msg[i];
    qDebug() << &(CAN_comData->frame.can_id);
    CAN_comData->nbytes = write(CAN_comData->s, &(CAN_comData->frame), sizeof(CAN_comData->frame));
    if(CAN_comData->nbytes != sizeof(CAN_comData->frame))
        printf("Send Error frame[0]!\r\n");

}

void CAN_Transmitter::dataTransmit(u_int32_t msgId, u_int32_t msgDLC)
{
    CAN_comData->frame.can_dlc = msgDLC;
    CAN_comData->frame.can_id = msgId;
    qDebug() << &(CAN_comData->frame.can_id);
    CAN_comData->nbytes = write(CAN_comData->s, &(CAN_comData->frame), sizeof(CAN_comData->frame));
    if(CAN_comData->nbytes != sizeof(CAN_comData->frame))
        printf("Send Error frame[0]!\r\n");
}

void CAN_Transmitter::transmitAngle(float angle)
{
    CAN_comData->RT_data.floats.fl1 = angle;
    for(int i = 0; i < 4; i++){
        CAN_comData->frame.data[i] = CAN_comData->RT_data.uintData[3-i];
    }
    dataTransmit(CAN_ID::T_Angle, 4);

}


void CAN_Transmitter::transmitVelocity(float velocity)
{
    CAN_comData->RT_data.floats.fl1 = velocity;
    for(int i = 0; i < 4; i++){

        CAN_comData->frame.data[i] = CAN_comData->RT_data.uintData[3-i];
    }
    dataTransmit(CAN_ID::T_Velocity, 4);

}

void CAN_Transmitter::transmitStartBreak(DriverState state)
{
    u_int8_t startState[1];

    if(state == DriverState::START){
        startState[0] = {DriverState::START};
        dataTransmit(startState, CAN_ID::T_StartBreak, sizeof(startState));
    }
    else if (state == DriverState::STOP){
        startState[0] = {DriverState::STOP};
        dataTransmit(startState, CAN_ID::T_StartBreak, sizeof(startState));
    }

}


void CAN_Transmitter::transmitClearPlot()
{
    dataTransmit(CAN_ID::T_CleanPlot, 0);
}

void CAN_Transmitter::transmitRatio(float ratio, CAN_ID id)
{
    CAN_comData->RT_data.floats.fl1 = ratio;
    for(int i = 0; i < 4; i++){
        CAN_comData->frame.data[i] = CAN_comData->RT_data.uintData[3-i];
    }
    dataTransmit(CAN_comData->frame.data, id, 4);
}


