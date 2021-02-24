#include "canclass.h"

typedef union{
    unsigned char uint8t[4];
    float fl;
} uintFloat;

uintFloat myUintFloat;

CAN_Class::CAN_Class(QWidget *parent) : QWidget(parent)
{
    canSetUp();
}
/*
void CAN_Class::brakeSwitch()
{
    unsigned int brakeState;
    printf(" 1.Start\n 2.Stop\n Enter: ");
    scanf("%u", &brakeState);
    switch(brakeState){
    case 1:
        break;
    case 2:
        brakeState = 0;
        break;
    }
    frame.can_id = 0x125;
    frame.can_dlc = 1;
    frame.data[0] = brakeState;
    printf("Brake state = %d\r\n", frame.data[0]);
    nbytes = write(s, &frame, sizeof(frame));
    if(nbytes != sizeof(frame)) {
    printf("Send Error frame[0]!\r\n");
    //system("sudo ifconfig vcan0 down");
    }
}
void CAN_Class::floatTransmit(void){
       //5.Set send data
    frame.can_id = 0x126;
    frame.can_dlc = 8;

    printf("can_id  = 0x%X\r\n", frame.can_id);
    printf("can_dlc = %d\r\n", frame.can_dlc);

    for(int i = 0; i<4; i++)
        frame.data[i] = myUintFloat.uint8t[3-i];

    nbytes = write(s, &frame, sizeof(frame));
        if(nbytes != sizeof(frame)) {
            printf("Send Error frame[0]!\r\n");
        }
}*/
void CAN_Class::dataTransmit(double *per, double *variable){
    for(int i = 0; i<1; ++i){
    for(int j = 0; j <= 360; ++j){
        usleep((*per)*1000/360*1000);
        //printf("~~~\n");
        frame.data[0] = *variable;
        printf("data[%d] = %d\r\n", j, frame.data[0]);

        nbytes = write(s, &frame, sizeof(frame));
        if(nbytes != sizeof(frame)) {
        printf("Send Error frame[0]!\r\n");
        //system("sudo ifconfig vcan0 down");
        }
    }
    }
}

void CAN_Class::dataTransmit(double per, double variable, double var2){
    for(int i = 0; i<1; ++i){
    for(int j = 0; j <= 360; ++j){
        usleep((per)*1000/360*1000);
        printf("~~~\n");
        myUintFloat.floats.fl1 = variable;
        myUintFloat.floats.fl2 = var2;
        for(int i = 0; i < 4; i++){
            frame.data[i+4] = myUintFloat.uint8t[7-i];
            frame.data[i] = myUintFloat.uint8t[3-i];
        }
        nbytes = write(s, &frame, sizeof(frame));
        if(nbytes != sizeof(frame)) {
        printf("Send Error frame[0]!\r\n");
        //system("sudo ifconfig vcan0 down");
        }
    }
    }
}
int CAN_Class::canSetUp(void){
    memset(&frame, 0, sizeof(struct can_frame));

    printf("this is a can send demo\r\n");

    //1.Create socket
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) {
        perror("socket PF_CAN failed");
        return 1;
    }

    //2.Specify can0 device
    strcpy(ifr.ifr_name, "vcan0");
    ret = ioctl(s, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        perror("ioctl failed");
        return 1;
    }

    //3.Bind the socket to can0
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind failed");
        return 1;
    }

    //4.Disable filtering rules, do not receive packets, only send
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

    frame.can_id = 0x125;
    frame.can_dlc = 8;
    for(int i = 0; i < 8; i++){
        frame.data[i] = 0;
        myUintFloat.uint8t[i]=0;
    }

    return 0;
}
int CAN_Class::canSetDown(){
    printf("sleeeep\n");
    UNISTD_OVERRIDE::myCloseCan(s);
    return 0;
}
