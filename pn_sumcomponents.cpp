#include "pn_sumcomponents.h"

PN_SumComponents::PN_SumComponents(QObject *parent)
    : ProcessNode{parent}
{

}

PN_SumComponents::PN_SumComponents(QByteArray identifier, int idxInControlller, PN_SumComponents *controller, TypeHelper::ValueType type, quint16 valueNumber, QObject *parent)
    : ProcessNode{identifier, idxInControlller, type, valueNumber, parent}
{
    m_processorType = TypeHelper::SumComponents;
}


void PN_SumComponents::slot_quatChanged(QQuaternion quat, int frame)
{
    emit singleValueChanged(quat.scalar()+quat.x()+quat.y()+quat.z(), frame);
}

void PN_SumComponents::slot_vectorChanged(QVector3D vect, int frame)
{
    emit singleValueChanged(vect.x()+vect.y()+vect.z(), frame);
}

void PN_SumComponents::slot_touchChanged(QList<float> touch, int frame)
{
    float sum = 0;
    for(const float &x: touch) {
        sum += x;
    }
    emit singleValueChanged(sum, frame);
}

void PN_SumComponents::slot_valuesChanged(QList<float> values, int frame)
{
    float sum = 0;
    for(const float &x: values) {
        sum += x;
    }
    emit singleValueChanged(sum, frame);
}

void PN_SumComponents::slot_singleValueChanged(float value, int frame)
{
    emit singleValueChanged(value, frame);
}

void PN_SumComponents::slot_boolValueChanged(bool value, int frame)
{
    emit singleValueChanged((float)value, frame);
}

void PN_SumComponents::slot_trigger(int frame)
{
    emit triggerActivated(frame);
}

ProcessNode *PN_SumComponents::createSubprocessor(QString objectname_id)
{
    PN_SumComponents *newSubprocessor = new PN_SumComponents(identifier(), subProcessor.size(), this);
    connect(this, &QObject::destroyed, newSubprocessor, &QObject::deleteLater);
    emit newSubprocessorWasCreated(newSubprocessor);
    return newSubprocessor;
}

//float PN_SumComponents::process(float value)
//{
//}

//quint8 PN_SumComponents::process(quint8 value)
//{
//}


void PN_SumComponents::initSaveData()
{
    QDomDocument _doc("tmpdoc");
    QDomElement root = _doc.createElement("processor");
    root.setAttribute("nSubprocessors", subProcessor.count());
    _doc.appendChild(root);
//    qDebug() << "send sumcom data";
    connect(this, &ProcessNode::sendSubNodeTree, projManager::Instance(), &ProjectFileManager::addSubtree, Qt::SingleShotConnection);
    emit sendSubNodeTree(identifier(), _doc);
}

void PN_SumComponents::loadDataFromQdomElement(QDomElement domElement)
{
    emit didFinishLoad(identifier());
}
