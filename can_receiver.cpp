#include "can_receiver.h"

CAN_Receiver::CAN_Receiver(CAN_Handler *parent)
{
    parentCAN_Handler = parent;
    CAN_comData = parentCAN_Handler->getCAN_Struct();
    CAN_FilterInit();
}

void CAN_Receiver::CAN_FilterInit()
{
    CAN_Filter[0].can_id = toCanId(Device_ID::CAN_STM1, ControllerData::R_Position);
    CAN_Filter[0].can_mask = CAN_SFF_MASK;
    CAN_Filter[1].can_id = toCanId(Device_ID::CAN_STM1, ControllerData::R_Speed);
    CAN_Filter[1].can_mask = CAN_SFF_MASK;
    CAN_Filter[2].can_id = toCanId(Device_ID::CAN_STM1, ControllerCommand::MovingFinishedAcknowedge);
    CAN_Filter[2].can_mask = CAN_SFF_MASK;
    CAN_Filter[3].can_id = toCanId(Device_ID::CAN_STM1, RPiCommand::R_CleanPlot);
    CAN_Filter[3].can_mask = CAN_SFF_MASK;
    setsockopt(CAN_comData->s, SOL_CAN_RAW, CAN_RAW_FILTER, &CAN_Filter, sizeof(CAN_Filter));

}

bool CAN_Receiver::dataReceive()
{
    CAN_comData->nbytes = read(CAN_comData->s, &(CAN_comData->frame), sizeof(CAN_comData->frame));
    if(CAN_comData->nbytes > 0) {
        if(CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerData::R_Position)){
            for(int i = 0; i < 4; i++){
                CAN_comData->RT_data.uintData[i+4]=CAN_comData->frame.data[i+4];
                CAN_comData->RT_data.uintData[i]=CAN_comData->frame.data[i];
            }
            emit AngleSignal(CAN_comData->RT_data.floats.fl1, CAN_comData->RT_data.floats.fl2);
        }
        else if(CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerData::R_Speed))
        {
            for(int i = 0; i < 4; i++){
                CAN_comData->RT_data.uintData[i+4]=CAN_comData->frame.data[i+4];
                CAN_comData->RT_data.uintData[i]=CAN_comData->frame.data[i];
            }

            emit VelocitySignal(CAN_comData->RT_data.floats.fl1, CAN_comData->RT_data.floats.fl2);
        }
        else if(CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerData::R_Current)){
            for(int i = 0; i < 4; i++){
                CAN_comData->RT_data.uintData[i+4]=CAN_comData->frame.data[i+4];
                CAN_comData->RT_data.uintData[i]=CAN_comData->frame.data[i];
            }

            emit CurrentSignal(CAN_comData->RT_data.floats.fl1, CAN_comData->RT_data.floats.fl2);
        }
        else if(CAN_comData->frame.can_id == toCanId(Device_ID::CAN_RPi, RPiCommand::R_CleanPlot)){

            emit CleanPlotSignal();
        }
        else if(CAN_comData->frame.can_id == toCanId(Device_ID::CAN_RPi, ControllerCommand::MotorMoved)){
            for(int i = 0; i < 8; ++i){
               CAN_comData->RT_data.uintData[i] = CAN_comData->frame.data[i];
            }
            //emit StartBreakReceived();
        }
        else if((CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerData::R_PositionProportionalRatio)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerData::R_PositionDifferentialRatio)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerData::R_PositionIntegralRatio)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerData::R_SpeedProportionalRatio)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerData::R_SpeedDifferentialRatio)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerData::R_SpeedIntegralRatio))){

            for(int i = 0; i < 8; ++i){
               CAN_comData->RT_data.uintData[i] = CAN_comData->frame.data[i];
            }
            emit RatioSignal(CAN_comData->RT_data.floats.fl1, CAN_comData->frame.can_id);
        }
        else if((CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM1, ControllerCommand::HeartbeatRespond)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM2, ControllerCommand::HeartbeatRespond)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM3, ControllerCommand::HeartbeatRespond)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM4, ControllerCommand::HeartbeatRespond)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM5, ControllerCommand::HeartbeatRespond)) ||
                (CAN_comData->frame.can_id == toCanId(Device_ID::CAN_STM6, ControllerCommand::HeartbeatRespond)))
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

