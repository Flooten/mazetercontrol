#ifndef MCGRAPHICSSCENE_H
#define MCGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QString>
#include <QMap>
#include <QGraphicsTextItem>
#include "sensordata.h"

namespace MC
{
    class MCGraphicsScene : public QGraphicsScene
    {
        Q_OBJECT
    public:
        MCGraphicsScene(QObject *parent = NULL);
        ~MCGraphicsScene();

        void update(const SensorData& sensor_data);

    private:
        QPixmap* background_image_;
        QMap<QString,QGraphicsTextItem*> sensor_values_;

    };
} // namespace MC

#endif // MCGRAPHICSSCENE_H
