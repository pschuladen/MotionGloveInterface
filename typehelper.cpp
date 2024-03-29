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
    case TypeHelper::IntValue:
        return QColor("lime");
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
    case TypeHelper::BoolList:
        return QColor("pink");
    }
    return QColor();

}

const QString TypeHelper::getStringForValueType(const TypeHelper::ValueType vtype)
{
    switch(vtype) {
    case TypeHelper::IntValue:
        return QString("intvalue");
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
    case TypeHelper::BoolList:
        return QString("boolList");
    }
}

QUrl TypeHelper::getQmlUrlForProcessorType(ProcessorType procType)
{
    switch(procType) {
    case TypeHelper::NoteSelector:
        return QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_NoteSelectorView.qml"));
    case TypeHelper::TouchRecognizer:
        return QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_TouchRecognizerView.qml"));
        break;
    case TypeHelper::SplitComponents:
        return QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_SplitComponentsView.qml"));
        break;
    case TypeHelper::DiffValue:
        return QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_DiffValueView.qml"));
        break;
    case TypeHelper::AbsValue:
        return QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_AbsValueView.qml"));
        break;
    case TypeHelper::SumComponents:
        return QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_SumComponentsView.qml"));
        break;
    case TypeHelper::GenericProcessor:
        return QUrl("");
        break;
    case TypeHelper::Scale:
        return QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_ScaleView.qml"));
        break;
    default:
        return QUrl("");
    }
}

QString TypeHelper::getPrefixForNodetype(NodeType nodeType)
{
    switch(nodeType){
    case TypeHelper::Connection:
        return "connection";
    case TypeHelper::OscInput:
        return "oscinput";
    case TypeHelper::OscOutput:
        return "oscoutput";
    case TypeHelper::Generic:
        return "gen";
    case TypeHelper::Gui:
        return "gui";
    case TypeHelper::Input:
        return "input";
    case TypeHelper::Process:
        return "process";
    case TypeHelper::Output:
        return "output";
    case TypeHelper::Audio:
        return "audio";
    case TypeHelper::Audiocontroller:
        return "aucon";
    }
}

size_t TypeHelper::getSizeForValueType(const ValueType vType)
{
    switch(vType) {
    case TypeHelper::IntValue:
        return 1;
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
    case TypeHelper::BoolList:
        return 6;
    }
}

QString TypeHelper::getNameForValueComponent(ValueType vType, int idx)
{
    switch (vType) {
    case TypeHelper::Undefined:
        return "undef";
        break;
    case TypeHelper::Vector:
        switch(idx) {
        case 0:
            return "x";
        case 1:
            return "y";
        case 2:
            return "z";
        default:
            return "vec";
        }
        break;
    case TypeHelper::Quat:
        switch(idx) {
        case 0:
            return "x";
        case 1:
            return "y";
        case 2:
            return "z";
        case 3:
            return "w";
        default:
            return "qu";
        }
        break;
    case TypeHelper::List:
        return "v"+ QString::number(idx);
        break;
    case TypeHelper::SingleValue:
        return "val";
        break;
    case TypeHelper::BoolValue:
        return "b";
        break;
    case TypeHelper::Trigger:
        return "t";
        break;
    default:
        return "v";
        break;
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
