#include "../include/entity/item.hpp"
#include "../include/entity/player.hpp"
#include "../include/entity/mob.hpp"
#include "../include/entity/rocket.hpp"
#include "../include/weapon/rocketLauncher.hpp"
#include "../include/mainScene.hpp"

#define PLAYER_SPEED 0.1

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor.
 */
MainScene::MainScene(QObject* parent, int fps) : QGraphicsScene(parent) {
    // Scene options
    setSceneRect(0, 0, 900, 900);       // Scene size
    setItemIndexMethod(QGraphicsScene::NoIndex);      // Collision detection method : linear
    entities = new QList<Entity*>();

    // Scene setup example
    Item* it = new Item(Vector2(50, 600), Vector2(50, 50), ItemType::Weapon, Sprites::SpriteImage::Coin);
    it->setWeapon(new RocketLauncher(WeaponType::RocketLauncherType::Bazooka));
    addEntity(it);
    it = new Item(Vector2(50, 300), Vector2(50, 50), ItemType::Weapon, Sprites::SpriteImage::Coin);
    it->setWeapon(new Gun(WeaponType::GunType::DesertEagle));
    addEntity(it);
    it = new Item(Vector2(50, 50), Vector2(50, 50), ItemType::Weapon, Sprites::SpriteImage::Coin);
    it->setWeapon(new Gun(WeaponType::GunType::DesertEagle));
    addEntity(it);
    Player* pl = new Player(20, 2000, PLAYER_SPEED, Vector2(300, 300), Vector2(100, 100), Sprites::SpriteImage::Player, Teams::Player);
    setControlledPlayer(pl);
    addEntity(pl);
    Mob* mob = new Mob(20, 1, 0.05, Vector2(900, 900), Vector2(60, 60), Sprites::SpriteImage::Player, Teams::Ennemy, pl);
    addEntity(mob);

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
                // mainPlayer->actionUseWeapon(Vector2(mousePos.x(), mousePos.y()) - mainPlayer->getPos());
                mainPlayer->actionSetUsingWeapon(true);
                mainPlayer->actionSetTargetDirection(Vector2(mousePos) - mainPlayer->getPos());
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
        mainPlayer->actionSetTargetDirection(Vector2(mousePos) - mainPlayer->getPos());
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