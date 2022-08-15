#ifndef PN_SUMCOMPONENTS_H
#define PN_SUMCOMPONENTS_H

#include "processnode.h"

class PN_SumComponents : public ProcessNode
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit PN_SumComponents(QObject *parent = nullptr);
    explicit PN_SumComponents(QByteArray identifier, int idxInControlller,
                              PN_SumComponents *controller,
                              TypeHelper::ValueType type=TypeHelper::Undefined, quint16 valueNumber=0,
                              QObject *parent = nullptr);



    // ValueNotifierClass interface
public slots:
    void slot_quatChanged(QQuaternion quat, int frame) override;
    void slot_vectorChanged(QVector3D vect, int frame) override;
    void slot_touchChanged(QList<float> touch, int frame) override;
    void slot_valuesChanged(QList<float> values, int frame) override;
    void slot_singleValueChanged(float value, int frame) override;
    void slot_boolValueChanged(bool value, int frame) override;
    void slot_trigger(int frame) override;

    // ProcessNode interface
public:
    ProcessNode *createSubprocessor(QString objectname_id) override;

private:
//    float process(float value) override;
//    quint8 process(quint8 value) override;

    // SaveLoadInterfaceClass interface
public slots:
    void initSaveData() override;
    void loadDataFromQdomElement(QDomElement domElement) override;
};

#endif // PN_SUMCOMPONENTS_H
