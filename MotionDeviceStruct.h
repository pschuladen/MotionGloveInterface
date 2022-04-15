#ifndef MOTIONDEVICESTRUCT_H
#define MOTIONDEVICESTRUCT_H

#include <QString>
#include "devicedatainput.h"

struct MotionDevice {
    QString deviceName;
    QString address;
    uint16_t port;
    uint16_t sendIntervall;
    bool connectStatus;
    DeviceDataInput *inputHandler;
//    uint16_t numSensors;
};

#endif // MOTIONDEVICESTRUCT_H
