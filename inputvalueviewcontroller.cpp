#include "inputvalueviewcontroller.h"

InputValueViewController::InputValueViewController(QObject *parent)
    : QObject{parent}
{


}

InputValueViewController::ValueViewMode InputValueViewController::viewmode() const
{
    return m_viewmode;
}

void InputValueViewController::setViewmode(ValueViewMode newViewmode)
{
    if (m_viewmode == newViewmode)
        return;
    m_viewmode = newViewmode;

    switch(newViewmode) {
    case ValueViewMode::Custom:
        break;
    case ValueViewMode::Accel:
        setValname({"X", "Y", "Z"});
        m_values.resize(3);
        setMaxvalue(100);
        setMinvalue(-100);
        break;
    case ValueViewMode::Gyro:
        setValname({"X", "Y", "Z"});
        m_values.resize(3);
        setMaxvalue(100);
        setMinvalue(-100);
        break;
    case ValueViewMode::Grav:
        setValname({"X", "Y", "Z"});
        m_values.resize(3);
        setMaxvalue(1);
        setMinvalue(-1);
        break;
    case ValueViewMode::Quat:
        setValname({"X", "Y", "Z", "W"});
        m_values.resize(4);
        setMaxvalue(1);
        setMinvalue(-1);
        break;
    case ValueViewMode::Touch:
        setValname({"t1", "t2", "t3", "t4", "t5", "t6"});
        m_values.resize(6);
        setMaxvalue(100);
        setMinvalue(0);
        break;
    }
    setDotColor(modeColorMap.value(newViewmode, QColor("white")));
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

void InputValueViewController::vectorChanged(QVector3D vec)
{
//    qInfo() << "vector changed" << vec;
    m_values[0] = vec.x();
    m_values[1] = vec.y();
    m_values[2] = vec.z();
    checkAndEmitValuesChanged();
}

void InputValueViewController::quatChanged(QQuaternion newQuat)
{
    m_values[0] = newQuat.x();
    m_values[1] = newQuat.y();
    m_values[2] = newQuat.z();
    m_values[3] = newQuat.scalar();
    checkAndEmitValuesChanged();
}

void InputValueViewController::touchChanged(QList<float> touch)
{
    setValues(touch);
}

void InputValueViewController::newValues(QList<float> newValues)
{
    setValues(newValues);
}

void InputValueViewController::checkAndEmitValuesChanged()
{
    if(emitvalues()) {
        emit valuesChanged();
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

//int InputValueViewController::valuecount() const
//{
//    return m_valuecount;
//}

//void InputValueViewController::setValuecount(int newValuecount)
//{
//    if (m_valuecount == newValuecount)
//        return;
//    m_valuecount = newValuecount;
//    emit valuecountChanged();
//}

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