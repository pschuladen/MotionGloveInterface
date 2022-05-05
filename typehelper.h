#ifndef TYPEHELPER_H
#define TYPEHELPER_H

#include <QtCore>
//#include <QMap>

struct TypeHelper
{
    Q_GADGET
public:
    TypeHelper();

    enum SensorType {Custom, Accel, Gyro, Grav, RotQuat, Touch};
    Q_ENUM(SensorType);

    enum ValueType {
        Undefined, Vector, Quat, List,
        SingleValue, BoolValue, Trigger
    };
    Q_ENUM(ValueType);

    static const ValueType valueTypeForSensor(SensorType styp);

//    const QMap<SensorType, ValueType> sensorToValueTypeMap = {
//        {SensorType::Custom, ValueType::Undefined},
//        {SensorType::Accel, ValueType::Vector},
//        {SensorType::Gyro, ValueType::Vector},
//        {SensorType::Grav, ValueType::Vector},
//        {SensorType::RotQuat, ValueType::Quat},
//        {SensorType::Touch, ValueType::List}
//    };


};

#endif // TYPEHELPER_H
