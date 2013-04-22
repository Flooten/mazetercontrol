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
#include <QGraphicsLineItem>
#include <QObject>
#include <QVector>

namespace MC
{
    class ControlSignalsPlotScene : public PlotScene
    {
        Q_OBJECT
    public:
        ControlSignalsPlotScene(int view_width, int view_height, QObject* parent = NULL);

        void newControlSignals(ControlSignals control_signals);

        void drawGrid() override;

    private:
        QVector<ControlSignals> control_signals_;

        const int MAX_LEVEL_LEFT_ = 20;
        const int ZERO_LEVEL_LEFT_ = 70;
        const int MIN_LEVEL_LEFT_ = 120;

        const int MAX_LEVEL_RIGHT_ = 140;
        const int ZERO_LEVEL_RIGHT_ = 190;
        const int MIN_LEVEL_RIGHT_ = 240;

        QPen lpen_;
        QPen rpen_;

        enum Line
        {
            MAX_RIGHT,
            ZERO_RIGHT,
        };

        QVector<QGraphicsLineItem*> line_vector_;

        QGraphicsLineItem* max_line_l_;
        QGraphicsLineItem* zero_line_l_;
        QGraphicsLineItem* min_line_l_;

        QGraphicsLineItem* max_line_r_;
        QGraphicsLineItem* zero_line_r_;
        QGraphicsLineItem* min_line_r_;

    public slots:
        void draw() override;
    };
} // namespace MC

#endif // CONTROLSIGNALSPLOTSCENE_H
