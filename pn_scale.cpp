#include "pn_scale.h"

PN_Scale::PN_Scale(QObject *parent)
    : ProcessNode{parent}
{
    m_processorType = TypeHelper::Scale;
    m_inHigh = 1;
    m_inLow = 0;
    m_outHigh = 1;
    m_outLow = 0;
    m_clipOutput = false;
    evalMultip();
}

PN_Scale::PN_Scale(QByteArray identifier, int idxInControlller, PN_Scale *controller,
                   TypeHelper::ValueType type, quint16 valueNumber, QObject *parent)
    : ProcessNode{identifier, idxInControlller, type, valueNumber, parent},
      m_clipOutput{controller->clipOutput()},
      m_inHigh{controller->inHigh()}, m_inLow{controller->inLow()},
      m_outHigh{controller->outHigh()}, m_outLow{controller->outLow()}
{
    m_processorType = TypeHelper::Scale;
    evalMultip();
    connectPropertiesToProcessor(controller, this);
}

float PN_Scale::inLow() const
{
    return m_inLow;
}

void PN_Scale::setInLow(float newInLow)
{
    if (qFuzzyCompare(m_inLow, newInLow))
        return;
    m_inLow = newInLow;
    evalMultip();
    emit inLowChanged(newInLow);
}

float PN_Scale::inHigh() const
{
    return m_inHigh;
}

void PN_Scale::setInHigh(float newInHigh)
{
    if (qFuzzyCompare(m_inHigh, newInHigh))
        return;
    m_inHigh = newInHigh;
    evalMultip();
    emit inHighChanged(newInHigh);
}

float PN_Scale::outLow() const
{
    return m_outLow;
}

void PN_Scale::setOutLow(float newOutLow)
{
    if (qFuzzyCompare(m_outLow, newOutLow))
        return;
    m_outLow = newOutLow;
    evalMultip();
    emit outLowChanged(newOutLow);
}

float PN_Scale::outHigh() const
{
    return m_outHigh;
}

void PN_Scale::setOutHigh(float newOutHigh)
{
    if (qFuzzyCompare(m_outHigh, newOutHigh))
        return;
    m_outHigh = newOutHigh;
    evalMultip();
    emit outHighChanged(newOutHigh);
}

//int PN_Scale::newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, int atIdx, quint16 nValuesInList)
//{
//    if(!acceptsInputType(type)) return -1;

////    if(atIdx < 0 ) atIdx = subProcessor.size();


////    while(atIdx + 1 > subProcessor.size()) {
////        PN_Scale *newSubprocessor = new PN_Scale(identifier() , subProcessor.size(), this, type, nValuesInList);
////        connect(this, &QObject::destroyed, newSubprocessor, &QObject::deleteLater);
////        appendToConnectedTypes(TypeHelper::Undefined);
////        subProcessor.append(newSubprocessor);
////        emit newSubprocessorWasCreated(newSubprocessor);
////    }

////    connect(this, &PN_Scale::connectionRequestFromSender, subProcessor[atIdx], &PN_Scale::setConnectionFromSender, Qt::SingleShotConnection);
////    emit connectionRequestFromSender(sender, type, nValuesInList);

//    return atIdx;


////    qDebug() << "newconnection subprocessors" << this << subProcessor;


////    if(newSubprocessor->setConnectionFromSender(sender, type, nValuesInList)) {
//////        subProcessor.append(newSubprocessor);

////        emit newSubprocessorWasCreated(newSubprocessor);
////        return subProcessor.size() -1;
////    }
////    else {
////        newSubprocessor->deleteLater();
////        return -1;
////    }
//}

bool PN_Scale::connectToSubProcessorAtIndex(int index, TypeHelper::ValueType type, quint16 nValuesInList)
{
    return false;
}

ProcessNode *PN_Scale::createProcessControl(QString objectname_id)
{
    PN_Scale *scale = new PN_Scale();
    scale->setObjectName(objectname_id+"-processcontroller");
    setInitialProperties(this, scale);
    connectPropertiesToProcessor(this, scale);
    return qobject_cast<ProcessNode*>(scale);
}

ProcessNode *PN_Scale::createSubprocessor(QString objectname_id)
{
    PN_Scale *newSubprocessor = new PN_Scale(identifier() , subProcessor.size(), this);
    connect(this, &QObject::destroyed, newSubprocessor, &QObject::deleteLater);
//    subProcessor.append(newSubprocessor);
    emit newSubprocessorWasCreated(newSubprocessor);
    return newSubprocessor;
}

