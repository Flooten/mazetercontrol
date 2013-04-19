/*
 * FILNAMN:       plotscene.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-19
 *
 * BESKRIVNING:   PlotScene är klassen som representerar den virtuella
 *                plotscen som presenterar en tidsplot av någon data.
 *
 */

#ifndef PLOTSCENE_H
#define PLOTSCENE_H

#include <QGraphicsScene>
#include <QPen>

namespace MC
{
    class PlotScene : public QGraphicsScene
    {
        Q_OBJECT
    public:
        PlotScene(QObject* parent = NULL);

        virtual void drawGrid() = 0;
        virtual void draw() = 0;

    public slots:
        virtual void clear();

    protected:
        QPen dashed_pen_ = QPen(Qt::DashLine);
        int time_ = 0;

    };
} // namespace MC
#endif // PLOTSCENE_H
