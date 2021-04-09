#include "can_receiver.h"

CAN_Receiver::CAN_Receiver(CAN_Handler *parent)
{
    parentCAN_Handler = parent;
    CAN_comData = parentCAN_Handler->getCAN_Struct();
    CAN_FilterInit();
}

void CAN_Receiver::CAN_FilterInit()
{
    CAN_Filter[0].can_id = Device_ID::CAN_STM1 + ControllerData::R_Position;
    CAN_Filter[0].can_mask = CAN_SFF_MASK;
    CAN_Filter[1].can_id = Device_ID::CAN_STM1 + ControllerData::R_Speed;
    CAN_Filter[1].can_mask = CAN_SFF_MASK;
    CAN_Filter[2].can_id = Device_ID::CAN_STM1 + ControllerCommand::MovingFinishedAcknowedge;
    CAN_Filter[2].can_mask = CAN_SFF_MASK;
    CAN_Filter[3].can_id = Device_ID::CAN_STM1 + RPiCommand::R_CleanPlot;
    CAN_Filter[3].can_mask = CAN_SFF_MASK;
    setsockopt(CAN_comData->s, SOL_CAN_RAW, CAN_RAW_FILTER, &CAN_Filter, sizeof(CAN_Filter));

}

bool CAN_Receiver::dataReceive()
{
    CAN_comData->nbytes = read(CAN_comData->s, &(CAN_comData->frame), sizeof(CAN_comData->frame));
    if(CAN_comData->nbytes > 0) {
        uint32_t can_id = CAN_comData->frame.can_id;
        uint8_t *data = CAN_comData->frame.data;
        uint32_t dlc = CAN_comData->frame.can_dlc;
        for(int i = 0; i < 4; i++){
            CAN_comData->RT_data.uintData[i+4]=data[i+4];
            CAN_comData->RT_data.uintData[i]=data[i];
        }
        if(can_id == Device_ID::CAN_STM1 + ControllerData::R_Position){
            emit AngleSignal(CAN_comData->RT_data.floats.fl1, CAN_comData->RT_data.floats.fl2);
        }
        else if(can_id == Device_ID::CAN_STM1 + ControllerData::R_Speed)
        {
            emit VelocitySignal(CAN_comData->RT_data.floats.fl1, CAN_comData->RT_data.floats.fl2);
        }
        else if(can_id == Device_ID::CAN_STM1 + ControllerData::R_Current)
        {
            emit CurrentSignal(CAN_comData->RT_data.floats.fl1, CAN_comData->RT_data.floats.fl2);
        }
        else if(can_id == Device_ID::CAN_RPi + RPiCommand::R_CleanPlot)
        {
            emit CleanPlotSignal();
        }
        else if(can_id == Device_ID::CAN_RPi + ControllerCommand::MotorMoved){
            //emit StartBreakReceived();
        }
        else if((can_id >= Device_ID::CAN_STM1 + ControllerData::R_PositionProportionalRatio) &&
                 can_id <= Device_ID::CAN_STM1 + ControllerData::R_SpeedDifferentialRatio)
        {
            emit RatioSignal(CAN_comData->RT_data.floats.fl1, CAN_comData->frame.can_id);
        }
        else if((can_id == Device_ID::CAN_STM1 + ControllerCommand::HeartbeatRespond) ||
                (can_id == Device_ID::CAN_STM2 + ControllerCommand::HeartbeatRespond) ||
                (can_id == Device_ID::CAN_STM3 + ControllerCommand::HeartbeatRespond) ||
                (can_id == Device_ID::CAN_STM4 + ControllerCommand::HeartbeatRespond) ||
                (can_id == Device_ID::CAN_STM5 + ControllerCommand::HeartbeatRespond) ||
                (can_id == Device_ID::CAN_STM6 + ControllerCommand::HeartbeatRespond))
            emit HeartbeatSignal();
        return true;
    }
    return false;
}


void CAN_Receiver::receiveAll(){
    while(1){
        dataReceive();
    }
}

