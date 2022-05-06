#ifndef PROCESSNODECONTROLLER_H
#define PROCESSNODECONTROLLER_H

/*
 * This class is obsolete and will be deleted soon.
*/


#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QColor>

#include <QQuaternion>
#include <QVector3D>
#include <QList>

#include "dataprocessingnode.h"
//#include "valuenotifierclass.h"

#include "typehelper.h"

class ValueNotifierClass;
class ProcessNodeController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QList<float> inputValues READ inputValues WRITE setInputValues NOTIFY inputValuesChanged)
    Q_PROPERTY(QList<float> outputValues READ outputValues WRITE setOutputValues NOTIFY outputValuesChanged)
    Q_PROPERTY(QList<QList<float>> multiInputValues READ multiInputValues WRITE setMultiInputValues NOTIFY multiInputValuesChanged)
    Q_PROPERTY(QList<QList<float>> multiOutputValues READ multiOutputValues WRITE setMultiOutputValues NOTIFY multiOutputValuesChanged)

    Q_PROPERTY(bool acceptMultiInput READ acceptMultiInput WRITE setAcceptMultiInput NOTIFY acceptMultiInputChanged)
    Q_PROPERTY(int numberInputs READ numberInputs WRITE setNumberInputs NOTIFY numberInputsChanged)
    Q_PROPERTY(int activeIndex READ activeIndex WRITE setActiveIndex NOTIFY activeIndexChanged)
    Q_PROPERTY(QList<QColor> connectorColor READ connectorColor WRITE setConnectorColor NOTIFY connectorColorChanged)

    Q_PROPERTY(ProcessingType processType READ processType WRITE setProcessType NOTIFY processTypeChanged)

    Q_PROPERTY(float singleInputValue READ singleInputValue WRITE setSingleInputValue NOTIFY singleInputValueChanged)


public:
    explicit ProcessNodeController(QObject *parent = nullptr);

    enum ProcessingType {
        ViewOnly,
        Scale
    };
    Q_ENUM(ProcessingType)

    ProcessingType m_processType;
    virtual QVector3D processVector(QVector3D vector, int frame=-1);
    virtual QQuaternion processQuat(QQuaternion quat, int frame=-1);
    virtual float processSingleValue(float value, int frame=-1);
    virtual QList<float> processValueList(QList<float> values, int frame=-1);
    virtual bool processBoolValue(bool boolVal, int frame=-1); //TODO: implement


//    enum ValueType {
//        Undefined,
//        Vector,
//        Quat,
//        List,
//        Single
//    };
//    Q_ENUM(ValueType)
//    enum ValueSubType {
//        Any, X, Y, Z,
//    }

    const QList<QColor> &connectorColor() const;
    void setConnectorColor(const QList<QColor> &newConnectorColor);

    int numberInputs() const;
    void setNumberInputs(int newNumberInputs);

    bool acceptMultiInput() const;
    void setAcceptMultiInput(bool newAcceptMultiInput);

    const QList<QList<float> > &multiOutputValues() const;
    void setMultiOutputValues(const QList<QList<float> > &newMultiOutputValues);

    const QList<QList<float> > &multiInputValues() const;
    void setMultiInputValues(const QList<QList<float> > &newMultiInputValues);

    const QList<float> &outputValues() const;
    void setOutputValues(const QList<float> &newOutputValues);

    const QList<float> &inputValues() const;
    void setInputValues(const QList<float> &newInputValues);

    int activeIndex() const;
    void setActiveIndex(int newActiveIndex);

    const ProcessingType &processType() const;
    void setProcessType(const ProcessingType &newProcessType);

    bool objectIsView() const;
    void setObjectIsView(bool newObjectIsView);

    float singleInputValue() const;
    void setSingleInputValue(float newSingleInputValue);

    QList<ValueNotifierClass*> valueNotifier;

private:
    void createNotifier(int n);

    virtual float simpleProcessingFunction(float value);

    QList<QColor> m_connectorColor;

    int m_numberInputs;

    bool m_acceptMultiInput;

    QList<QList<float> > m_multiOutputValues;

    QList<QList<float> > m_multiInputValues;

    QList<float> m_outputValues;

    QList<float> m_inputValues;

    int m_activeIndex;

    bool m_objectIsView;

//    ProcessNodeController::ProcessingType m_processType;

    float m_singleInputValue;

public slots:
    void singleValueChanged(float value);
    bool connectionRequest(int index, TypeHelper::ValueType valueType);

signals:

    void connectorColorChanged();
    void numberInputsChanged();
    void acceptMultiInputChanged();
    void multiOutputValuesChanged();
    void multiInputValuesChanged();
    void outputValuesChanged();
    void inputValuesChanged();
    void activeIndexChanged();
    void processTypeChanged();
    void singleInputValueChanged();
};
//Q_DECLARE_METATYPE(ProcessNodeViewController)

#endif // PROCESSNODECONTROLLER_H
