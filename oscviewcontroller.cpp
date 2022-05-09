#include "oscviewcontroller.h"

OscViewController::OscViewController(QObject *parent)
{
    setParent(parent);
    setObjectName(standardObjectName());
}

const QString &OscViewController::destIp() const
{
    return m_destIp;
}

void OscViewController::setDestIp(const QString &newDestIp)
{
    //TODO: check if matches ip address, regex
    if (m_destIp == newDestIp)
        return;
    m_destIp = newDestIp;
    emit destIpChanged(newDestIp);
}

int OscViewController::destPort() const
{
    return m_destPort;
}

void OscViewController::setDestPort(int newDestPort)
{
    if(newDestPort < 1025 || 65534 < newDestPort) return;
    if (m_destPort == newDestPort)
        return;
    m_destPort = newDestPort;
    emit destPortChanged(newDestPort);
}

const QList<QString> &OscViewController::oscPaths() const
{
    return m_oscPaths;
}

void OscViewController::setOscPaths(const QList<QString> &newOscPaths)
{
    if (m_oscPaths == newOscPaths)
        return;
    m_oscPaths = newOscPaths;
    setNPaths(newOscPaths.size());
    m_valueTypes.resize(newOscPaths.size());
    emit oscPathsChanged(newOscPaths);
}

void OscViewController::addOscPath(QString newPath)
{
    qInfo() << "new osc path" << newPath;
    if(!newPath.startsWith('/')) return ;

    m_oscPaths.append(newPath);
    m_valueTypes.resize(m_oscPaths.size());
    setNPaths(m_oscPaths.size());

    emit oscPathAdded(newPath);
}

void OscViewController::setOscPathAtIndex(QString newPath, quint32 idx)
{
    qInfo() << "path changed at idx" << idx << "to" << newPath;
    if(!newPath.startsWith('/') && idx < m_oscPaths.size()) return;

    m_oscPaths.replace(idx, newPath);
    emit oscPathAtIndexChanged(newPath, idx);

}

void OscViewController::newConnectionAtIndex(int idx, TypeHelper::ValueType valueType)
{

}




int OscViewController::nPaths() const
{
    return m_nPaths;
}

void OscViewController::setNPaths(int newNPaths)
{
    if (m_nPaths == newNPaths)
        return;
    m_nPaths = newNPaths;
    emit nPathsChanged();
}

const QList<TypeHelper::ValueType> &OscViewController::valueTypes() const
{
    return m_valueTypes;
}

void OscViewController::setValueTypes(const QList<TypeHelper::ValueType> &newValueTypes)
{
    if (m_valueTypes == newValueTypes)
        return;
    m_valueTypes = newValueTypes;
    emit valueTypesChanged();
}

const QString OscViewController::standardObjectName()
{
return QString("oscviewcontroller");
}

void OscViewController::addOscPath()
{
    emit sig_addOscPath();
}
