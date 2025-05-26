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
#define PLAYER_DIMS Vector2(100, 100)
#define PLAYER_SPEED 0.1

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor.
 */
MainScene::MainScene(QObject* parent, int fps) : QGraphicsScene(parent) {
    // Scene options
    setSceneRect(-50000, -50000, 100000, 100000);       // Scene size
    setFocus();
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
        Sprites::SpriteImage::Player,
        Teams::Player
    );
    pl->grabWeapon(new Gun(WeaponType::GunType::DesertEagle), Inventory::WeaponSlot_1);
    setControlledPlayer(pl);
    addEntity(pl);

    gameScore = 0;

    // Activate game loop
    sceneTime = 0;
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
    sceneTime += deltaTime;
    checkCollisions();
    updateEntities();
    cleanupScene();
    spawnMobWave();
    if(mainPlayer!=nullptr){
        emit playerMoved(getMainPlayer());
    }
    

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

/**
 * Get the main player of the scene
 *
 * @return the main player
 */
Player* MainScene::getMainPlayer() {
    return mainPlayer;
}

void MainScene::setBackgroundTile(const QString &image_path){
    this->m_tileImage = QPixmap(image_path);
}

void MainScene::drawBackground(QPainter *painter, const QRectF &rect) {
    if (m_tileImage.isNull())
        return;
    int startX = qFloor(rect.left() / m_tileImage.width()) * m_tileImage.width();
    int startY = qFloor(rect.top() / m_tileImage.height()) * m_tileImage.height();
    int numTilesX = qCeil(rect.width() / m_tileImage.width()) + 1;
    int numTilesY = qCeil(rect.height() / m_tileImage.height()) + 1;
    for (int y = 0; y < numTilesY; ++y) {
        for (int x = 0; x < numTilesX; ++x) {
            painter->drawPixmap(
                startX + x * m_tileImage.width(),
                startY + y * m_tileImage.height(),
                m_tileImage
            );
        }
    }
}