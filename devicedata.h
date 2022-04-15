#ifndef DEVICEDATA_H
#define DEVICEDATA_H

#include <QObject>



class DeviceData : public QObject
{
    Q_OBJECT

    struct acceleration {
        float x,y,z;
    };
    struct gyroscope {
        float x,y,z;
    };
    struct quaternion {
        float x,y,z,w;
    };
    struct touchValues {
        int touch[6];
    };
    struct gravityVector {
        float x,y,z;
    };

public:
    explicit DeviceData(QObject *parent = nullptr);



signals:

};

#endif // DEVICEDATA_H
