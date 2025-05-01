#ifndef MAINSCENE_HPP
#define MAINSCENE_HPP

#include <QGraphicsScene>
#include <QTimer>
#include <QObject>
#include "entity/entity.hpp"
#include "entity/player.hpp"
#include "mobSpawner.hpp"

class MainScene : public QGraphicsScene {
    Q_OBJECT  // This macro should be the first thing inside the class definition

private:
    QList<Entity*>* entities;
    QTimer* gameTimer;
    qint64 deltaTime;   // Time between two frames
    qint64 sceneTime;   // Time passed since start of scene
    Player* mainPlayer = nullptr;
    MobSpawner* mobSpawner = nullptr;

protected:
    void addEntity(Entity* entity);
    void setControlledPlayer(Player* player);
    void checkCollisions();
    void updateEntities();
    void cleanupScene();
    void spawnMobWave();
    void gameLoop();

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

public:
    // Constructors/Destructor
    MainScene(QObject* parent = nullptr, int fps=60);
    virtual ~MainScene();

    void setSpawner(const QString& spawnerFilename);
};

#endif   // MAINSCENE_HPP