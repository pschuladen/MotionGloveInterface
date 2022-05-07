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

QColor TypeHelper::getColorForValueType(ValueType vtype)
{

    switch (vtype) {
    case TypeHelper::Undefined:
        return QColor("white");
    case TypeHelper::Vector:
        return QColor("red");
    case TypeHelper::Quat:
        return QColor("blue");
    case TypeHelper::List:
        return QColor("yellow");
    case TypeHelper::SingleValue:
        return QColor("green");
    case TypeHelper::BoolValue:
        return QColor("deepskyblue");
    case TypeHelper::Trigger:
        return QColor("darkred");
    }
    return QColor();

}
