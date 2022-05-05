#ifndef PROC_SCALE_H
#define PROC_SCALE_H

#include <QObject>
#include "processnodecontroller.h"

class Proc_Scale : public ProcessNodeController
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float inLow READ inLow WRITE setInLow NOTIFY inLowChanged)
    Q_PROPERTY(float inHigh READ inHigh WRITE setInHigh NOTIFY inHighChanged)
    Q_PROPERTY(float outLow READ outLow WRITE setOutLow NOTIFY outLowChanged)
    Q_PROPERTY(float outHigh READ outHigh WRITE setOutHigh NOTIFY outHighChanged)


    float m_inLow;

    float m_inHigh;

    float m_outLow;

    float m_outHigh;

public:
    explicit Proc_Scale(QObject *parent = nullptr, bool isProcessing=false);

//    virtual QVector3D processVector(QVector3D vector, int frame=-1) override;
//    virtual QQuaternion processQuat(QQuaternion quat, int frame=-1) override;
//    virtual float processSingleValue(float value, int frame=-1) override;
//    virtual QList<float> processValueList(QList<float> values, int frame=-1) override;


    float inLow() const;
    void setInLow(float newInLow);

    float inHigh() const;
    void setInHigh(float newInHigh);

    float outLow() const;
    void setOutLow(float newOutLow);

    float outHigh() const;
    void setOutHigh(float newOutHigh);

private:
    virtual float simpleProcessingFunction(float value) override;

signals:

    void inLowChanged();
    void inHighChanged();
    void outLowChanged();
    void outHighChanged();
};

#endif // PROC_SCALE_H
