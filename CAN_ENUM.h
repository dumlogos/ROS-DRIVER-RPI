#ifndef CAN_ENUM_H
#define CAN_ENUM_H

enum CAN_ID{
    All,
    T_Angle = 0x123,
    T_Velocity,
    T_StartBreak,
    T_AngleProportionalRatio,
    T_AngleDifferentialRatio,
    T_AngleIntegralRatio,
    T_VelocityProportionalRatio,
    T_VelocityDifferentialRatio,
    T_VelocityIntegralRatio,
    T_ClearPosition = 0x12C,
    T_CleanPlot = 0x12F,
    R_Angle = 0x133,
    R_Velocity,
    R_StartBreak,
    R_CleanPlot = 0x13F
};

enum DriverState{
   STOP,
   START
};

enum CAN_IFace{
  VCAN,
  CAN0
};



#endif // CAN_ENUM_H
