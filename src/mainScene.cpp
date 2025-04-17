#include "../include/entity/item.hpp"
#include "../include/entity/player.hpp"
#include "../include/entity/mob.hpp"
#include "../include/entity/rocket.hpp"
#include "../include/mainScene.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor.
 */
MainScene::MainScene(QObject* parent, int fps) : QGraphicsScene(parent) {
    // Scene options
    setSceneRect(0, 0, 1000, 1000);       // Scene size
    setItemIndexMethod(QGraphicsScene::NoIndex);      // Collision detection method : linear
    entities = new QList<Entity*>();

    // Scene setup example
    Item* it = new Item(Vector2(50, 600), Vector2(50, 50), Sprites::SpriteImage::Coin);
    addEntity(it);
    Player* pl = new Player(20, Vector2(300, 300), Vector2(100, 100), Sprites::SpriteImage::Player);
    addEntity(pl);
    // Mob* mob = new Mob(20, 20, Vector2(300, 300), Vector2(60, 60), Sprites::SpriteImage::Player);
    // addEntity(mob);
    Rocket* ro = new Rocket(Effect(Effects::EffectType::Boom, 10000, 3000), 50, Vector2(0, 0.1), 800, Vector2(300, 0), Vector2(50, 50), Sprites::SpriteImage::Coin);
    addEntity(ro);

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
    for (qint64 i=0; i<entities->size(); i++) {
        Entity* entity =entities->at(i);
        
        if (entity->onUpdate(deltaTime)) {      // Update entity. True if entity wants to spawn another entity

            // Spawn new entities while current entity in loop wants to spawn entities
            Entity* newEntity = entity->getSpawned();
            while (newEntity != nullptr) {
                addEntity(newEntity);
                newEntity = entity->getSpawned();
            }
        }
    }
}

/**
 * Cleanup the scene from removed entities
 */
void MainScene::cleanupScene() {
    for (qint64 i=0; i<entities->size(); i++) {
        Entity* entity = entities->at(i);
        if (entity->getDeleted()) {     // if entity has been removed
            entities->removeAt(i);       // remove it
            delete entity;
            i--;                        // Readjust i to avoid out of list bounds
        }
    }
}

/**
 * Main game loop. Triggered every frame.
 */
void MainScene::gameLoop() {
    checkCollisions();
    updateEntities();
    cleanupScene();
}