#include "pn_touchrecognizer.h"

PN_TouchRecognizer::PN_TouchRecognizer(QObject *parent)
    : ProcessNode{parent}
{
//    m_touchThresholds.resize(6);
//    for(int i=0; i< m_touchThresholds.size(); i++) m_touchThresholds[i] = 45;
}

PN_TouchRecognizer::PN_TouchRecognizer(QByteArray identifier, int idxInControlller, PN_TouchRecognizer *controller, TypeHelper::ValueType type, quint16 valueNumber, QObject *parent)
    : ProcessNode{identifier, idxInControlller, type, valueNumber, parent},
      m_touchThresholds{controller->touchThresholds()},
      m_touchOffValues{controller->touchOffValues()},
      m_thresholdValue{controller->thresholdValue()}
{
    m_processorType = TypeHelper::TouchRecognizer;
    touchedBoolList.resize(6);
//    m_touchThresholds.resize(6);
    connect(this, &PN_TouchRecognizer::touchThresholdsChanged, controller, &PN_TouchRecognizer::setTouchThresholds);
    connect(this, &PN_TouchRecognizer::touchOffValuesChanged, controller, &PN_TouchRecognizer::setTouchOffValues);
    connect(controller, &PN_TouchRecognizer::sig_catchOffValues, this, &PN_TouchRecognizer::slot_catchOffValues);
    connect(controller, &PN_TouchRecognizer::thresholdValueChanged, this, &PN_TouchRecognizer::setThresholdValue);
}


ProcessNode *PN_TouchRecognizer::createSubprocessor(QString objectname_id)
{
    PN_TouchRecognizer *newSubprocessor = new PN_TouchRecognizer(identifier(), subProcessor.size(), this);
    connect(this, &QObject::destroyed, newSubprocessor, &QObject::deleteLater);
    emit newSubprocessorWasCreated(newSubprocessor);
    return newSubprocessor;
}

const QList<float> &PN_TouchRecognizer::touchThresholds() const
{
    return m_touchThresholds;
}

void PN_TouchRecognizer::setTouchThresholds(const QList<float> &newTouchThreshold)
{
    if (m_touchThresholds == newTouchThreshold)
        return;
    m_touchThresholds = newTouchThreshold;
    emit touchThresholdsChanged(m_touchThresholds);
}

void PN_TouchRecognizer::setTouchThresholdAtIdx(int idx, float threshold)
{
    if(idx < touchThresholds().size()) {
        if(threshold != touchThresholds().at(idx)) {
            m_touchThresholds[idx] = threshold;
            emit touchThresholdsChanged(m_touchThresholds);
        }
    }
}


void PN_TouchRecognizer::initSaveData()
{
    QDomDocument _doc("tmpdoc");
    QDomElement root = _doc.createElement("processor");
    root.setAttribute("nSubprocessors", subProcessor.count());
    _doc.appendChild(root);
    QDomElement _threshVal = _doc.createElement("thresholdValue");
    _threshVal.setAttribute("value", QString::number(thresholdValue()));
    QDomElement _tOffVals = _doc.createElement("touchOffValues");
    QDomElement _tThreshs = _doc.createElement("touchThresholds");
    for(int i = 0; i< 6 ; i++) {
        _tThreshs.setAttribute(QString("t%1").arg(i), touchThresholds().at(i));
        _tOffVals.setAttribute(QString("t%1").arg(i), touchOffValues().at(i));
    }
    root.appendChild(_threshVal);
    root.appendChild(_tOffVals);
    root.appendChild(_tThreshs);

    connect(this, &ProcessNode::sendSubNodeTree, projManager::Instance(), &ProjectFileManager::addSubtree, Qt::SingleShotConnection);
    emit sendSubNodeTree(identifier(), _doc);
}

