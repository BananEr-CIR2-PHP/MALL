#include "../include/entity/item.hpp"
#include "../include/entity/player.hpp"
#include "../include/entity/mob.hpp"
#include "../include/weapon/gun.hpp"
#include "../include/mainScene.hpp"
#include "../include/lootTables.hpp"

#define PLAYER_MAX_LIFE 200
#define PLAYER_MAX_ENERGY 500
#define PLAYER_BASE_GOLD 0
#define PLAYER_BASE_POS Vector2(0, 0)
#define PLAYER_DIMS Vector2(50, 100)
#define PLAYER_SPEED 0.5

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor.
 */
MainScene::MainScene(QObject* parent, int fps) : QGraphicsScene(parent) {
    // Scene options
    setSceneRect(0, 0, 900, 900);       // Scene size
    setItemIndexMethod(QGraphicsScene::NoIndex);      // Collision detection method : linear
    entities = new QList<Entity*>();
    setSpawner("level1.json");

    // Generate caches
    Item::generateCache();
    LootTables::generateTables();
    
    // Initialize player
    Player* pl = new Player(
        PLAYER_MAX_LIFE,
        PLAYER_MAX_ENERGY,
        PLAYER_BASE_GOLD,
        PLAYER_SPEED,
        PLAYER_BASE_POS,
        PLAYER_DIMS,
        "player.png",
        Teams::Player
    );
    pl->grabWeapon(Weapon::create("gun/blue_laser_pistol.json"), Inventory::WeaponSlot_1);
    setControlledPlayer(pl);
    addEntity(pl);

    gameScore = 0;

    // Activate game loop
    sceneTime = 0;
    deltaTime = (qint64) (1000/fps);
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainScene::gameLoop);

    gameTimer->start(deltaTime);

    deltaTimer.start();
    lastFrameTime = deltaTimer.elapsed();
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
    delete mobSpawner;
    Item::deleteCache();      // Delete the cache (should occur automatically, but we delete it just in case)
    LootTables::deleteTables();
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
                entity->onCollide(otherEntity, deltaTime);
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
            // If entity is a mob, add its score to global score
            if (Mob* mob = dynamic_cast<Mob*>(entity)) {
                gameScore += mob->getScoreValue();
            }
            entities->removeAt(i);       // remove the entity
            delete entity;
            i--;                        // Readjust i to avoid out of list bounds
        }
    }
}

/**
 * Spawn all the mobs that should spawn at this frame
 */
void MainScene::spawnMobWave() {
    if (mobSpawner) {
        Mob* newMob = mobSpawner->getSpawned(sceneTime, mainPlayer);
        while (newMob != nullptr) {
            addEntity(newMob);
            newMob = mobSpawner->getSpawned(sceneTime, mainPlayer);
        }
    }
}

/**
 * Main game loop. Triggered every frame.
 */
void MainScene::gameLoop() {
    // Change delta time
    sceneTime = deltaTimer.elapsed();
    deltaTime = sceneTime - lastFrameTime;
    lastFrameTime = sceneTime;

    checkCollisions();
    updateEntities();
    cleanupScene();
    spawnMobWave();

    if (mainPlayer && mainPlayer->getIsDead()) {
        // TODO: react to player death. Use gameScore to get the total score of the game
    }
}

/**
 * Set a new spawner for the scene
 * 
 * @param spawnerFilename File name of new spawner (should look like "foo.json")
 */
void MainScene::setSpawner(const QString& spawnerFilename) {
    delete mobSpawner;
    mobSpawner = new MobSpawner(spawnerFilename);
}

/**
 * Define which player entity is controlled by user
 */
void MainScene::setControlledPlayer(Player* player) {
    mainPlayer = player;
}

/**
 * Handle mouse press event
 */
void MainScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (mainPlayer) {
        switch (event->button()) {
            case Qt::LeftButton:
                // Player action: player is now using weapon
                QPointF mousePos = event->scenePos();
                mainPlayer->actionSetUsingWeapon(true);
                mainPlayer->actionSetTargetDirection(Vector2(mousePos) - mainPlayer->getCenterPos());
                break;
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

/**
 * Handle mouse release event
 */
void MainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (mainPlayer) {
        switch (event->button()) {
            case Qt::LeftButton:
                // Player action: player is no longer using weapon
                mainPlayer->actionSetUsingWeapon(false);
                break;
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

/**
 * Handle mouse move event
 */
void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (mainPlayer) {
        QPointF mousePos = event->scenePos();
        mainPlayer->actionSetTargetDirection(Vector2(mousePos) - mainPlayer->getCenterPos());
    }
}

/**
 * Handle key press event
 */
void MainScene::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_Q:
            mainPlayer->actionSetLeftMovement(1);
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            mainPlayer->actionSetRightMovement(1);
            break;
        case Qt::Key_Up:
        case Qt::Key_Z:
            mainPlayer->actionSetUpMovement(1);
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            mainPlayer->actionSetDownMovement(1);
            break;
        case Qt::Key_E:
            mainPlayer->actionSetGrabPress(true);
            break;
        case Qt::Key_A:
            mainPlayer->actionChangeWeapon();
            break;
    }

    QGraphicsScene::keyPressEvent(event);
}

/**
 * Handle key release event
 */
void MainScene::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_Q:
            mainPlayer->actionSetLeftMovement(0);
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            mainPlayer->actionSetRightMovement(0);
            break;
        case Qt::Key_Up:
        case Qt::Key_Z:
            mainPlayer->actionSetUpMovement(0);
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            mainPlayer->actionSetDownMovement(0);
            break;
        case Qt::Key_E:
            mainPlayer->actionSetGrabPress(false);
            break;
    }

    QGraphicsScene::keyReleaseEvent(event);
}