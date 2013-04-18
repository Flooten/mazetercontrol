/*
 * FILNAMN:       controlsignalsplotscene.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 * BESKRIVNING:   ControlSignalsPlotScene är klassen som representerar den scen
 *                som presenterar en tidsplot av motorns kontrollsignalvärden.
 *
 */

#ifndef CONTROLSIGNALSPLOTSCENE_H
#define CONTROLSIGNALSPLOTSCENE_H

#include "controlsignals.h"

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QObject>

namespace MC
{
    class ControlSignalsPlotScene : public QGraphicsScene
    {
        Q_OBJECT
    public:
        ControlSignalsPlotScene(QObject* parent = NULL);
        ~ControlSignalsPlotScene();

        void newControlSignals(ControlSignals control_signals);

    private:
        QVector<ControlSignals> control_signals_;

        const int MAX_LEVEL_LEFT_ = -5;
        const int ZERO_LEVEL_LEFT_ = 45;
        const int MIN_LEVEL_LEFT_ = 95;

        const int MAX_LEVEL_RIGHT_ = 105;
        const int ZERO_LEVEL_RIGHT_ = 155;
        const int MIN_LEVEL_RIGHT_ = 205;

        int time_ = 0;

        QPen lpen_;
        QPen rpen_;
        QPen standard_pen_;

        void drawScale();

    public slots:
        void clear();
        void draw();
    };
} // namespace MC

#endif // CONTROLSIGNALSPLOTSCENE_H
