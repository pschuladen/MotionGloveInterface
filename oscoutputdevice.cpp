#include "oscoutputdevice.h"

OscOutputDevice::OscOutputDevice(QObject *parent)
    : OscOutputViewController{parent}
{
    initialiseOscDevice();
}

OscOutputDevice::OscOutputDevice(QString uniqueID, OscOutputViewController *viewController, QObject *parent)
    : OscOutputViewController{uniqueID, parent}
{
    setViewControllerObject(viewController);
//    initialiseOscDevice();

//    ProjectFileManager *pm = projManager::Instance();

//    connect(this, &OscOutputDevice::registerSaveableObject, pm, &ProjectFileManager::addSaveableObject);
//    connect(this, &QObject::destroyed, pm, &ProjectFileManager::removeSaveableObject);
//    connect(this, &OscOutputDevice::sendPropertyMapElement, pm, &ProjectFileManager::addSubNode);
//    connect(this, &OscOutputDevice::announceAdditionalData, pm, &ProjectFileManager::increaseSaveObjectCounter);
//    emit registerSaveableObject(this);
}

bool OscOutputDevice::setViewControllerObject(OscOutputViewController *vc)
{
    if (vc == nullptr) return false;

    m_viewController = vc;

    typedef OscOutputViewController ovc;
    typedef OscOutputDevice ood;

    connect(vc, &ovc::sig_changeIp, this, &ood::setDestIp);
    connect(vc, &ovc::sig_changePort, this, &ood::setDestPort);
//    connect(vc, &ovc::oscPathsChanged, this, &ood::setOscPaths);
    connect(vc, &ovc::oscPathAtIndexChanged, this, &ood::setOscPathAtIndex);
//    connect(vc, &ovc::oscPathAdded, this, &ood::setOscPathAtIndex);
//    connect(vc, &ovc::valueTypesChanged, this, &ood::setValueTypes);
    connect(vc, &ovc::sig_addOscPath, this, &ood::slot_addOscPath);

    connect(vc, &ovc::sig_deleteOscPathAtIdx, this, &ood::slot_deleteOscPathAtIdx);

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
    qDebug() << "setting oscpaths" << newOscPaths;

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


void OscOutputDevice::slot_deleteOscPathAtIdx(int idx)
{
    if(idx+1 > packetBuilder.size()) return;
    packetBuilder.at(idx)->deleteLater();
    packetBuilder.removeAt(idx);
    m_oscPaths.removeAt(idx);
    m_valueTypes.removeAt(idx);
    emit oscPathsChanged(oscPaths());
    emit valueTypesChanged(valueTypes());
    emit sig_deleteOscOutputView(uniqueID(), idx);
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

void OscOutputDevice::sendOscMsgBuffer(const QByteArray oscBuffer, size_t msgSize, int frame)
{
    qint64 _seby = socket->writeDatagram(oscBuffer.data(), msgSize, destinationAddress(), destPort());
    if (_seby < msgSize) qWarning() << "Osc message send incomplete in output socket";
}

void OscOutputDevice::getNotifierRequest(int atIdx, QString nodeId)
{
    emit sig_sendNotiferPtr(getNotifier(atIdx), nodeId);
}


ValueNotifierClass *OscOutputDevice::getNotifier(int idx)
{
    qDebug() << "OSCoutputdevice notifier" << packetBuilder;
    if (idx < packetBuilder.size()) return packetBuilder.at(idx);
    else return nullptr;
}


void OscOutputDevice::initSaveData()
{
    qDebug() << "this data should be saved" << m_destinationAddress.toString() << m_destPort;
    qDebug() << "oscPaths" << oscPaths();
    typedef ProjectFileManager::SLI_element _sliEl;

    QDomDocument _doc("tmpdoc");
    QDomElement root = _doc.createElement("device-settings");
    _doc.appendChild(root);
    QDomElement dest = _doc.createElement("destinations");
    root.appendChild(dest);

    // multiple destinations?
    QDomElement addr = _doc.createElement("address");
    dest.appendChild(addr);
    addr.setAttribute("port", m_destPort);
    addr.setAttribute("ip", m_destinationAddress.toString());

    QDomElement _oscPaths = _doc.createElement("oscPaths");
    for(auto iter = oscPaths().constBegin(); iter!=oscPaths().constEnd(); ++iter) {

    }
    for(const QString &_oaddr: oscPaths()) {
        QDomElement _onePath = _doc.createElement("osc");
        _onePath.setAttribute("path", _oaddr);
        _oscPaths.appendChild(_onePath);
    }
    root.appendChild(_oscPaths);
    connect(this, &OscOutputDevice::sendSubNodeTree, projManager::Instance(), &ProjectFileManager::addSubtree, Qt::SingleShotConnection);
    emit sendSubNodeTree(uniqueID(), _doc);

/*
    QVariantMap _pMap;
    QVariantList _addrList;
    _addrList.append(QVariant::fromValue<_sliEl>(_sliEl("address",
                                                              {{"ip", QVariant(m_destinationAddress.toString())},
                                                               {"port", QVariant(m_destPort)}
                                                        })));
    _pMap["destinations"] = _addrList;

    QVariantList _pathVarList;
    for(const QString &_oaddr: oscPaths()) {
        _pathVarList.append(QVariant::fromValue<_sliEl>(_sliEl("osc",
                                                        {{"path", QVariant(_oaddr)}})));//QVariantMap({{"oscOutPath", QVariant(_oaddr)}}));
    }
    _pMap[ps_oscPaths] = _pathVarList;//QVariant(QVariantList(m_oscPaths));

    connect(this, &OscOutputDevice::sendPropertyMapElement, projManager::Instance(), &ProjectFileManager::addSubNode, Qt::SingleShotConnection);
    emit sendPropertyMapElement(uniqueID(), "device", _pMap);
    */
}



void OscOutputDevice::loadDataFromQdomElement(QDomElement domElement)
{
    QDomElement _destinations = domElement.firstChildElement("destinations");
    if(!_destinations.isNull()) {
        QDomNodeList _addrs = _destinations.elementsByTagName("address");
        for(int i = 0; i < _addrs.count(); i++) {
            if(_addrs.at(i).isElement()) {
                QDomElement _add = _addrs.at(i).toElement();
                if(_add.hasAttribute("ip")) {
                   QDomAttr _ip = _add.attributeNode("ip");
                   setDestIp(_ip.value());
                   qDebug() << "setting ip" << _ip.value();
                }
                if(_add.hasAttribute("port")) {
                    QDomAttr _port = _add.attributeNode("port");
                    bool _suc;
                    quint16 _uintport = _port.value().toUInt(&_suc);
                    if(_suc) setDestPort(_uintport);
                }
            }
        }
    }
    QDomElement _oscPaths = domElement.firstChildElement("oscPaths");
    if(!_oscPaths.isNull()) {
        QDomNodeList _paths = _oscPaths.elementsByTagName("osc");
        qDebug() << "paths found" << _paths.count();
        qDebug() << "children of" << _oscPaths.tagName() << _oscPaths.childNodes().length();
        for(int i = 0; i < _paths.count(); i++) {
            if(_paths.at(i).isElement()) {
                QDomElement _pat = _paths.at(i).toElement();
                if(_pat.hasAttribute("path")) {
                    QDomAttr _opat = _pat.attributeNode("path");
                    addOscPath(_opat.value());
                }
            }
        }
    }
    emit didFinishLoad(uniqueID());
}