float PN_Scale::process(float value)
{
    qDebug() << "processing" << value << this;
    if(clipOutput())
        return qBound(outLow(), ((value - inLow()) * multi()) + outLow(), outHigh());

    else
        return ((value-inLow()) * multi()) + outLow();

//        float result = ((value - inLow()) * multi()) + outLow();
//        if(result < outLow()) return outLow();
//        else if(result > outHigh()) return outHigh();
//        else return result;
}

quint8 PN_Scale::process(quint8 value)
{
    return value;
}

void PN_Scale::evalMultip()
{
    float inputDiff = inHigh()-inLow();
    if(inputDiff==0) inputDiff = 1;
    setMulti((outHigh()-outLow()) / (inputDiff));
}

float PN_Scale::multi() const
{
    return m_multi;
}



void PN_Scale::setMulti(float newMulti)
{
    m_multi = newMulti;
}

bool PN_Scale::acceptsInputType(TypeHelper::ValueType typ, int atIdx) const
{
        if(typ == TypeHelper::Undefined) return false;
        else return true;
}

void PN_Scale::connectPropertiesToProcessor(PN_Scale *propertyMaster, PN_Scale *propertySlave)
{
//TODO: differentiate between ProcessorRoles
    connect(propertyMaster, &PN_Scale::inLowChanged, propertySlave, &PN_Scale::setInLow);
    connect(propertyMaster, &PN_Scale::inHighChanged, propertySlave, &PN_Scale::setInHigh);
    connect(propertyMaster, &PN_Scale::outLowChanged, propertySlave, &PN_Scale::setOutLow);
    connect(propertyMaster, &PN_Scale::outHighChanged, propertySlave, &PN_Scale::setOutHigh);
    connect(propertyMaster, &PN_Scale::clipOutputChanged, propertySlave, &PN_Scale::setClipOutput);
    connect(propertySlave, &PN_Scale::connectedTypeAtIdxChanged, propertyMaster, &PN_Scale::setConnectedTypesAtIdx);

    // has to be in another way...
//    connect(propertySlave, &PN_Scale::connectedTypesChanged, propertyMaster, &PN_Scale::setConnectedTypes);
}

void PN_Scale::setInitialProperties(PN_Scale *propertyMaster, PN_Scale *propertySlave)
{
//    propertySlave->setProperty()
//TODO: differentiate between ProcessorRoles
    propertySlave->setInLow(propertyMaster->inLow());
    propertySlave->setInHigh(propertyMaster->inHigh());
    propertySlave->setOutLow(propertyMaster->outLow());
    propertySlave->setOutHigh(propertyMaster->outHigh());
    propertySlave->setClipOutput(propertyMaster->clipOutput());
}

void PN_Scale::initSaveData()
{
    QDomDocument _doc("tmpdoc");
    QDomElement root = _doc.createElement("processor");
    root.setAttribute("nSubprocessors", subProcessor.count());
    _doc.appendChild(root);
    QDomElement _params = _doc.createElement("parameter");
    _params.setAttribute("inLow", inLow());
    _params.setAttribute("inHigh", inHigh());
    _params.setAttribute("outLow", outLow());
    _params.setAttribute("outHigh", outHigh());
    _params.setAttribute("clipOutput", clipOutput());
    root.appendChild(_params);

    connect(this, &ProcessNode::sendSubNodeTree, projManager::Instance(), &ProjectFileManager::addSubtree);
    emit sendSubNodeTree(identifier(), _doc);
}

void PN_Scale::loadDataFromQdomElement(QDomElement domElement)
{

    QDomElement _paras = domElement.firstChildElement("parameter");
    if(!_paras.isNull()) {
        const QList<QString> _propNames = {"inLow", "outHigh", "outLow", "inHigh", "clipOutput"};
        bool _paraOk;
        for(const QString &_prop: _propNames) {
            float _val = _paras.attribute(_prop, "0").toFloat(&_paraOk);
            if(_paraOk) {
                qDebug() << "setting property" << _prop;
                if(!setProperty(_prop.toLocal8Bit(), _val)) {
                    qWarning() << "Property" << _prop << "for object" << identifier() << processorType() << "not properly set!";
                }
            }
        }
    }
    emit didFinishLoad(identifier());
}

bool PN_Scale::clipOutput() const
{
    return m_clipOutput;
}

void PN_Scale::setClipOutput(bool newClipOutput)
{
    if (m_clipOutput == newClipOutput)
        return;
    m_clipOutput = newClipOutput;
    emit clipOutputChanged(newClipOutput);
}
