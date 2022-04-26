#ifndef DATAPROCESSINGNODE_H
#define DATAPROCESSINGNODE_H
/*
 * This class provides mechanism for processing the sensor data.
 * (This is the base class?)
 * It provides all processing methods.
 * For this several slots for receiving data from previous nodes
 * are provided with several types (e.g. Vector, Quat, QList<float>, trigger,...)
 *
 * A suitable sync mechanism has to be established to ensure that values
 * from the same input package are processed.
 *
 *  methods:
 *  scale, sum, rot_quat, rot_vect, quatToEuler, eulerToQuat, threshold
*/

#include <QObject>

class DataProcessingNode : public QObject
{
    Q_OBJECT

public:
    explicit DataProcessingNode(QObject *parent = nullptr);

signals:
    void dataChanged(QList<float> newData);

};

#endif // DATAPROCESSINGNODE_H
