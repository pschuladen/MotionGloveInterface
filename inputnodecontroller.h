#ifndef INPUTNODECONTROLLER_H
#define INPUTNODECONTROLLER_H

//#include <QQuickItem>

#include <QtQml/qqmlregistration.h>
#include "inputvalueviewcontroller.h"
#include "nodeviewcontroller.h"

class InputNodeController : public NodeViewController
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(InputValueViewController* inputValueController READ inputValueController WRITE setInputValueController NOTIFY inputValueControllerChanged)
    Q_PROPERTY(bool mouseHover READ mouseHover WRITE setMouseHover NOTIFY mouseHoverChanged)

public:
    InputNodeController();


    InputValueViewController *inputValueController() const;
    void setInputValueController(InputValueViewController *newInputValueController);

    bool mouseHover() const;

public slots:
    void setMouseHover(bool newMouseHover);

signals:

    void inputValueControllerChanged();
    void mouseHoverChanged(bool mouseHover);

private:
    InputValueViewController *m_inputValueController;
    bool m_mouseHover;
};

#endif // INPUTNODECONTROLLER_H
