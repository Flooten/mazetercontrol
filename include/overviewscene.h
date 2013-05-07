/*
 * FILNAMN:       overviewscene.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 * BESKRIVNING:   OverviewScene är klassen som representerar den scen
 *                som presenterar översikten av roboten.
 *
 */

#ifndef OVERVIEWSCENE_H
#define OVERVIEWSCENE_H

#include "sensordata.h"

#include <QGraphicsScene>
#include <QString>
#include <QMap>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

namespace MC
{
    class OverviewScene : public QGraphicsScene
    {
        Q_OBJECT
    public:
        OverviewScene(QObject *parent = NULL);
        ~OverviewScene();

        void updateSensorData(const SensorData& sensor_data);
        void buttonPressed(QKeyEvent* event);
        void buttonReleased(QKeyEvent* event);
        void show();
        void hide();

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

        QGraphicsPixmapItem* background_image_;
        QMap<SensorDataIndex, QGraphicsTextItem*> sensor_data_;
        QMap<ButtonIndex, QGraphicsPixmapItem*> buttons_;

        const QString UNIT_CM = " cm";
        const QString UNIT_DEG = " degrees";

        void createButtons();
        void createTextItems();
        void placeTextItems();
    };
} // namespace MC

#endif // OVERVIEWSCENE_H
