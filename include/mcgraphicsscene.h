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

#include "sensordata.h"

#include <QGraphicsScene>
#include <QString>
#include <QMap>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>

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
        void draw();

    private:
        enum SensorDataIndex
        {
            FRONT_LEFT,
            FRONT_RIGHT,
            LEFT_LONG,
            RIGHT_LONG,
            LEFT_SHORT,
            RIGHT_SHORT,
            BACK,
            ANGLE,
            LINE_DEVIATION,
            LINE_TYPE
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
        QMap<SensorDataIndex, QGraphicsTextItem*> sensor_data_;
        QMap<ButtonIndex, QGraphicsPixmapItem*> buttons_;

        const QString UNIT_CM = " cm";
        const QString UNIT_DEG = " degrees";

        void createButtons();
        void createTextItems();
        void placeTextItems();
    };
} // namespace MC

#endif // MCGRAPHICSSCENE_H
