#ifndef MCGRAPHICSSCENE_H
#define MCGRAPHICSSCENE_H

#include <QGraphicsScene>

class MCGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MCGraphicsScene(QObject *parent = NULL);
};

#endif // MCGRAPHICSSCENE_H
