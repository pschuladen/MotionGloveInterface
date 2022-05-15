#include "oscoutputviewcontroller.h"

OscOutputViewController::OscOutputViewController(QObject *parent)
    : ValueNotifierClass{parent}
{
//    setParent(parent);
    setObjectName(standardObjectName()); //should maybe just be set in qml?
}

const QString &OscOutputViewController::destIp() const
{
    return m_destIp;
}

void OscOutputViewController::setDestIp(const QString &newDestIp)
{
    //TODO: check if matches ip address, regex
    if (m_destIp == newDestIp)
        return;
    m_destIp = newDestIp;
    emit destIpChanged(newDestIp);
}

quint16 OscOutputViewController::destPort() const
{
    return m_destPort;
}

void OscOutputViewController::setDestPort(quint16 newDestPort)
{
    if(newDestPort < 1025 || 65534 < newDestPort) return;
    if (m_destPort == newDestPort)
        return;
    m_destPort = newDestPort;
    emit destPortChanged(newDestPort);
}

const QList<QString> &OscOutputViewController::oscPaths() const
{
    return m_oscPaths;
}

void OscOutputViewController::setOscPaths(const QList<QString> &newOscPaths)
{
    qInfo() << "controller receiving new path" << newOscPaths;
    if (m_oscPaths == newOscPaths)
        return;
    m_oscPaths = newOscPaths;
    setNPaths(newOscPaths.size());
    m_valueTypes.resize(newOscPaths.size());
    emit oscPathsChanged(newOscPaths);
}

void OscOutputViewController::addOscPath(QString newPath)
{
    qInfo() << "new osc path" << newPath;
    if(!newPath.startsWith('/')) return ;

    m_oscPaths.append(newPath);
    m_valueTypes.resize(m_oscPaths.size());
    setNPaths(m_oscPaths.size());

    emit oscPathAdded(newPath);
}

void OscOutputViewController::setOscPathAtIndex(QString newPath, quint32 idx)
{
    qInfo() << "path changed at idx" << idx << "to" << newPath;
    if(!newPath.startsWith('/') && idx < m_oscPaths.size()) return;

    m_oscPaths.replace(idx, newPath);
    emit oscPathAtIndexChanged(newPath, idx);
}

void OscOutputViewController::newConnectionAtIndex(int idx, TypeHelper::ValueType valueType)
{

}

int OscOutputViewController::nPaths() const
{
    return m_nPaths;
}

void OscOutputViewController::setNPaths(int newNPaths)
{
    if (m_nPaths == newNPaths)
        return;
    m_nPaths = newNPaths;
    emit nPathsChanged();
}

const QList<TypeHelper::ValueType> &OscOutputViewController::valueTypes() const
{
    return m_valueTypes;
}

void OscOutputViewController::setValueTypes(const QList<TypeHelper::ValueType> &newValueTypes)
{
    qInfo() << "setting value types from" << valueTypes() << "to" << newValueTypes;
    if (m_valueTypes == newValueTypes)
        return;
    m_valueTypes = newValueTypes;
    emit valueTypesChanged(newValueTypes);
}

const QString OscOutputViewController::standardObjectName()
{
    return QString("oscviewcontroller");
}

void OscOutputViewController::addOscPath()
{
    emit sig_addOscPath();
}
