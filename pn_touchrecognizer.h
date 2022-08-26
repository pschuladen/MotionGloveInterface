#ifndef PN_TOUCHRECOGNIZER_H
#define PN_TOUCHRECOGNIZER_H

#include "processnode.h"

class PN_TouchRecognizer : public ProcessNode
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QList<float> touchThresholds READ touchThresholds WRITE setTouchThresholds NOTIFY touchThresholdsChanged)
    Q_PROPERTY(QList<float> touchOffValues READ touchOffValues WRITE setTouchOffValues NOTIFY touchOffValuesChanged)
    Q_PROPERTY(QList<float> currentTouchValues READ currentTouchValues WRITE setCurrentTouchValues NOTIFY currentTouchValuesChanged)
    Q_PROPERTY(float thresholdValue READ thresholdValue WRITE setThresholdValue NOTIFY thresholdValueChanged)


public:
    explicit PN_TouchRecognizer(QObject *parent = nullptr);
    explicit PN_TouchRecognizer(QByteArray identifier, int idxInControlller,
                              PN_TouchRecognizer *controller,
                              TypeHelper::ValueType type=TypeHelper::Undefined, quint16 valueNumber=0,
                              QObject *parent = nullptr);

    // ProcessNode interface
public:
    ProcessNode *createSubprocessor(QString objectname_id) override;
    const QList<float> &touchThresholds() const;
    void setTouchThresholds(const QList<float> &newTouchThresholds);
    void setTouchThresholdAtIdx(int idx, float threshold);
    float thresholdValue() const;
    void setThresholdValue(float newThresholdValue);

    const QList<float> &touchOffValues() const;
    void setTouchOffValues(const QList<float> &newTouchOffValues);

    const QList<float> &currentTouchValues() const;
    void setCurrentTouchValues(const QList<float> &newCurrentTouchValues);

signals:
    void touchThresholdsChanged(QList<float> touchThresh);
    void thresholdValueChanged(float newThreshold);

    void sig_catchOffValues();

    void touchOffValuesChanged(QList<float> &newTouchOffValues);

    void currentTouchValuesChanged();

private:
    QList<float> m_touchThresholds{45,45,45,45,45,45};

    // SaveLoadInterfaceClass interface
    float m_thresholdValue = 0.5;

    QList<float> m_touchOffValues = QList<float>(6);

    QList<float> m_currentTouchValues = QList<float>(6);

    void calcTouchThresholdValues();

    bool catchOffValues = false;
    bool catchValues = false;

    QList<bool> touchedBoolList = QList<bool>(6);

    QList<float> getTListFromQDomElement(QDomElement domElement, QString paramName);

public slots:
    void initSaveData() override;
    void loadDataFromQdomElement(QDomElement domElement) override;

    void slot_catchOffValues();


    // ValueNotifierClass interface
public:
    bool acceptsInputType(TypeHelper::ValueType typ, int atIdx) const override;

public slots:
    void slot_valuesChanged(QList<float> values, int frame) override;
};

#endif // PN_TOUCHRECOGNIZER_H
