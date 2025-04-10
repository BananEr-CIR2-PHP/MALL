#include "../include/item.hpp"
#include "../include/player.hpp"
#include "../include/mainScene.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor.
 */
MainScene::MainScene(QObject* parent, int fps) {
    setSceneRect(0, 0, 1000, 1000);       // Scene size
    setItemIndexMethod(QGraphicsScene::NoIndex);      // Collision detection method : linear
    entities = new QList<Entity*>();

    // TODO: fix memory leak, better entity spawn
    QImage* img = new QImage("../res/img/coin.png");
    Sprite* sp1 = new Sprite(img);
    Sprite* sp2 = new Sprite(img);
    Item* it = new Item(Vector2(50, 600), Vector2(50, 50), sp1);
    addEntity(it);
    Player* pl = new Player(20, Vector2(50, 600), Vector2(50, 50), sp2);
    addEntity(pl);

    // Activate game loop
    deltaTime = (qint64) (1000/fps);
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainScene::gameLoop);

    gameTimer->start(deltaTime);
}

/**
 * Destructor
 */
MainScene::~MainScene() {
    for (Entity* entity : *entities) {
        delete entity;
    }
    delete entities;
    disconnect(gameTimer, nullptr, nullptr, nullptr);       // Delete timer signal
    delete gameTimer;
}

// -- METHODS ---

/**
 * Add an entity to the scene
 * 
 * @param entity The entity to add to the scene
 */
void MainScene::addEntity(Entity* entity) {
    addItem(entity);
    entities->append(entity);
}

/**
 * Triggers onCollide(Entity* other) on each colliding Entity
 */
void MainScene::checkCollisions() {
    for (Entity* entity : *entities) {
        for (QGraphicsItem* graphicsItem : entity->collidingItems()) {
            if (Entity* otherEntity = dynamic_cast<Entity*>(graphicsItem)) {
                entity->onCollide(otherEntity);
            }
        }
    }
}

/**
 * Triggers onUpdate() on each Entity
 */
void MainScene::updateEntities() {
    for (Entity* entity : *entities) {
        entity->onUpdate(deltaTime);
    }
}

/**
 * Main game loop. Triggered every frame.
 */
void MainScene::gameLoop() {
    updateEntities();
    checkCollisions();
}