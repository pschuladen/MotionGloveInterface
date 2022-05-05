#ifndef TYPEHELPER_H
#define TYPEHELPER_H

#include <QtCore>

struct TypeHelper
{
    Q_GADGET
public:
    TypeHelper();

    enum SensorType: quint8 {Custom, Accel, Gyro, Grav, RotQuat, Touch};
    Q_ENUM(SensorType);

    enum ValueType: quint8 { // TODO: add touchvalues
        Undefined, Vector, Quat, List,
        SingleValue, BoolValue, Trigger
    };
    Q_ENUM(ValueType);

    static const ValueType valueTypeForSensor(SensorType styp);


};

struct TouchValues {
    quint8 tval[6];
    quint8 nvals = 6;
};

#endif // TYPEHELPER_H
