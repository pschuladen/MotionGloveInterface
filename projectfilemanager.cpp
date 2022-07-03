#include "projectfilemanager.h"

ProjectFileManager::ProjectFileManager(QObject *parent)
    : QObject{parent}
{

}

void ProjectFileManager::addSaveableObject(QObject *obj)
{

}

void ProjectFileManager::removeSaveableObject(QObject *obj)
{

}

void ProjectFileManager::increaseSaveObjectCounter(int inc)
{
    setSaveObjectCounter(m_saveObjectCounter + inc);
}

void ProjectFileManager::setXmlFilePath(QUrl fileUrl, bool loadIt)
{
    m_xmlPath = getFilePathNameFromUrl(fileUrl);
    m_docReadyRead = false;
    if(loadIt) {
        QFile _file(m_xmlPath);
        if(_file.exists()) {
            if(_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                if(xmlDoc.setContent(&_file)) {
                    QDomElement root = xmlDoc.firstChildElement();
                    if(root.tagName() == "ProcessElements") {
                        qDebug() << "file exists" << "and has has the right root element";
                        m_docReadyRead = true;
                        emit selectedXmlFileReadyLoad();
                    }
                }
                _file.close();
            }

            //            if(xmlDoc.childNodes()
        }
    }
}

void ProjectFileManager::prepareXmlDoc()
{
    xmlDoc.clear();
    QDomProcessingInstruction header = xmlDoc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"" );
    xmlDoc.appendChild( header );

    categoryElems.clear();
    singleElems.clear();

    createClearXmlElems();

    emit readyForWritingXml();
}

void ProjectFileManager::writeXmlFile(QUrl fileUrl)
{


//    QDomProcessingInstruction header = xmlDoc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"" );
//    xmlDoc.appendChild( header );

    QDomElement root = xmlDoc.createElement("ProcessElements");
    xmlDoc.appendChild(root);


    for(auto iter = singleElems.constBegin(); iter != singleElems.constEnd(); ++iter) {

        if(categoryElems.contains(iter->nodeType)) {
            categoryElems[iter->nodeType].appendChild(iter->qDomElement);
        }
        else {
            qWarning() << "unexpected nodetype category while saving project";
        }
    }

    for(TypeHelper::NodeType _cat: m_nodeCategories) {
        root.appendChild(categoryElems.value(_cat));
    }


    const QString _fpath = getFilePathNameFromUrl(fileUrl);
    QFile file(_fpath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "could not open file" << _fpath;
        return;
    }

    QTextStream stream(&file);

    stream << xmlDoc.toString();
    file.close();

    qInfo() << "Written file" << _fpath;



}

void ProjectFileManager::loadSceneFromCurrentFile()
{
    qDebug() << "loading scene from current file";
    if(m_docReadyRead) {
        typedef TypeHelper _t;

        const QList<_t::NodeType> _nodeCats = {_t::OscOutput,
                                               _t::OscInput,
                                               _t::Input,
                                               _t::Output,
                                               _t::Process,
                                               _t::Connection};

        for(_t::NodeType _type: _nodeCats) {
            extractNodesWithType(_type);
        }


//        QDomElement root = xmlDoc.firstChildElement();
//        qDebug() << xmlDoc.firstChildElement().tagName();

//        QDomElement _catOscOut = root.firstChildElement(TypeHelper::getPrefixForNodetype(TypeHelper::OscOutput));
//        if(!_catOscOut.isNull()) {
//            QDomNodeList _oscOutNodes = _catOscOut.childNodes();
//            for(int i = 0; i < _oscOutNodes.count(); i++) {
//                if(_oscOutNodes.at(i).isElement()) {
//                    QDomElement _ele = _oscOutNodes.at(i).toElement();
//                    qDebug() << _ele.tagName();
//                    emit qDomElemToScene(_ele, TypeHelper::OscOutput);
//                }
//            }
//        }
    }
    else {
        qWarning() << "xml-doc not ready for load";
    }
}

void ProjectFileManager::loadNodeCategoryFromCurrentFile(TypeHelper::NodeType type)
{
    qDebug() << "loading nodecategory from current file:" << type;


    if(m_docReadyRead) {
        extractNodesWithType(type);
    }
    else {
        qWarning() << "xml-doc not ready for load";
    }
}

