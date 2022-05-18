#ifndef OSCINPUTPARSER_H
#define OSCINPUTPARSER_H

#include "valuenotifierclass.h"
#include "oscpp/server.hpp"
#include "typehelper.h"

class OscInputParser : public ValueNotifierClass
{
    Q_OBJECT
    Q_PROPERTY(bool handleUnmappedOsc READ handleUnmappedOsc WRITE setHandleUnmappedOsc NOTIFY handleUnmappedOscChanged)
    Q_PROPERTY(TypeHelper::SensorType sensorType READ sensorType WRITE setSensorType NOTIFY sensorTypeChanged)

public:
    explicit OscInputParser(QObject *parent = nullptr);
    explicit OscInputParser(TypeHelper::SensorType sensorType, QObject *parent = nullptr);
    explicit OscInputParser(TypeHelper::ValueType valType, quint16 nValues, QObject *parent = nullptr);

    bool handleUnmappedOsc() const;
    void setHandleUnmappedOsc(bool newHandleUnmappedOsc);

    void handleOscArgs(OSCPP::Server::ArgStream args, quint32 frame=0);

    const TypeHelper::SensorType &sensorType() const;
    void setSensorType(const TypeHelper::SensorType &newSensorType);



signals:
    void gotUnmappedMessage();

    void handleUnmappedOscChanged();

    void errorInOscParsing();

    void sensorTypeChanged();

private:

    typedef void (OscInputParser::*OscHandleFn)(OSCPP::Server::ArgStream*);
    OscHandleFn m_oscHandle = &OscInputParser::oscR_unMapped;

    quint32 m_frame = 0;

    QVector3D m_vector;
    QQuaternion m_quat;
    QList<float> m_valueList;
    float m_value;


    void oscR_vectorType(OSCPP::Server::ArgStream *args);
    void oscR_quatType(OSCPP::Server::ArgStream *args);
    void oscR_touchType(OSCPP::Server::ArgStream *args);
    void oscR_floatListType(OSCPP::Server::ArgStream *args);
    void oscR_intListType(OSCPP::Server::ArgStream *args);
    void oscR_listType(OSCPP::Server::ArgStream *args);
    void oscR_gravityVector(OSCPP::Server::ArgStream *args);

    void oscR_singleValue(OSCPP::Server::ArgStream *args);

    void oscR_unMapped(OSCPP::Server::ArgStream *args);

    void set3dVectorFromArgs(OSCPP::Server::ArgStream *args);
    void setQuaternionFromArgs(OSCPP::Server::ArgStream *args);
    void setFloatListFromFloatArgs(OSCPP::Server::ArgStream *args);
    void setFloatListFromIntArgs(OSCPP::Server::ArgStream *args);
    void setFloatListFromArgs(OSCPP::Server::ArgStream *args);
    void setSingleFloatFromArgs(OSCPP::Server::ArgStream *args);

    bool m_handleUnmappedOsc = false;
    TypeHelper::SensorType m_sensorType;
};

#endif // OSCINPUTPARSER_H
