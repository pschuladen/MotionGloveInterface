#include "inputvalueviewcontroller.h"

InputValueViewController::InputValueViewController(QObject *parent)
    : ValueNotifierClass{parent}
{
}

TypeHelper::SensorType InputValueViewController::viewmode() const
{
    return m_viewmode;
}

void InputValueViewController::setViewmode(TypeHelper::SensorType newViewmode)
{
    if (m_viewmode == newViewmode)
        return;
    m_viewmode = newViewmode;

    switch(newViewmode) {
    case TypeHelper::Custom:
        break;
    case TypeHelper::Accel:
        setValname({"X", "Y", "Z"});
        m_values.resize(3);
        setMaxvalue(100);
        setMinvalue(-100);
        break;
    case TypeHelper::Gyro:
        setValname({"X", "Y", "Z"});
        m_values.resize(3);
        setMaxvalue(100);
        setMinvalue(-100);
        break;
    case TypeHelper::Grav:
        setValname({"X", "Y", "Z"});
        m_values.resize(3);
        setMaxvalue(1);
        setMinvalue(-1);
        break;
    case TypeHelper::RotQuat:
        setValname({"X", "Y", "Z", "W"});
        m_values.resize(4);
        setMaxvalue(1);
        setMinvalue(-1);
        break;
    case TypeHelper::Touch:
        setValname({"t1", "t2", "t3", "t4", "t5", "t6"});
        m_values.resize(6);
        setMaxvalue(100);
        setMinvalue(0);
        break;
    }
    setDotColor(TypeHelper::getColorForValueType(TypeHelper::valueTypeForSensor(newViewmode)));//modeColorMap.value(newViewmode, QColor("white")));
    setValueCount(m_values.size());

//    qInfo() << "new viewmode" << newViewmode;
    emit viewmodeChanged();
}

int InputValueViewController::valueCount() const
{
    return m_valueCount;
}


const QList<float> &InputValueViewController::values() const
{
    return m_values;
}

void InputValueViewController::setValues(const QList<float> &newValues)
{
    if (m_values == newValues)
        return;
    m_values = newValues;
    checkAndEmitValuesChanged();
//    emit valuesChanged();
}

const QList<QString> &InputValueViewController::valname() const
{
    return m_valname;
}

void InputValueViewController::setValname(const QList<QString> &newValname)
{
    if (m_valname == newValname)
        return;
    m_valname = newValname;
    emit valnameChanged();
}

const QString &InputValueViewController::nodeIdentifier() const
{
    return m_nodeIdentifier;
}

void InputValueViewController::setNodeIdentifier(const QString &newNodeIdentifier)
{
    if (m_nodeIdentifier == newNodeIdentifier)
        return;
    m_nodeIdentifier = newNodeIdentifier;
    emit nodeIdentifierChanged();
}

void InputValueViewController::slot_vectorChanged(QVector3D vec, int frame)
{
//    qInfo() << "vector changed" << vec;
    m_values[0] = vec.x();
    m_values[1] = vec.y();
    m_values[2] = vec.z();
    checkAndEmitValuesChanged();
}

void InputValueViewController::slot_quatChanged(QQuaternion newQuat, int frame)
{
    m_values[0] = newQuat.x();
    m_values[1] = newQuat.y();
    m_values[2] = newQuat.z();
    m_values[3] = newQuat.scalar();
    checkAndEmitValuesChanged();
}

void InputValueViewController::slot_touchChanged(QList<float> touch, int frame)
{
    setValues(touch);
}

void InputValueViewController::slot_valuesChanged(QList<float> newValues, int frame)
{
    setValues(newValues);
}

void InputValueViewController::checkAndEmitValuesChanged()
{
    if(emitvalues()) {
        emit viewValuesChanged();
    }
}

void InputValueViewController::setValueCount(int newValueCount)
{
    if (m_valueCount == newValueCount)
        return;
    m_valueCount = newValueCount;
    emit valueCountChanged();
}

float InputValueViewController::minvalues() const
{
    return m_minvalue;
}

void InputValueViewController::setMinvalue(float newMinvalue)
{
    if (qFuzzyCompare(m_minvalue, newMinvalue))
        return;
    m_minvalue = newMinvalue;
    emit minvalueChanged();
}

float InputValueViewController::maxvalues() const
{
    return m_maxvalue;
}

void InputValueViewController::setMaxvalue(float newMaxvalue)
{
    if (qFuzzyCompare(m_maxvalue, newMaxvalue))
        return;
    m_maxvalue = newMaxvalue;
    emit maxvalueChanged();
}

bool InputValueViewController::emitvalues() const
{
    return m_emitvalues;
}

void InputValueViewController::setEmitvalues(bool newEmitvalues)
{
//    qInfo() << "backend emitvalues set" << newEmitvalues;
    if (m_emitvalues == newEmitvalues)
        return;
    m_emitvalues = newEmitvalues;
    emit emitvaluesChanged();
}

const QColor &InputValueViewController::dotColor() const
{
    return m_dotColor;
}

void InputValueViewController::setDotColor(const QColor &newDotColor)
{
    if (m_dotColor == newDotColor)
        return;
    m_dotColor = newDotColor;
    emit dotColorChanged();
}

ValueNotifierClass *InputValueViewController::sourceNotifier() const
{
    return m_sourceNotifier;
}

void InputValueViewController::setSourceNotifier(ValueNotifierClass *newSourceNotifier)
{
    if(m_sourceNotifier != nullptr) {
        ValueNotifierClass::disconnectValueTypeSignalToSlot(m_sourceNotifier, this, TypeHelper::valueTypeForSensor(viewmode()));
    }
    m_sourceNotifier = newSourceNotifier;
    ValueNotifierClass::connectValueTypeSignalToSlot(newSourceNotifier, this, TypeHelper::valueTypeForSensor(viewmode()));
}


ValueNotifierClass *InputValueViewController::getNotifier(int idx)
{
    return m_sourceNotifier;
}
