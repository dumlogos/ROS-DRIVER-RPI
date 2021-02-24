#ifndef CAN_THREAD_H
#define CAN_THREAD_H

#include <QThread>

class CAN_Thread :  public QThread{
Q_OBJECT
public:
    void run(){
        dataTransmit(double per, double variable, double variable2);
    }

signals:

};

#endif // CAN_THREAD_H
