#ifndef MAINSCENE_HPP
#define MAINSCENE_HPP

#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QObject>
#include <QString>
#include <QPixmap>
#include "entity/entity.hpp"
#include "entity/player.hpp"
#include "mobSpawner.hpp"

class MainScene : public QGraphicsScene {
    Q_OBJECT  // This macro should be the first thing inside the class definition

private:
    QList<Entity*>* entities;
    QElapsedTimer deltaTimer;
    qint64 lastFrameTime;

    QTimer* gameTimer;
    qint64 deltaTime;   // Time between two frames
    qint64 sceneTime;   // Time passed since start of scene
    Player* mainPlayer = nullptr;
    MobSpawner* mobSpawner = nullptr;
    QPixmap m_tileImage;
    qint64 gameScore = 0;   // Total score of the game

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
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    

public:
    // Constructors/Destructor
    MainScene(QObject* parent = nullptr, int fps=60);
    virtual ~MainScene();
    Player* getMainPlayer();
    void setBackgroundTile(const QString &image_path);

    void setSpawner(const QString& spawnerFilename);
signals:
    void playerMoved(Player* player);
};

#endif   // MAINSCENE_HPP