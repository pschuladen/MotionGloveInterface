#include "pn_splitcomponents.h"

PN_SplitComponents::PN_SplitComponents(QObject *parent)
    : ProcessNode{parent}
{
    processRole = ViewController;
    subProcessor.append(PN_SplitComponents::createSubprocessor(identifier()));
    appendToConnectedTypes(TypeHelper::Undefined);
}

PN_SplitComponents::PN_SplitComponents(QByteArray identifier, int idxInControlller, PN_SplitComponents *controller, TypeHelper::ValueType type, quint16 valueNumber, QObject *parent)
    : ProcessNode{identifier, idxInControlller, type, valueNumber, parent}
{
    m_processorType = TypeHelper::SplitComponents;
    m_supportsSubValues = true;
    processRole = Processor;
}


ProcessNode *PN_SplitComponents::createSubprocessor(QString objectname_id)

{
    PN_SplitComponents *newSubprocessor = new PN_SplitComponents(identifier(), subProcessor.size(), this);
    connect(this, &QObject::destroyed, newSubprocessor, &QObject::deleteLater);
    emit newSubprocessorWasCreated(newSubprocessor);
    return newSubprocessor;
}


ValueNotifierClass *PN_SplitComponents::getNotifier(int idx)
{
    if(processRole == ViewController)
        return subProcessor.first()->getNotifier(idx);
    else if(processRole == Processor)
        return subNotifier.value(idx, nullptr);
    else
        return nullptr;
}


void PN_SplitComponents::deleteSubprocessorAtIdx(quint16 idx)
{
}


void PN_SplitComponents::initSaveData()
{
    QDomDocument _doc("tmpdoc");
    QDomElement root = _doc.createElement("processor");
    root.setAttribute("nSubprocessors", subProcessor.count());
    QDomElement _params = _doc.createElement("parameter");

    _params.setAttribute("connectedType", connectedTypes().first());
    root.appendChild(_params);
    _doc.appendChild(root);
    //    qDebug() << "send sumcom data";
    connect(this, &ProcessNode::sendSubNodeTree, projManager::Instance(), &ProjectFileManager::addSubtree, Qt::SingleShotConnection);
    emit sendSubNodeTree(identifier(), _doc);
}

void PN_SplitComponents::loadDataFromQdomElement(QDomElement domElement)
{
    QDomElement _paras = domElement.firstChildElement("parameter");
    if(!_paras.isNull()) {
        int _valTypeInt = _paras.attribute("connectedType", "0").toInt();
        TypeHelper::ValueType _valType = static_cast<TypeHelper::ValueType>(_valTypeInt);

        subProcessor.first()->setConnectedValueType(_valType);
        setConnectedTypesAtIdx(0, _valType);


    }
    else {
        qWarning() << "pn_scale: no parameter found in file";
    }
    emit didFinishLoad(identifier());
}
