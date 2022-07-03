#include "inputnodecontroller.h"

InputNodeController::InputNodeController()
{

}

InputValueViewController *InputNodeController::inputValueController() const
{
    return m_inputValueController;
}

void InputNodeController::setInputValueController(InputValueViewController *newInputValueController)
{
    if (m_inputValueController == newInputValueController)
        return;
    m_inputValueController = newInputValueController;
    connect(this, &InputNodeController::mouseHoverChanged, newInputValueController, &InputValueViewController::setMouseHover);
    connect(newInputValueController, &InputValueViewController::mouseHoverChanged, this, &InputNodeController::setMouseHover);
    newInputValueController->setCanDragInput(false);
//    qDebug() << "value controller set" << this << newInputValueController;
    emit inputValueControllerChanged();
}

bool InputNodeController::mouseHover() const
{
    return m_mouseHover;
}

void InputNodeController::setMouseHover(bool newMouseHover)
{

    if (m_mouseHover == newMouseHover)
        return;
    m_mouseHover = newMouseHover;
    emit mouseHoverChanged(m_mouseHover);
}


void InputNodeController::initSaveData()
{
    QDomDocument _doc("tmpdoc");
    QDomElement root = _doc.createElement("view-node");
    _doc.appendChild(root);
    QDomElement _pos = _doc.createElement("position");
    _pos.setAttribute("x", x());
    _pos.setAttribute("y", y());
    root.appendChild(_pos);

    QDomElement _source = _doc.createElement("attributes");

    _source.setAttribute("sourceDevice", sourceDevice());
    _source.setAttribute("inputPath", inputPath());
    _source.setAttribute("valueType", valueType());

    root.appendChild(_source);

    connect(this, &NodeViewController::sendSubNodeTree, projManager::Instance(), &ProjectFileManager::addSubtree, Qt::SingleShotConnection);
    emit sendSubNodeTree(uniqueID(), _doc);


}
const QString &InputNodeController::inputPath() const
{
    return m_inputPath;
}

void InputNodeController::setInputPath(const QString &newInputPath)
{
    if (m_inputPath == newInputPath)
        return;
    m_inputPath = newInputPath;
    emit inputPathChanged();
}

const TypeHelper::ValueType &InputNodeController::valueType() const
{
    return m_valueType;
}

void InputNodeController::setValueType(const TypeHelper::ValueType &newValueType)
{
    if (m_valueType == newValueType)
        return;
    m_valueType = newValueType;
    emit valueTypeChanged();
}

const QString &InputNodeController::sourceDevice() const
{
    return m_sourceDevice;
}

void InputNodeController::setSourceDevice(const QString &newSourceDevice)
{
    if (m_sourceDevice == newSourceDevice)
        return;
    m_sourceDevice = newSourceDevice;
    emit sourceDeviceChanged();
}
