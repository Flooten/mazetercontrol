#ifndef PLOTSCENE_H
#define PLOTSCENE_H

#include <QGraphicsScene>
#include <QPen>

namespace MC
{
    class PlotScene : public QGraphicsScene
    {
    public:
        PlotScene(QObject* parent = NULL);

        virtual void clear() = 0;
        virtual void drawGrid() = 0;
        virtual void draw() = 0;

    protected:
        QPen dashed_pen_ = QPen(Qt::DashLine);
    };
} // namespace MC
#endif // PLOTSCENE_H
