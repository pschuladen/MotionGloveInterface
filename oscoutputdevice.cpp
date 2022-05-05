#include "oscoutputdevice.h"

OscOutputDevice::OscOutputDevice(QObject *parent)
    : QObject{parent}
{
    socket = new QUdpSocket(this);

    QByteArray pongBuf(100, char());
    OSCPP::Client::Packet pongPacket(pongBuf.data(), 100);
    QString sstr("/123456789012345");

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


