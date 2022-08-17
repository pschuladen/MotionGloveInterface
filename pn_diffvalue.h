#ifndef PN_DIFFVALUE_H
#define PN_DIFFVALUE_H

#include "processnode.h"

class PN_DiffValue : public ProcessNode
{
    Q_OBJECT
public:
    explicit PN_DiffValue(QObject *parent = nullptr);
    explicit PN_DiffValue(QByteArray identifier, int idxInControlller,
                              PN_DiffValue *controller,
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

    // ProcessNode interface
public:
    ProcessNode *createSubprocessor(QString objectname_id) override;

private:
    QQuaternion m_lastQuat;
    QVector3D m_lastVect;
    QList<float> m_lastTouch;
    QList<float> m_lastValues;
    float m_lastValue = 0;
    bool m_lastBool;

};

#endif // PN_DIFFVALUE_H
