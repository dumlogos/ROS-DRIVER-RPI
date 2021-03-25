#include "can_transmitter.h"

CAN_Transmitter::CAN_Transmitter(CAN_Handler *parent)
{
    parentCAN_Handler = parent;
    CAN_comData = parentCAN_Handler->getCAN_Struct();
}

bool CAN_Transmitter::dataTransmit(u_int8_t msg[], u_int32_t msgId, u_int32_t msgDLC)
{
    CAN_comData->frame.can_dlc = msgDLC;
    CAN_comData->frame.can_id = msgId;
    for(u_int8_t i = 0; i < msgDLC; ++i)
        CAN_comData->frame.data[i] = msg[i];
    qDebug() << &(CAN_comData->frame.can_id);
    CAN_comData->nbytes = write(CAN_comData->s, &(CAN_comData->frame), sizeof(CAN_comData->frame));
    if(CAN_comData->nbytes != sizeof(CAN_comData->frame))
    {
        printf("Send Error frame[0]!\r\n");
        return false;
    }
    return true;

}

bool CAN_Transmitter::dataTransmit(u_int32_t msgId, u_int32_t msgDLC)
{
    CAN_comData->frame.can_dlc = msgDLC;
    CAN_comData->frame.can_id = msgId;
    qDebug() << &(CAN_comData->frame.can_id);
    CAN_comData->nbytes = write(CAN_comData->s, &(CAN_comData->frame), sizeof(CAN_comData->frame));
    if(CAN_comData->nbytes != sizeof(CAN_comData->frame)){
        printf("Send Error frame[0]!\r\n");
        return false;
    }
    return true;
}

bool CAN_Transmitter::transmitCommand(uint32_t cmndId,  uint8_t *data)
{
    if(data != nullptr)
        return dataTransmit(data, cmndId, sizeof(data));
    else
        return dataTransmit(cmndId);

}
bool CAN_Transmitter::transmitCommand(Device_ID device, RPiCommand rpiCommand, uint8_t *data)
{
   return transmitCommand(toCanId(device, rpiCommand), data);
}
bool CAN_Transmitter::transmitCommand(Device_ID device, ControllerCommand cntrCommand, uint8_t *data)
{
   return transmitCommand(toCanId(device, cntrCommand), data);
}

void CAN_Transmitter::transmitAngle(float position, Device_ID device)
{
    CAN_comData->RT_data.floats.fl1 = position;
    for(int i = 0; i < 4; i++){
        CAN_comData->frame.data[i] = CAN_comData->RT_data.uintData[3-i];
    }
    dataTransmit(toCanId(device, ControllerData::T_Position), 4);
}
void CAN_Transmitter::transmitVelocity(float speed, Device_ID device)
{
    CAN_comData->RT_data.floats.fl1 = speed;
    for(int i = 0; i < 4; i++){

        CAN_comData->frame.data[i] = CAN_comData->RT_data.uintData[3-i];
    }
    dataTransmit(toCanId(device, ControllerData::T_Speed), 4);

}

void CAN_Transmitter::transmitStartBreak(DriverState state, Device_ID device)
{
    u_int8_t startState[1];

    if(state == DriverState::START){
        startState[0] = {DriverState::START};
        dataTransmit(startState,
                     toCanId(device, ControllerCommand::ToggleStopDriver),
                     sizeof(startState));
    }
    else if (state == DriverState::STOP){
        startState[0] = {DriverState::STOP};
        dataTransmit(startState,
                     toCanId(device, ControllerCommand::ToggleStopDriver),
                     sizeof(startState));
    }

}
void CAN_Transmitter::transmitClearPlot()
{
    for(int i = 0; i<6; ++i)
        dataTransmit(toCanId(Device_ID::CAN_All, RPiCommand::T_CleanPlot), 0);
}

void CAN_Transmitter::transmitRatio(float ratio, Device_ID device, ControllerData ratioType)
{
    CAN_comData->RT_data.floats.fl1 = ratio;
    for(int i = 0; i < 4; i++){
        CAN_comData->frame.data[i] = CAN_comData->RT_data.uintData[3-i];
    }
    dataTransmit(CAN_comData->frame.data, toCanId(device, ratioType), 4);
}


