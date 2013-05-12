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
        PlotScene(int view_width, int view_height, QObject* parent = NULL);

        virtual void drawGrid() = 0;
        virtual void draw() = 0;

    public slots:
        virtual void clear();

    signals:
        void center(int time);

    protected:
        QPen dashed_pen_ = QPen(Qt::DashLine);        
        QPen red_pen_ = QPen(Qt::red);
        QPen blue_pen_ = QPen(Qt::blue);
        QPen green_pen_ = QPen(Qt::green);

        int time_ = 0;
        int view_width_;
        int view_height_;
    };
} // namespace MC
#endif // PLOTSCENE_H
