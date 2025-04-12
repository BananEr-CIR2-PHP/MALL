#ifndef MAINSCENE_HPP
#define MAINSCENE_HPP

#include <QGraphicsScene>
#include <QTimer>
#include <QObject>
#include "entity/entity.hpp"
#include "entity/player.hpp"

class MainScene : public QGraphicsScene {
    Q_OBJECT  // This macro should be the first thing inside the class definition

private:
    QList<Entity*>* entities;
    QTimer* gameTimer;
    qint64 deltaTime;   // Time between two frames

protected:
    void addEntity(Entity* entity);
    void checkCollisions();
    void updateEntities();
    void cleanupScene();
    void gameLoop();

public:
    // Constructors/Destructor
    MainScene(QObject* parent = nullptr, int fps=60);
    virtual ~MainScene();
};

#endif   // MAINSCENE_HPP