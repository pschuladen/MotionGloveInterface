#ifndef OSCINPUTVIEWCONTROLLER_H
#define OSCINPUTVIEWCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

class OscInputViewController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool connectDevice READ connectDevice WRITE setConnectDevice NOTIFY connectDeviceChanged)


public:
    explicit OscInputViewController(QObject *parent = nullptr);

    bool connectDevice() const;
    void setConnectDevice(bool newConnectDevice);

signals:

    void connectDeviceChanged(bool connect);
private:
    bool m_connectDevice;
};

#endif // OSCINPUTVIEWCONTROLLER_H
