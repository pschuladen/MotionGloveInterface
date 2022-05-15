#include "oscoutputdevice.h"

OscOutputDevice::OscOutputDevice(QObject *parent)
{
    setParent(parent);
    initialiseOscDevice();


//        QString sstr("/123456789012345");
////    qInfo() << "string size" << sstr.size();
//    pongPacket.openMessage(sstr.toUtf8(), 9)
//            .int32(123)
//            .float32(1.23)
//            .float32(2.34)
//            .float32(3.45)
//            .float32(4.45).float32(4.45).float32(4.45).float32(4.45).float32(4.45)
//            .closeMessage();
//pongPacket.reset();
//    size_t pongSize = pongPacket.size();
//    qInfo() << "message size" << pongSize;
//    qInfo() << "buffer:" << pongBuf;
//    qint64 sendedBytes = socket->writeDatagram(pongBuf.data(), pongSize, QHostAddress(QHostAddress::LocalHost), 55110);

}

OscOutputDevice::OscOutputDevice(OscOutputViewController *viewController, QObject *parent)
    : OscOutputViewController{parent}
{
//    setParent(parent);
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

//    m_destPort = 55211;
//    m_destinationAddress.setAddress(QHostAddress::LocalHost);
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
//    QHostAddress address(newDestIp);
//    if (QAbstractSocket::IPv4Protocol == address.protocol())
//    {
//       qDebug("Valid IPv4 address.");
//    }
//    else if (QAbstractSocket::IPv6Protocol == address.protocol())
//    {
//       qDebug("Valid IPv6 address.");
//    }
//    else
//    {
//        emit destIpChanged(destIp());
//        return;
//        qDebug("Unknown or invalid address.");
//    }
    if(newDestIp == destIp()) {
    if(m_destinationAddress.toString() == newDestIp) {
            return;
        }

    }
    if(m_destinationAddress.setAddress(newDestIp)) {
//        setOscDestIp(newDestIp);
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
    connect(newOsc, &OscPacketBuilder::oscMessageBufferReady,this, &OscOutputDevice::sendOscMsgBuffer);
    packetBuilder.append(newOsc);
    m_oscPaths.append(newPath);
    m_valueTypes.append(TypeHelper::Undefined);
    emit valueTypesChanged(valueTypes());
    emit oscPathsChanged(oscPaths());
//    outputPaths.append(OscOutput(newPath.toUtf8(), newOsc));
}

void OscOutputDevice::slot_addOscPath()
{
    qInfo() << "adding osc path";
//    QString newPathAddress = QString("/out/%1").arg(outputPaths.size()+1);
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
    qInfo() << "got new Conenction at index" << idx << valueType;
    if(idx < packetBuilder.size() && idx < valueTypes().size()) {
        m_valueTypes.replace(idx, valueType);
        emit valueTypesChanged(valueTypes());
    }
}

void OscOutputDevice::sendOscMsgBuffer(const QByteArray &oscBuffer, size_t msgSize, int frame)
{
//    qInfo() << "attempt to send data to" << destinationAddress() << destPort() << oscBuffer;
    qint64 _seby = socket->writeDatagram(oscBuffer.data(), msgSize, destinationAddress(), destPort());
    if (_seby < msgSize) qWarning() << "Osc message send incomplete in output socket";
}


ValueNotifierClass *OscOutputDevice::getNotifier(int idx)
{
//    qInfo() <<"oscdevice asked for notifier. " << packetBuilder;
    if (idx < packetBuilder.size()) return packetBuilder.at(idx);
    else return nullptr;
}