void PN_TouchRecognizer::loadDataFromQdomElement(QDomElement domElement)
{
    QDomElement _threshVal = domElement.firstChildElement("thresholdValue");
    if(!_threshVal.isNull()) {
        setThresholdValue(_threshVal.attribute("value", "0.6").toFloat());
    }
    else {
        qWarning() << "pn_touchRecognizer: no touchThreshValue found in file";
    }
    setTouchOffValues(getTListFromQDomElement(domElement, "touchOffValues"));
    setTouchThresholds(getTListFromQDomElement(domElement, "touchThresholds"));

    emit didFinishLoad(identifier());
}

void PN_TouchRecognizer::slot_catchOffValues()
{
    catchOffValues = true;
}

float PN_TouchRecognizer::thresholdValue() const
{
    return m_thresholdValue;
}

void PN_TouchRecognizer::setThresholdValue(float newThresholdValue)
{
    qDebug() << "pn_touchRecognizer: thresholdValue" << objectName() << newThresholdValue;
    if (qFuzzyCompare(m_thresholdValue, newThresholdValue))
        return;
    m_thresholdValue = newThresholdValue;
    emit thresholdValueChanged(m_thresholdValue);
    calcTouchThresholdValues();
}

const QList<float> &PN_TouchRecognizer::touchOffValues() const
{
    return m_touchOffValues;
}

void PN_TouchRecognizer::setTouchOffValues(const QList<float> &newTouchOffValues)
{
    if (m_touchOffValues == newTouchOffValues)
        return;
    m_touchOffValues = newTouchOffValues;
    emit touchOffValuesChanged(m_touchOffValues);
    calcTouchThresholdValues();
}

const QList<float> &PN_TouchRecognizer::currentTouchValues() const
{
    return m_currentTouchValues;
}

void PN_TouchRecognizer::setCurrentTouchValues(const QList<float> &newCurrentTouchValues)
{
    if (m_currentTouchValues == newCurrentTouchValues)
        return;
    m_currentTouchValues = newCurrentTouchValues;
    emit currentTouchValuesChanged();
}

void PN_TouchRecognizer::calcTouchThresholdValues()
{
    for(int i = 0; i<6; i++) {
        m_touchThresholds[i] = m_touchOffValues[i] * thresholdValue();
    }
    emit touchThresholdsChanged(m_touchThresholds);
}

QList<float> PN_TouchRecognizer::getTListFromQDomElement(QDomElement domElement, QString paramName)
{
    QDomElement _valElem = domElement.firstChildElement(paramName);
    QList<float> _valList(6);
    if(!_valElem.isNull()) {
        for(int i = 0; i<6; i++) {
            bool _vok;
            float _thresh = _valElem.attribute(QString("t%1").arg(i), "30").toFloat(&_vok);
            if(_vok) _valList[i] = _thresh;
            else _valList[i] = 45;
        }
    }
    else {
        qWarning() << "pn_touchRecognizer: no touchThreshols found in file";
    }
    return _valList;
}


bool PN_TouchRecognizer::acceptsInputType(TypeHelper::ValueType typ, int atIdx) const
{
    if(typ == TypeHelper::List && atIdx == 0) return true;
    else return false;
}

void PN_TouchRecognizer::slot_valuesChanged(QList<float> values, int frame)
{
    if(catchOffValues) {
        values.resize(6);
        setTouchOffValues(values);
//        calcTouchThresholdValues();
//        for(int i = 0; i<6; i++) {
//            m_touchThresholds[i] = m_touchOffValues[i] * thresholdValue();
//        }
//        emit touchThresholdsChanged(m_touchThresholds);
//        emit touchOffValuesChanged(m_touchOffValues);
        catchOffValues = false;
    }
    bool _didChange = false;
    for(int i = 0; i < 6; i++) {
        bool _touch = values[i] < m_touchThresholds[i];
        _didChange = _didChange || _touch != touchedBoolList[i];
        touchedBoolList[i] = _touch;
    }
    if(_didChange) {
        emit boolListChanged(touchedBoolList, frame);
    }
}
