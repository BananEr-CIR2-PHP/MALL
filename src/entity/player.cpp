#include "../../include/entity/player.hpp"
#include "../../include/entity/mob.hpp"
#include "../../include/entity/missile.hpp"

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
 * @param team The team this entity belongs to
 */
Player::Player(qreal life, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite, Teams::Team team) : LivingEntity(life, position, dimensions, sprite, team) {
    initFlags();
}

/**
 * Destructor
 */
Player::~Player() {
    delete weapon1;
    delete weapon2;
    delete droppedWeapon;
}

/**
 * Gather the given item
 * May fail if non-human circumstances are met (e.g.: gather more than 1 item per frame)
 * 
 * @param item The item to gather
 * @return True if succeeded, false otherwise. May fail if non-human circumstances are met (more than 1 grab per frame)
 */
bool Player::gatherItem(Item* item) {
    if (!isDead) {
        // Player wants to gather only 1 item per key input
        grabKeyPressed = false;

        switch (item->getType()) {
            case ItemType::Gold:
                // TODO
                std::cout << "Gather GOLD!" << std::endl;
                break;
            case ItemType::HPPotion:
                // TODO
                std::cout << "Healing!" << std::endl;
                break;
            case ItemType::Weapon:
                // If item has a weapon and player can drop its active weapon
                if (item->hasWeapon() && dropWeapon(activeWeaponSlot)) {
                    grabWeapon(item->takeWeapon(), activeWeaponSlot);
                    item->setDeleted(true);
                    return true;
                }
                else {
                    return false;
                }
        }
    }
    return false;
}

/**
 * Grab the given weapon and add it to the given inventory slot.
 * May fail if non-human circumstances are met (e.g.: more than 1 grab per frame)
 * If succeeded, given weapon is now managed by this player.
 * 
 * @param weapon The weapon to grab
 * @param slot The slot to add the weapon to
 * @return True if succeeded, false otherwise
 */
bool Player::grabWeapon(Weapon* weapon, Inventory::WeaponSlot slot) {
    // If successfully dropped the active weapon, grab the weapon
    if (!hasWeapon(slot)) {
        switch (slot) {
            case Inventory::WeaponSlot_1:
                weapon1 = weapon;
                break;
            case Inventory::WeaponSlot_2:
                weapon2 = weapon;
                break;
        }
        return true;
    }
    else {
        return false;
    }
}

/**
 * Drop the weapon that is in the given inventory slot
 * May fail if non-human circumstances are met (e.g.: more than 1 grab per frame)
 * 
 * @param slot The slot to drop the weapon from
 * @return True if succeeded, false otherwise
 */
bool Player::dropWeapon(Inventory::WeaponSlot slot) {
    // If droppedWeapon is already set, return false (method failed)
    if (droppedWeapon) {
        return false;
    }
    else {
        // If a weapon is held in slot, held weapon becomes dropped wweapon
        switch (slot) {
            case Inventory::WeaponSlot_1:
                droppedWeapon = weapon1;
                weapon1 = nullptr;

                break;
            case Inventory::WeaponSlot_2:
                droppedWeapon = weapon2;
                weapon2 = nullptr;
                break;
        }
        return true;
    }
}

/**
 * Know whether if player has a weapon in given slot
 * 
 * @return True if a weapon is in the given slot, false otherwise
 */
bool Player::hasWeapon(Inventory::WeaponSlot slot) const {
    switch (slot) {
        case Inventory::WeaponSlot_1:
            return weapon1 != nullptr;
        
        case Inventory::WeaponSlot_2:
            return weapon2 != nullptr;

        default:
            return false;
    }
}

/**
 * Get active weapon of Player
 * 
 * @return Active weapon
 */
