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
#include "plotscene.h"

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QObject>

namespace MC
{
    class ControlSignalsPlotScene : public PlotScene
    {
        Q_OBJECT
    public:
        ControlSignalsPlotScene(QObject* parent = NULL);

        void newControlSignals(ControlSignals control_signals);

    private:
        QVector<ControlSignals> control_signals_;

        const int MAX_LEVEL_LEFT_ = -5;
        const int ZERO_LEVEL_LEFT_ = 45;
        const int MIN_LEVEL_LEFT_ = 95;

        const int MAX_LEVEL_RIGHT_ = 105;
        const int ZERO_LEVEL_RIGHT_ = 155;
        const int MIN_LEVEL_RIGHT_ = 205;

        QPen lpen_;
        QPen rpen_;

        void drawGrid() override;

    public slots:
        void draw() override;
    };
} // namespace MC

#endif // CONTROLSIGNALSPLOTSCENE_H
