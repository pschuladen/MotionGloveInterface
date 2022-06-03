#include "oscoutputdevice.h"

OscOutputDevice::OscOutputDevice(QObject *parent)
{
    setParent(parent);
    initialiseOscDevice();
}

OscOutputDevice::OscOutputDevice(OscOutputViewController *viewController, QObject *parent)
    : OscOutputViewController{parent}
{
    setViewControllerObject(viewController);
    initialiseOscDevice();
}

bool OscOutputDevice::setViewControllerObject(OscOutputViewController *vc)
{
    if (vc == nullptr) return false;

    m_viewController = vc;

    typedef OscOutputViewController ovc;
    typedef OscOutputDevice ood;

    connect(vc, &ovc::destIpChanged, this, &ood::setDestIp);
    connect(vc, &ovc::destPortChanged, this, &ood::setDestPort);
    connect(vc, &ovc::oscPathsChanged, this, &ood::setOscPaths);
    connect(vc, &ovc::oscPathAtIndexChanged, this, &ood::setOscPathAtIndex);
//    connect(vc, &ovc::oscPathAdded, this, &ood::setOscPathAtIndex);
    connect(vc, &ovc::valueTypesChanged, this, &ood::setValueTypes);
    connect(vc, &ovc::sig_addOscPath, this, &ood::slot_addOscPath);

    connect(this, &ood::destIpChanged, vc, &ovc::setDestIp);
    connect(this, &ood::destPortChanged, vc, &ovc::setDestPort);
    connect(this, &ood::oscPathsChanged, vc, &ovc::setOscPaths);
    connect(this, &ood::oscPathAtIndexChanged, vc, &ovc::setOscPathAtIndex);
//    connect(this, &ood::sig_addOscPath, vc, &ovc::addOscPath);
    connect(this, &ood::valueTypesChanged, vc, &ovc::setValueTypes);

    return true;
}

void OscOutputDevice::initialiseOscDevice()
{
    socket = new QUdpSocket(this);

    QByteArray pongBuf(100, char());

    OSCPP::Client::Packet pongPacket(pongBuf.data(), 100);

    OscOutputDevice::setDestPort(55211);
    OscOutputDevice::setDestIp("127.0.0.1");
}

const QHostAddress &OscOutputDevice::destinationAddress() const
{
    return m_destinationAddress;
}

void OscOutputDevice::addOscPath()
{
    addOscPath(QString("/out%1").arg(nPaths()+1));
}




void OscOutputDevice::setDestIp(const QString &newDestIp)
{
    if(newDestIp == destIp()) {
    if(m_destinationAddress.toString() == newDestIp) {
            return;
        }

    }
    if(m_destinationAddress.setAddress(newDestIp)) {
        m_destIp = newDestIp;
    }
    emit destIpChanged(destIp());
}

void OscOutputDevice::setDestPort(quint16 newDestPort)
{
    if(0 < newDestPort && 65534 > newDestPort && newDestPort > 1024) {
        if(newDestPort == m_destPort) return;
        m_destPort = newDestPort;
    }
    emit destPortChanged(destPort());
}

void OscOutputDevice::setOscPaths(const QList<QString> &newOscPaths)
{
    if(oscPaths() == newOscPaths) return;
    m_oscPaths = newOscPaths;

    for (int i = 0; i<oscPaths().size(); i++) {
        packetBuilder.at(i)->setOscAddress(oscPaths().at(i).toUtf8());
    }

    emit oscPathsChanged(oscPaths());

}

void OscOutputDevice::setValueTypes(const QList<TypeHelper::ValueType> &newValueTypes)
{
    if (newValueTypes == valueTypes()) return;
    m_valueTypes = newValueTypes;
    emit valueTypesChanged(valueTypes());
}

void OscOutputDevice::addOscPath(QString newPath)
{
    OscPacketBuilder *newOsc = new OscPacketBuilder(newPath.toUtf8(), packetBuilder.size(), this);
    connect(newOsc, &OscPacketBuilder::gotNewConnectionWithType, this, &OscOutputDevice::newConnectionAtIndex);
    connect(newOsc, &OscPacketBuilder::oscMessageBufferReady, this, &OscOutputDevice::sendOscMsgBuffer);
    packetBuilder.append(newOsc);
    m_oscPaths.append(newPath);
    m_valueTypes.append(TypeHelper::Undefined);
    emit valueTypesChanged(valueTypes());
    emit oscPathsChanged(oscPaths());
}

void OscOutputDevice::slot_addOscPath()
{
    addOscPath(QString("/out/%1").arg(packetBuilder.size()+1));
}

void OscOutputDevice::setOscPathAtIndex(QString newPath, quint32 idx)
{
    if(idx >= oscPaths().size() ||oscPaths().at(idx) == newPath) return;
    m_oscPaths.replace(idx, newPath);//[idx] = newPath;
    packetBuilder.at(idx)->setOscAddress(newPath.toUtf8());
    emit oscPathsChanged(oscPaths());
}

void OscOutputDevice::newConnectionAtIndex(int idx, TypeHelper::ValueType valueType)
{
    if(idx < packetBuilder.size() && idx < valueTypes().size()) {
        m_valueTypes.replace(idx, valueType);
        emit valueTypesChanged(valueTypes());
    }
}

void OscOutputDevice::sendOscMsgBuffer(const QByteArray &oscBuffer, size_t msgSize, int frame)
{
    qint64 _seby = socket->writeDatagram(oscBuffer.data(), msgSize, destinationAddress(), destPort());
    if (_seby < msgSize) qWarning() << "Osc message send incomplete in output socket";
}


ValueNotifierClass *OscOutputDevice::getNotifier(int idx)
{
    if (idx < packetBuilder.size()) return packetBuilder.at(idx);
    else return nullptr;
}
