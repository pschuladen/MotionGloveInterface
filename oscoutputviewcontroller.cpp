#include "oscoutputviewcontroller.h"

OscOutputViewController::OscOutputViewController(QObject *parent)
    : ValueNotifierClass{parent}
{
    setObjectName(standardObjectName()); //should maybe just be set in qml?
}

OscOutputViewController::OscOutputViewController(QString uniqueID, QObject *parent)
    : ValueNotifierClass{parent}, m_uniqueID{uniqueID}
{
    setObjectName(standardObjectName());
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
    if (m_oscPaths == newOscPaths)
        return;
    m_oscPaths = newOscPaths;
    setNPaths(newOscPaths.size());
    m_valueTypes.resize(newOscPaths.size());
    emit oscPathsChanged(newOscPaths);
}

void OscOutputViewController::addOscPath(QString newPath)
{
    if(!newPath.startsWith('/')) return ;

    m_oscPaths.append(newPath);
    m_valueTypes.resize(m_oscPaths.size());
    setNPaths(m_oscPaths.size());

    emit oscPathAdded(newPath);
}

void OscOutputViewController::setOscPathAtIndex(QString newPath, quint32 idx)
{
    if(!newPath.startsWith('/') && idx < m_oscPaths.size()) return;

    m_oscPaths.replace(idx, newPath);
    emit oscPathAtIndexChanged(newPath, idx);
}

void OscOutputViewController::newConnectionAtIndex(int idx, TypeHelper::ValueType valueType)
{
    qWarning() << "WARNING! newConnectionAtIndex not implemented";
}

void OscOutputViewController::viewAtIndexHovered(bool hovered, int idx)
{
    if(hovered) setIndexHovered(idx);
    else setIndexHovered(-1);

    emit sig_viewAtIndexHovered(hovered, idx);
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
    if (m_valueTypes == newValueTypes)
        return;
    m_valueTypes = newValueTypes;
    emit valueTypesChanged(newValueTypes);
}

const QString OscOutputViewController::standardObjectName()
{
    return QString("oscviewcontroller");
}

bool OscOutputViewController::nodeAtIndexWasCreated(int idx)
{
    return m_createdOutputNodes.contains(idx);
}

void OscOutputViewController::nodeWithIdxWasCreated(int idx)
{
    m_createdOutputNodes.insert(idx);
}

void OscOutputViewController::removeNodeWithIdx(int idx)
{
    m_createdOutputNodes.remove(idx);
}

void OscOutputViewController::addOscPath()
{
    emit sig_addOscPath();
}


//ValueNotifierClass *OscOutputViewController::getNotifier(int idx)
//{
//}

const QString &OscOutputViewController::uniqueID() const
{
    return m_uniqueID;
}

void OscOutputViewController::setUniqueID(const QString &newUniqueID)
{
    if (m_uniqueID == newUniqueID)
        return;
    m_uniqueID = newUniqueID;
    emit uniqueIDChanged();
}

const QList<bool> &OscOutputViewController::outputNodeCreated() const
{
    return m_outputNodeCreated;
}

void OscOutputViewController::setOutputNodeCreated(const QList<bool> &newOutputNodeCreated)
{
    if (m_outputNodeCreated == newOutputNodeCreated)
        return;
    m_outputNodeCreated = newOutputNodeCreated;
    emit outputNodeCreatedChanged();
}

const QSet<int> &OscOutputViewController::createdOutputNodes() const
{
    return m_createdOutputNodes;
}

void OscOutputViewController::setCreatedOutputNodes(const QSet<int> &newCreatedOutputNodes)
{
    if (m_createdOutputNodes == newCreatedOutputNodes)
        return;
    m_createdOutputNodes = newCreatedOutputNodes;
    emit createdOutputNodesChanged();
}

int OscOutputViewController::indexHovered() const
{
    return m_indexHovered;
}

void OscOutputViewController::setIndexHovered(int newIndexHovered)
{
    if (m_indexHovered == newIndexHovered)
        return;
    m_indexHovered = newIndexHovered;
    emit indexHoveredChanged();
}
