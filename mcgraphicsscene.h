/*
 * FILNAMN:       mcgraphicsscene.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 * BESKRIVNING:   MCGraphicsScene är klassen som representerar den scen
 *                som presenterar översikten av roboten.
 *
 */

#ifndef MCGRAPHICSSCENE_H
#define MCGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QString>
#include <QMap>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include "sensordata.h"

namespace MC
{
    class MCGraphicsScene : public QGraphicsScene
    {
        Q_OBJECT
    public:
        MCGraphicsScene(QObject *parent = NULL);
        ~MCGraphicsScene();

        void updateSensorData(const SensorData& sensor_data);
        void buttonPressed(QKeyEvent* event);
        void buttonReleased(QKeyEvent* event);

    private:
        enum SensorDataIndex
        {

        };

        enum ButtonIndex
        {
            KEY_UP,
            KEY_DOWN,
            KEY_LEFT,
            KEY_RIGHT,
            KEY_INVALID
        };

        QPixmap* background_image_;
        QMap<QString, QGraphicsTextItem*> sensor_data_;
        QMap<ButtonIndex, QGraphicsRectItem*> buttons_;

        const QString UNIT_CM = " cm";
        const QString UNIT_DEG = " degrees";

        void createButtons();
        void createTextItems();
        void placeTextItems();
    };
} // namespace MC

#endif // MCGRAPHICSSCENE_H
