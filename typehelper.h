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

    Q_INVOKABLE static const TypeHelper::ValueType valueTypeForSensor(const TypeHelper::SensorType styp);
    Q_INVOKABLE static const QColor getColorForValueType(const TypeHelper::ValueType vtype);
    Q_INVOKABLE static const QString getStringForValueType(const TypeHelper::ValueType vtype);

    static size_t getSizeForValueType(const TypeHelper::ValueType vType);


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
        Generic, Input, Process, Output, Audio
    };
    Q_ENUM(NodeType);
};

struct TouchValues {
    quint8 tval[6];
    quint8 nvals = 6;
};

#endif // TYPEHELPER_H
