#include "typehelper.h"

TypeHelper::TypeHelper()
{

}

const TypeHelper::ValueType TypeHelper::valueTypeForSensor(const TypeHelper::SensorType styp)
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

const QColor TypeHelper::getColorForValueType(const TypeHelper::ValueType vtype)
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

const QString TypeHelper::getStringForValueType(const TypeHelper::ValueType vtype)
{
    switch(vtype) {
    case TypeHelper::Undefined:
        return QString("undef");
    case TypeHelper::Vector:
        return QString("vect");
    case TypeHelper::Quat:
        return QString("quat");
    case TypeHelper::List:
        return QString("list");
    case TypeHelper::SingleValue:
        return QString("value");
    case TypeHelper::BoolValue:
        return QString("bool");
    case TypeHelper::Trigger:
        return QString("trig");
    }
}

QString TypeHelper::getPrefixForNodetype(NodeType nodeType)
{
    switch(nodeType){
    case TypeHelper::Connection:
        return "con";
    case TypeHelper::OscInput:
        return "oscin";
    case TypeHelper::OscOutput:
        return "oscout";
    case TypeHelper::Generic:
        return "gen";
    case TypeHelper::Gui:
        return "gui";
    case TypeHelper::Input:
        return "in";
    case TypeHelper::Process:
        return "proc";
    case TypeHelper::Output:
        return "out";
    case TypeHelper::Audio:
        return "audio";
    case TypeHelper::Audiocontroller:
        return "aucon";
    }
}

size_t TypeHelper::getSizeForValueType(const ValueType vType)
{
    switch(vType) {
    case TypeHelper::Undefined:
        return 0;
    case TypeHelper::Vector:
        return 3;
    case TypeHelper::Quat:
        return 4;
    case TypeHelper::List:
        return 12;
    case TypeHelper::SingleValue:
        return 1;
    case TypeHelper::BoolValue:
        return 1;
    case TypeHelper::Trigger:
        return 1;
    }
}

QString TypeHelper::getProtoStrForSensor(const SensorType sensType)
{
    switch(sensType) {
    case TypeHelper::Custom:
        return "sensor";
    case TypeHelper::Accel:
        return "acc";
    case TypeHelper::Gyro:
        return "gyr";
    case TypeHelper::Grav:
        return "gravity";
    case TypeHelper::RotQuat:
        return "quat";
    case TypeHelper::Touch:
        return "touch";
    }
}
