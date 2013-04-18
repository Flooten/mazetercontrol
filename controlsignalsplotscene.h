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
        int zero_level_left_ = 50;
        int zero_level_right_ = 150;
        int time_ = 0;

        QPen* lpen;
        QPen* rpen;

    public slots:
        void clear();
        void draw();
    };
} // namespace MC

#endif // CONTROLSIGNALSPLOTSCENE_H
