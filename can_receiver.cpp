#include "can_receiver.h"

CAN_Receiver::CAN_Receiver(CAN_Handler *parent)
{
    parentCAN_Handler = parent;
    CAN_comData = parentCAN_Handler->getCAN_Struct();
    CAN_FilterInit();
}

void CAN_Receiver::CAN_FilterInit()
{
    CAN_Filter[0].can_id = CAN_ID::R_Angle;
    CAN_Filter[0].can_mask = CAN_SFF_MASK;
    CAN_Filter[1].can_id = CAN_ID::R_Velocity;
    CAN_Filter[1].can_mask = CAN_SFF_MASK;
    CAN_Filter[2].can_id = CAN_ID::R_StartBreak;
    CAN_Filter[2].can_mask = CAN_SFF_MASK;
    CAN_Filter[3].can_id = CAN_ID::R_CleanPlot;
    CAN_Filter[3].can_mask = CAN_SFF_MASK;
    setsockopt(CAN_comData->s, SOL_CAN_RAW, CAN_RAW_FILTER, &CAN_Filter, sizeof(CAN_Filter));

}

bool CAN_Receiver::dataReceive()
{
    CAN_comData->nbytes = read(CAN_comData->s, &(CAN_comData->frame), sizeof(CAN_comData->frame));
    if(CAN_comData->nbytes > 0) {
        qDebug() << "receive " << CAN_comData->frame.can_id ;
        if(CAN_comData->frame.can_id == CAN_ID::R_Angle){
            for(int i = 0; i < 4; i++){
                CAN_comData->RT_data.uintData[7-i]=CAN_comData->frame.data[i+4];
                CAN_comData->RT_data.uintData[3-i]=CAN_comData->frame.data[i];
            }
            emit AngleSignal(CAN_comData->RT_data.floats.fl1, CAN_comData->RT_data.floats.fl2);
        }
        else if(CAN_comData->frame.can_id == CAN_ID::R_Velocity)
        {
            for(int i = 0; i < 4; i++){
                CAN_comData->RT_data.uintData[7-i]=CAN_comData->frame.data[i+4];
                CAN_comData->RT_data.uintData[3-i]=CAN_comData->frame.data[i];
            }

            emit VelocitySignal(CAN_comData->RT_data.floats.fl1, CAN_comData->RT_data.floats.fl2);
        }
        else if(CAN_comData->frame.can_id == CAN_ID::R_CleanPlot){

            emit CleanPlotSignal();
        }
        else if(CAN_comData->frame.can_id == CAN_ID::R_StartBreak){
            for(int i = 0; i < 8; ++i){
               CAN_comData->RT_data.uintData[i] = CAN_comData->frame.data[i];
            }
            //emit StartBreakReceived();
        }
        return true;
    }
    return false;
}



void CAN_Receiver::compareVars()
{

}

void CAN_Receiver::receiveAll(){
    while(1){
        dataReceive();
    }
}