void ProjectFileManager::valueConnectionElement(QString uniqueID, QString sourceId, int srcIdx, QString rcvId, int rcvIdx, TypeHelper::ValueType vType)
{
    QDomElement _vce = xmlDoc.createElement(uniqueID);
    _vce.setAttribute("sourceId", sourceId);
    _vce.setAttribute("srcIdx", srcIdx);
    _vce.setAttribute("receiverId", rcvId);
    _vce.setAttribute("rcvIdx", rcvIdx);
    _vce.setAttribute("valueType", vType);
    singleElems[uniqueID] = SingleElement(_vce, TypeHelper::Connection);
//    if(!categoryElems.contains(TypeHelper::Connection))
//        categoryElems[TypeHelper::Connection] = xmlDoc.createElement(TypeHelper::getPrefixForNodetype(TypeHelper::Connection));
//    categoryElems[TypeHelper::Connection].appendChild(_vce);
}

void ProjectFileManager::inputNodeElement(QString uniqueID, QString inputDevice, QString inputAddress)
{
    QDomElement _ine = xmlDoc.createElement(uniqueID);
    _ine.setAttribute("inputDevice", inputDevice);
    _ine.setAttribute("inputAddress", inputAddress);
    singleElems[uniqueID] = SingleElement(_ine, TypeHelper::Input);
//    if(!categoryElems.contains(TypeHelper::Input))
//        categoryElems[TypeHelper::Input] = xmlDoc.createElement(TypeHelper::getPrefixForNodetype(TypeHelper::Input));
//    categoryElems[TypeHelper::Input].appendChild(_ine);

}

void ProjectFileManager::outputNodeElement(QString uniqueID, QString outputDevice, quint16 outputIdx)
{
    QDomElement _oue = xmlDoc.createElement(uniqueID);
    _oue.setAttribute("ouputDevice", outputDevice);
    _oue.setAttribute("outputIdx", outputIdx);
    singleElems[uniqueID] = SingleElement(_oue, TypeHelper::Output);
}

void ProjectFileManager::processingNodeElement(QString uniqueID, TypeHelper::ProcessorType procType)
{
    QDomElement _pre = xmlDoc.createElement(uniqueID);
    _pre.setAttribute("processor-type", procType);
    singleElems[uniqueID] = SingleElement(_pre, TypeHelper::Process);
}

void ProjectFileManager::oscOutputdeviceElement(QString uniqueID)
{
    QDomElement _oscoute = xmlDoc.createElement(uniqueID);
    singleElems[uniqueID] = SingleElement(_oscoute, TypeHelper::OscOutput);
}

void ProjectFileManager::oscInputDeviceElement(QString uniqueID, QString deviceName)
{
    QDomElement _oscine = xmlDoc.createElement(uniqueID);
    _oscine.setAttribute("deviceName", deviceName);
    singleElems[uniqueID] = SingleElement(_oscine, TypeHelper::OscInput);
}

void ProjectFileManager::addSubNode(QString uniqueID, QString subtype, QVariantMap subMap)
{
    qDebug() << "received subnode for " << uniqueID << subtype;
    if(!singleElems.contains(uniqueID)) {
        qWarning() << "node with ID" << uniqueID << "wasn't created!";
        return;
    }

    QDomElement subNode = xmlDoc.createElement(subtype);
    for(auto iter = subMap.constBegin(); iter != subMap.constEnd(); ++iter) {
        qDebug() << "element key" << iter.key();
        QDomElement _el = xmlDoc.createElement(iter.key());


        if(qstrcmp(iter->typeName(), "QVariantList") == 0) {
            const QVariantList _list = iter->value<QVariantList>();
            qDebug() <<"list length for" << iter.key() << "is" << _list.length();

            for(auto subIter = _list.constBegin(); subIter != _list.constEnd(); ++subIter) {
                qDebug() << "subiter type" << subIter->typeName();
                if(qstrcmp(subIter->typeName(), "ProjectFileManager::SLI_element") == 0) {
                    SLI_element _subNodeMap = subIter->value<ProjectFileManager::SLI_element>();
                    QDomElement _subel = xmlDoc.createElement(_subNodeMap.cNodeName);
                    for(auto susubIter = _subNodeMap.attributes.constBegin();
                        susubIter != _subNodeMap.attributes.constEnd(); ++susubIter)
                    {
                        if(susubIter.value().canConvert<QString>()) {
//                        QVariant x = susubIter.value();
                            _subel.setAttribute(susubIter.key(), susubIter.value().toString());
                        }
                    }
                    _el.appendChild(_subel);
                }

                else if(subIter->canConvert<QString>()) {
                    _el.appendChild(xmlDoc.createElement(subIter->toString()));
                }
            }
        }

//        if(iter.value().canConvert<QList<QVariant>>()) {
//            qDebug() << "can convert to qlist" << iter.key();
//            for(auto subIter = iter->toList().constBegin(); subIter != iter->toList().constBegin(); ++subIter) {
//                if(subIter->canConvert<QString>()) {
//                    _el.appendChild(xmlDoc.createElement(subIter->toString()));
//                }
//            }
//        }
        else if(iter.value().canConvert<QString>()) {
            _el.appendChild(xmlDoc.createElement(iter.value().toString()));
        }
        else {
            qWarning() << "could not handle value for key" << iter.key() << iter.value();
        }
        subNode.appendChild(_el);
    }
    singleElems[uniqueID].qDomElement.appendChild(subNode);

    setSaveObjectCounter(m_saveObjectCounter - 1);

}

