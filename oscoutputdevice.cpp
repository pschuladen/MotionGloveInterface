#include "oscoutputdevice.h"

OscOutputDevice::OscOutputDevice(QObject *parent)
{
    setParent(parent);
    socket = new QUdpSocket(this);

//    QByteArray pongBuf(100, char());

//    OSCPP::Client::Packet pongPacket(pongBuf.data(), 100);

    //    QString sstr("/123456789012345");
//    qInfo() << "string size" << sstr.size();
//    pongPacket.openMessage(sstr.toUtf8(), 9)
//            .int32(123)
//            .float32(1.23)
//            .float32(2.34)
//            .float32(3.45)
//            .float32(4.45).float32(4.45).float32(4.45).float32(4.45).float32(4.45)
//            .closeMessage();
//    size_t pongSize = pongPacket.size();
//    qInfo() << "message size" << pongSize;
//    qInfo() << "buffer:" << pongBuf;
//    qint64 sendedBytes = socket->writeDatagram(pongBuf.data(), pongSize, QHostAddress(QHostAddress::LocalHost), 55110);

}

bool OscOutputDevice::setViewControllerObject(OscViewController *vc)
{
    if (vc == nullptr) return false;

    m_viewController = vc;

    typedef OscViewController ovc;
    typedef OscOutputDevice ood;

    connect(vc, &ovc::destIpChanged, this, &ood::setDestIp);
    connect(vc, &ovc::destPortChanged, this, &ood::setDestPort);
    connect(vc, &ovc::oscPathsChanged, this, &ood::setOscPaths);
    connect(vc, &ovc::oscPathAtIndexChanged, this, &ood::setOscPathAtIndex);
    connect(vc, &ovc::oscPathAdded, this, &ood::setOscPathAtIndex);
    connect(vc, &ovc::valueTypesChanged, this, &ood::setValueTypes);

    connect(this, &ovc::destIpChanged, vc, &ood::setDestIp);
    connect(this, &ovc::destPortChanged, vc, &ood::setDestPort);
    connect(this, &ovc::oscPathsChanged, vc, &ood::setOscPaths);
    connect(this, &ovc::oscPathAtIndexChanged, vc, &ood::setOscPathAtIndex);
    connect(this, &ovc::oscPathAdded, vc, &ood::setOscPathAtIndex);
    connect(this, &ovc::valueTypesChanged, vc, &ood::setValueTypes);

//    connect(viewController, &OscViewController)

    return true;
}

void OscOutputDevice::addOscPath()
{
    OscViewController::addOscPath(QString("/out%1").arg(nPaths()+1));
}


