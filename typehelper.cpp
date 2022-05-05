#include "typehelper.h"

TypeHelper::TypeHelper()
{

}

const TypeHelper::ValueType TypeHelper::valueTypeForSensor(SensorType styp)
{
//    return sensorToValueTypeMap.value(styp);
    switch (styp) {
    case SensorType::Accel:
        return ValueType::Vector;
    case SensorType::Gyro:
        return ValueType::Vector;
    case SensorType::Grav:
        return ValueType::Vector;
    case SensorType::RotQuat:
        return ValueType::Quat;
    case SensorType::Touch:
        return ValueType::List;
    case SensorType::Custom:
        return ValueType::Undefined;
    }
}
