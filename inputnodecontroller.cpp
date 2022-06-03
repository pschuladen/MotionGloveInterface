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
