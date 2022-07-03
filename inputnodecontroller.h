#ifndef INPUTNODECONTROLLER_H
#define INPUTNODECONTROLLER_H

//#include <QQuickItem>

#include <QtQml/qqmlregistration.h>
#include "inputvalueviewcontroller.h"
#include "nodeviewcontroller.h"
#include "typehelper.h"

class InputNodeController : public NodeViewController
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(InputValueViewController* inputValueController READ inputValueController WRITE setInputValueController NOTIFY inputValueControllerChanged)
    Q_PROPERTY(bool mouseHover READ mouseHover WRITE setMouseHover NOTIFY mouseHoverChanged)
    Q_PROPERTY(QString inputPath READ inputPath WRITE setInputPath NOTIFY inputPathChanged)
    Q_PROPERTY(QString sourceDevice READ sourceDevice WRITE setSourceDevice NOTIFY sourceDeviceChanged)
    Q_PROPERTY(TypeHelper::ValueType valueType READ valueType WRITE setValueType NOTIFY valueTypeChanged)

public:
    InputNodeController();


    InputValueViewController *inputValueController() const;
    void setInputValueController(InputValueViewController *newInputValueController);

    bool mouseHover() const;

    const QString &inputPath() const;
    void setInputPath(const QString &newInputPath);

    const TypeHelper::ValueType &valueType() const;
    void setValueType(const TypeHelper::ValueType &newValueType);

    const QString &sourceDevice() const;
    void setSourceDevice(const QString &newSourceDevice);

public slots:
    void setMouseHover(bool newMouseHover);

signals:

    void inputValueControllerChanged();
    void mouseHoverChanged(bool mouseHover);

    void inputPathChanged();

    void valueTypeChanged();

    void sourceDeviceChanged();

private:
    InputValueViewController *m_inputValueController;
    bool m_mouseHover;

    // SaveLoadInterfaceClass interface
    QString m_inputPath;

    TypeHelper::ValueType m_valueType;

    QString m_sourceDevice;

public slots:
    void initSaveData() override;
};

#endif // INPUTNODECONTROLLER_H
