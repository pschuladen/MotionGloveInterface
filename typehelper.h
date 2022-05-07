#ifndef TYPEHELPER_H
#define TYPEHELPER_H

#include <QtCore>
#include <QColor>

struct TypeHelper
{
    Q_GADGET
public:
    TypeHelper();

    enum SensorType: quint8 {Custom, Accel, Gyro, Grav, RotQuat, Touch};
    Q_ENUM(SensorType);

    enum ValueType: quint8 { // TODO: add touchvalues as type?
        Undefined, Vector, Quat, List,
        SingleValue, BoolValue, Trigger
    };
    Q_ENUM(ValueType);
    static const ValueType valueTypeForSensor(SensorType styp);
    Q_INVOKABLE static QColor getColorForValueType(TypeHelper::ValueType vtype);


    enum NodeConnectionType: quint8 {
        InputToProcess,
        ProcessToProcess,
        InputToOutput,
        InputToAudio,
        ProcessToOutput,
        ProcessToAudio
    };
    Q_ENUM(NodeConnectionType);

    enum NodeType: quint8 {
        Input, Process, Output, Audio
    };
    Q_ENUM(NodeType);
};

struct TouchValues {
    quint8 tval[6];
    quint8 nvals = 6;
};

#endif // TYPEHELPER_H