Weapon* Player::getActiveWeapon() const {
    switch (activeWeaponSlot) {
        case Inventory::WeaponSlot_1:
            return weapon1;
        case Inventory::WeaponSlot_2:
            return weapon2;
        default:
            return nullptr;     // Should never happen
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
        if (grabKeyPressed) {
            gatherItem(item);
        }
    }
    else if (Mob* mob = dynamic_cast<Mob*>(other)) {
        if (mob->getTeam() != getTeam()) {
            takeDamage(mob->getDamage());
        }
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity or not
 */
bool Player::onUpdate(qint64 deltaTime) {
    bool wantSpawn = LivingEntity::onUpdate(deltaTime) || droppedWeapon;
    if (Weapon* activeWeapon = getActiveWeapon()) {
        wantSpawn = wantSpawn || activeWeapon->wantSpawn();
    }

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
    // If a weapon was dropped
    if (droppedWeapon) {
        Item* drop = new Item(getPos(), Vector2(30, 30), ItemType::Weapon, Sprites::SpriteImage::Player);
        drop->setWeapon(droppedWeapon);
        droppedWeapon = nullptr;
        return drop;
    }
    else {
        // Take spawn requests from active weapon
        switch (activeWeaponSlot) {
            case Inventory::WeaponSlot_1:
                return weapon1->getSpawned();
            case Inventory::WeaponSlot_2:
                return weapon2->getSpawned();
        }
        return nullptr;
    }
}

/**
 * Paint player on scene
 * 
 * @param painter Painter to draw entity on
 */
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem* styleOption, QWidget* widget) {
    // Draw player sprite if it exists
    if (sprite != nullptr) {
        QSharedPointer<QImage> image = sprite->getImage();
        if (image != nullptr) {
            painter->drawImage(boundingRect(), *image);
        }
    }
    
    Weapon* activeWeapon = getActiveWeapon();
    if (activeWeapon) {
        if (const Sprite* weaponSprite = activeWeapon->getSprite()) {
            if (QSharedPointer<QImage> image = weaponSprite->getImage()) {
                Vector2 weaponDims = activeWeapon->getDims();
                painter->drawImage(QRectF(0, 0, weaponDims.getX(), weaponDims.getY()), *image);
            }
        }
    }
}

// --- INPUT EVENTS ---

/**
 * Player action:
 * Use active weapon, if player is holding any
 */
void Player::actionUseWeapon(Vector2 direction) {
    Weapon* heldWeapon;
    switch (activeWeaponSlot) {
        case Inventory::WeaponSlot_1:
            heldWeapon = weapon1;
            break;
        
        case Inventory::WeaponSlot_2:
            heldWeapon = weapon2;
            break;
        
        default:
            heldWeapon = nullptr;
            break;
    }

    // If player is holding a weapon
    if (heldWeapon) {
        heldWeapon->attack(getPos(), direction, team);
    }
}

/**
 * Player action:
 * Set left movement
 * 
 * @param mvt Left movement. Must be between 0 and 1.
 */
void Player::actionSetLeftMovement(qreal mvt) {
    if (mvt >= 0 && mvt <= 1) {
        leftKeyPressed = mvt;
    }
}

/**
 * Player action:
 * Set right movement
 * 
 * @param mvt Right movement. Must be between 0 and 1.
 */
void Player::actionSetRightMovement(qreal mvt) {
    if (mvt >= 0 && mvt <= 1) {
        rightKeyPressed = mvt;
    }
}

/**
 * Player action:
 * Set up movement
 * 
 * @param mvt Up movement. Must be between 0 and 1.
 */
void Player::actionSetUpMovement(qreal mvt) {
    if (mvt >= 0 && mvt <= 1) {
        upKeyPressed = mvt;
    }
}

/**
 * Player action:
 * Set down movement
 * 
 * @param mvt Down movement. Must be between 0 and 1.
 */
void Player::actionSetDownMovement(qreal mvt) {
    if (mvt >= 0 && mvt <= 1) {
        downKeyPressed = mvt;
    }
}

/**
 * Player action:
 * Set grab key press state
 * 
 * @param isGrabbing Grab state
 */
void Player::actionSetGrabPress(bool isGrabbing) {
    grabKeyPressed = isGrabbing;
}

/**
 * Player action:
 * Change active weapon
 */
void Player::actionChangeWeapon() {
    // Using a switch here because it's easier to add new slots this way.
    // TODO: need to test whether if we need to call player rect reload on weapon change, or not
    if (!isDead) {
        switch (activeWeaponSlot) {
            case Inventory::WeaponSlot_1:
                activeWeaponSlot = Inventory::WeaponSlot_2;
                if (weapon1) {
                    weapon1->destroySpawned();
                }
                break;
            case Inventory::WeaponSlot_2:
                activeWeaponSlot = Inventory::WeaponSlot_1;
                if (weapon2) {
                    weapon2->destroySpawned();
                }
                break;
        }
    }
}