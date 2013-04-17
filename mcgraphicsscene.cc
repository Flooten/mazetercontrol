#include "mcgraphicsscene.h"

MCGraphicsScene::MCGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    addPixmap(QPixmap(":/images/resources/skiss.png"));
}
