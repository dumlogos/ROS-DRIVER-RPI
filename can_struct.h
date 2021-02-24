#ifndef CAN_STRUCT_H
#define CAN_STRUCT_H

#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

struct CAN_Struct{
    typedef union{
        unsigned char uintData[8];
        struct floatsM{
            float fl1;
            float fl2;
        } floats;
        struct floatIntM{
            float fl;
            int in;
        } floatInt;
    } CAN_RT_Data;
    CAN_RT_Data RT_data;
    int ret;
    int s, nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
};


#endif // CAN_STRUCT_H
