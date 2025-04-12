#include "../include/player.hpp"
#include "../include/mob.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Player::Player() {
    initFlags();
}

/** Copy constructor
 * 
 * @param other Another Player
 */
Player::Player(const Player& other) : LivingEntity(other) {
    initFlags();
}

/**
 * Constructor
 * 
 * @param life Starting life of entity
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Player::Player(qreal life, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite) : LivingEntity(life, position, dimensions, sprite) {
    initFlags();
}

/**
 * Destructor
 */
Player::~Player() { }

/**
 * Gather the given item
 * 
 * @param item The item to gather
 */
void Player::gatherItem(Item* item) {
    if (!isDead) {
        item->setDeleted(true);
        // TODO: add to player inventory
    }
}

/**
 * Initialize player flags for inputs listening 
 */
void Player::initFlags() {
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

// --- INHERITED METHODS ---

/**
 * Called on death of this entity
 */
void Player::onDeath() { }

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void Player::onCollide(Entity* other) {
    if (Item* item = dynamic_cast<Item*>(other)) {
        gatherItem(item);
    }
    else if (Mob* mob = dynamic_cast<Mob*>(other)) {
        takeDamage(mob->getDamage());
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity or not
 */
bool Player::onUpdate(qint64 deltaTime) {
    bool wantSpawn = LivingEntity::onUpdate(deltaTime);

    if (!isDead) {
        // Build direction based on key presses
        Vector2 direction = Vector2(
            (rightKeyPressed ? 1 : 0) - (leftKeyPressed ? 1 : 0),
            (downKeyPressed ? 1 : 0) - (upKeyPressed ? 1 : 0)
        ).normalized();

        // Update position
        setPos(getPos() + direction * PLAYER_SPEED * getSpeedMultiplier() * deltaTime);
    }

    return wantSpawn;
}

/**
 * Get next Entity this entity wants to spawn
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* Player::getSpawned() {
    return nullptr;
}

// --- INPUT EVENTS ---

/**
 * Handle key press (keyboard event)
 */
void Player::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_Q:
            leftKeyPressed = true;
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            rightKeyPressed = true;
            break;
        case Qt::Key_Up:
        case Qt::Key_Z:
            upKeyPressed = true;
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            downKeyPressed = true;
            break;
    }
}

/**
 * Handle key release (keyboard event)
 */
void Player::keyReleaseEvent(QKeyEvent* event) {
    // TODO: solve bug: player event focus
    switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_Q:
            leftKeyPressed = false;
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            rightKeyPressed = false;
            break;
        case Qt::Key_Up:
        case Qt::Key_Z:
            upKeyPressed = false;
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            downKeyPressed = false;
            break;
    }
}

/**
 * Handle mouse press (mouse event)
 */
void Player::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mousePressEvent(event); // Call the base class to ensure normal behavior
}

/**
 * Handle mouse release (mouse event)
 */
void Player::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event); // Call the base class to ensure normal behavior
}

/**
 * Handle mouse movement (mouse event)
 */
void Player::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseMoveEvent(event); // Call the base class to ensure normal behavior
}
