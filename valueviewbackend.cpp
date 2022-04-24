#include "valueviewbackend.h"

ValueViewBackend::ValueViewBackend(QObject *parent)
    : QObject{parent}
{


}

ValueViewBackend::ValueViewMode ValueViewBackend::viewmode() const
{
    return m_viewmode;
}

void ValueViewBackend::setViewmode(ValueViewMode newViewmode)
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
    setValueCount(m_values.size());

//    qInfo() << "new viewmode" << newViewmode;
    emit viewmodeChanged();
}

int ValueViewBackend::valueCount() const
{
    return m_valueCount;
}


const QList<float> &ValueViewBackend::values() const
{
    return m_values;
}

void ValueViewBackend::setValues(const QList<float> &newValues)
{
    if (m_values == newValues)
        return;
    m_values = newValues;
    checkAndEmitValuesChanged();
//    emit valuesChanged();
}

const QList<QString> &ValueViewBackend::valname() const
{
    return m_valname;
}

void ValueViewBackend::setValname(const QList<QString> &newValname)
{
    if (m_valname == newValname)
        return;
    m_valname = newValname;
    emit valnameChanged();
}

const QString &ValueViewBackend::nodeIdentifier() const
{
    return m_nodeIdentifier;
}

void ValueViewBackend::setNodeIdentifier(const QString &newNodeIdentifier)
{
    if (m_nodeIdentifier == newNodeIdentifier)
        return;
    m_nodeIdentifier = newNodeIdentifier;
    emit nodeIdentifierChanged();
}

void ValueViewBackend::vectorChanged(QVector3D vec)
{
//    qInfo() << "vector changed" << vec;
    m_values[0] = vec.x();
    m_values[1] = vec.y();
    m_values[2] = vec.z();
    checkAndEmitValuesChanged();
}

void ValueViewBackend::quatChanged(QQuaternion newQuat)
{
    m_values[0] = newQuat.x();
    m_values[1] = newQuat.y();
    m_values[2] = newQuat.z();
    m_values[3] = newQuat.scalar();
    checkAndEmitValuesChanged();
}

void ValueViewBackend::touchChanged(QList<float> touch)
{
    setValues(touch);
}

void ValueViewBackend::newValues(QList<float> newValues)
{
    setValues(newValues);
}

void ValueViewBackend::checkAndEmitValuesChanged()
{
    if(emitvalues()) {
        emit valuesChanged();
    }
}

void ValueViewBackend::setValueCount(int newValueCount)
{
    if (m_valueCount == newValueCount)
        return;
    m_valueCount = newValueCount;
    emit valueCountChanged();
}

float ValueViewBackend::minvalues() const
{
    return m_minvalue;
}

void ValueViewBackend::setMinvalue(float newMinvalue)
{
    if (qFuzzyCompare(m_minvalue, newMinvalue))
        return;
    m_minvalue = newMinvalue;
    emit minvalueChanged();
}

float ValueViewBackend::maxvalues() const
{
    return m_maxvalue;
}

void ValueViewBackend::setMaxvalue(float newMaxvalue)
{
    if (qFuzzyCompare(m_maxvalue, newMaxvalue))
        return;
    m_maxvalue = newMaxvalue;
    emit maxvalueChanged();
}

bool ValueViewBackend::emitvalues() const
{
    return m_emitvalues;
}

void ValueViewBackend::setEmitvalues(bool newEmitvalues)
{
//    qInfo() << "backend emitvalues set" << newEmitvalues;
    if (m_emitvalues == newEmitvalues)
        return;
    m_emitvalues = newEmitvalues;
    emit emitvaluesChanged();
}
