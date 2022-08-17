#ifndef TYPEHELPER_H
#define TYPEHELPER_H

#include <QtCore>
#include <QColor>
#include <QStringList>

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
        Generic, Gui, Input, Process, Output, Audio, Audiocontroller, OscInput, OscOutput, Connection
    };
    Q_ENUM(NodeType);

    enum ProcessorType {
        GenericProcessor, Scale, SumComponents, AbsValue, DiffValue
    };
    Q_ENUM(ProcessorType);

    static QUrl getQmlUrlForProcessorType(ProcessorType procType);

    static QString getPrefixForNodetype(NodeType nodeType);

    static size_t getSizeForValueType(const TypeHelper::ValueType vType);

    static QString getProtoStrForSensor(const TypeHelper::SensorType sensType);

//    static QString generateIdForType(TypeHelper::NodeType nodeType);


};

struct TouchValues {
    quint8 tval[6];
    quint8 nvals = 6;
};




#endif // TYPEHELPER_H