void ProjectFileManager::addSubtree(QString uniqueID, QDomDocument subNodeTreeDocument)
{
    QDomNode subNodeTree = subNodeTreeDocument.firstChild();
    if(subNodeTree.isNull()) return;

    if(singleElems.contains(uniqueID)) {
        singleElems[uniqueID].qDomElement.appendChild(xmlDoc.importNode(subNodeTree, true));
    }
    else {
        qWarning() << "node with ID" << uniqueID << "wasn't created!";
        return;
    }
    setSaveObjectCounter(m_saveObjectCounter -1);
}

void ProjectFileManager::testReceiveDomeNode(QDomNode _dom)
{
    qDebug() << "test receive domnode";
    if(_dom.isElement()) {
        QDomElement dom = _dom.toElement();
       qDebug() << dom.tagName();
    if(dom.hasAttributes()) {

    QDomNamedNodeMap _attrs =  dom.attributes();
    for(int i = 0; i < _attrs.count(); i++) {
        QDomNode _n = _attrs.item(i);
        if(_n.isAttr()) {
            QDomAttr _a = _n.toAttr();
            qDebug() << _a.name() << _a.value();
        }
    }
    QDomDocument _xdoc("temp");
    QDomElement  _troot = _xdoc.createElement("rooot");
    _xdoc.appendChild(_troot);
    _troot.appendChild(_xdoc.importNode(_dom, true));
    qDebug() << "print result xml";
    qDebug() << _xdoc.toString();

//    qDebug() << "has attribute" << dom.attributes();
}
    }
}

QString ProjectFileManager::getFilePathNameFromUrl(QUrl fileUrl)
{

    QString _furl = fileUrl.path();
    if(_furl.endsWith(".xml")) {
        return _furl;
    }
    else {
        return _furl.append(".xml");
    }
}

void ProjectFileManager::setSaveObjectCounter(int newSaveObjectCounter)
{
    qDebug() << "save object counter" << newSaveObjectCounter;
    m_saveObjectCounter = newSaveObjectCounter;
    if(m_saveObjectCounter <= 0) {
        qDebug() << "xml path is" << m_xmlPath;
        if(m_xmlPath != "") {
            writeXmlFile(m_xmlPath);
        }
    }
}

void ProjectFileManager::createClearXmlElems()
{
    for(TypeHelper::NodeType &_typ: m_nodeCategories) {
        categoryElems[_typ] = xmlDoc.createElement(TypeHelper::getPrefixForNodetype(_typ));
    }
}

void ProjectFileManager::extractNodesWithType(TypeHelper::NodeType type)
{
    QDomElement root = xmlDoc.firstChildElement();

    qDebug() << "looking for" << type;

    QDomElement _catNodes = root.firstChildElement(TypeHelper::getPrefixForNodetype(type));
    if(!_catNodes.isNull()) {
        qDebug() << "found category" << type;
        QDomNodeList _oscOutNodes = _catNodes.childNodes();
        for(int i = 0; i < _oscOutNodes.count(); i++) {
            if(_oscOutNodes.at(i).isElement()) {
                QDomElement _ele = _oscOutNodes.at(i).toElement();
                qDebug() << _ele.tagName();
                emit qDomElemToScene(_ele, type);
            }
        }
    }
    emit finishedSendingCategory(type);
}
