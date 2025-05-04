#include "../../include/entity/player.hpp"
#include "../../include/entity/mob.hpp"
#include "../../include/entity/missile.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Player::Player() {
    energy = 0;
    maxEnergy = 0;
    gold = 0;
}

/** Copy constructor
 * 
 * @param other Another Player
 */
Player::Player(const Player& other) : LivingEntity(other), energy(other.energy), maxEnergy(other.maxEnergy), gold(other.gold) { }

/**
 * Constructor
 * 
 * @param life Starting life of entity. Also used as starting max life. Use Player::DefaultLife for default
 * @param energy Starting energy of entity. Also starting max energy. Use Player::DefaultEnergy for default
 * @param gold Starting gold of player.
 * @param speed Speed of player
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 * @param team The team this entity belongs to
 */
Player::Player(const qreal life, const qint64 energy, const qint64 gold, const qreal speed, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite, Teams::Team team) :
    LivingEntity(life, speed, position, dimensions, sprite, team), energy(energy), maxEnergy(energy), gold(gold)
{

}

/**
 * Destructor
 */
Player::~Player() {
    delete weapon1;
    delete weapon2;
    delete droppedWeapon;
}

// --- GETTERS ---

/**
 * Get current energy of player
 * 
 * @return energy of player
 */
qint64 Player::getEnergy() const {
    return energy;
}

/**
 * Get max energy of player
 * 
 * @return max energy of player
 */
qint64 Player::getMaxEnergy() const {
    return maxEnergy;
}

/**
 * Get the player amount of gold
 * 
 * @return Player gold amount
 */
qint64 Player::getGold() const {
    return gold;
}

// --- SETTERS ---

/**
 * Set new energy value for player. Automatically clamped in [0; maxEnergy]
 * 
 * @param newEnergy New player energy
 */
void Player::setEnergy(const qint64 newEnergy) {
    if (newEnergy > maxEnergy) {
        energy = maxEnergy;     // Refill energy
    }
    else if (newEnergy < 0) {
        energy = 0;
    }
    else {
        energy = newEnergy;
    }
}

/**
 * Consume the given amount of energy
 * 
 * @param consumedEnergy The amount of energy to consume
 */
void Player::consumeEnergy(const qint64 consumedEnergy) {
    setEnergy(energy - consumedEnergy);
}

/**
 * Set a new maximum energy
 * 
 * @param newMaxEnergy New max energy value
 */
void Player::setMaxEnergy(const qint64 newMaxEnergy) {
    if (newMaxEnergy < 0) {
        maxEnergy = 0;
    }
    else {
        maxEnergy = newMaxEnergy;
    }
    
    // Energy shouldn't be greater than maxEnergy
    if (energy > maxEnergy) {
        energy = maxEnergy;
    }
}

/**
 * Add the given amount of gold to player's inventory
 * 
 * @param newGold The amount of gold to add
 */
void Player::addGold(const qint64 amount) {
    gold += amount;
}

/**
 * Gather the given item
 * May fail if non-human circumstances are met (e.g.: gather more than 1 item per frame)
 * 
 * @param item The item to gather
 * @return True if succeeded, false otherwise. May fail if non-human circumstances are met (more than 1 grab per frame)
 */
bool Player::gatherItem(Item* item) {
    bool succeeded = false;
    if (!isDead) {
        // Player wants to gather only 1 item per key input
        grabKeyPressed = false;

        switch (item->getType()) {
            case ItemType::Gold:
                addGold(item->getStrength());
                succeeded = true;
                break;
            case ItemType::HPPotion:
                setLife(getLife() + item->getStrength());
                succeeded = true;
                break;
            case ItemType::EnergyPotion:
                setEnergy(getEnergy() + item->getStrength());
                succeeded = true;
                break;
            case ItemType::Weapon:
                // If item has a weapon and player can drop its active weapon
                if (item->hasWeapon() && dropWeapon(activeWeaponSlot)) {
                    grabWeapon(item->takeWeapon(), activeWeaponSlot);
                    succeeded = true;
                }
                else {
                    succeeded = false;
                }
                break;
            default:
                succeeded = false;
                break;
        }
    }
    else {
        succeeded = false;
    }
    if (succeeded) {
        item->setDeleted(true);
    }
    return succeeded;
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
        prepareGeometryChange();
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
        // If a weapon is held in slot, held weapon becomes dropped weapon
        prepareGeometryChange();
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

    if (!isDead) {
        // Decrease weapon cooldown
        if (weaponDelay > 0) {
            weaponDelay -= deltaTime;
        }
        else if (useWeaponKeyPressed) {
            // Eventually use weapon
            actionUseWeapon(targetDir);
        }
        // Spawn shot bullets
        if (Weapon* activeWeapon = getActiveWeapon()) {
            wantSpawn = wantSpawn || activeWeapon->wantSpawn();
        }

        // Build direction based on key presses
        Vector2 direction = Vector2(
            (rightKeyPressed ? 1 : 0) - (leftKeyPressed ? 1 : 0),
            (downKeyPressed ? 1 : 0) - (upKeyPressed ? 1 : 0)
        ).normalized();

        // Update looking direction
        if (direction.getX() < 0) {
            setLookingLeft(true);
        }
        else if (direction.getX() > 0) {
            setLookingLeft(false);
        }

        // Update position
        setPos(getPos() + direction * getSpeed() * getSpeedMultiplier() * deltaTime);
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
        // Get weapon item spawn position: top left corner of weapon
        Vector2 itemPos;
        if (getLookingLeft()) {
            itemPos = Vector2(
                getPos().getX() + getDims().getX() - droppedWeapon->getDims().getX(),
                getPos().getY() + getDims().getY()/2 - droppedWeapon->getDims().getY()/2
            );
        }
        else {
            itemPos = Vector2(
                getPos().getX(),
                getPos().getY() + getDims().getY()/2 - droppedWeapon->getDims().getY()/2 
            );
        }

        // Drop the weapon
        Item* drop = new Item(itemPos, droppedWeapon->getDims(), ItemType::Weapon, Sprites::SpriteImage::None);
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
 * Get boundingRect of this player (relative to player position)
 * 
 * @return bounding rect of this player
 */
QRectF Player::boundingRect() const {
    Weapon* activeWeapon = getActiveWeapon();
    if (activeWeapon) {
        Vector2 rectDims = getDims().maximum(activeWeapon->getDims());
        return QRectF(0, 0, rectDims.getX(), rectDims.getY());
    }
    else {
        return Entity::boundingRect();
    }
}

/**
 * Paint player on scene
 * 
 * @param painter Painter to draw entity on
 */
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem* styleOption, QWidget* widget) {
    // If looking left, mirror player render along y axis
    if (getLookingLeft()) {
        Vector2 dims = getDims();
        QPointF center = QPointF(dims.getX()/2, dims.getY()/2);
        painter->translate(center);
        painter->scale(-1, 1);  // Flip image on y axis
        painter->translate(-center);
    }

    // Draw player sprite if it exists
    if (sprite != nullptr) {
        QSharedPointer<QImage> image = sprite->getImage();
        if (image != nullptr) {
            Vector2 dims = getDims();
            painter->drawImage(QRectF(0, 0, dims.getX(), dims.getY()), *image);
        }
    }
    
    // Draw active weapon (if any)
    Weapon* activeWeapon = getActiveWeapon();
    if (activeWeapon) {
        if (const Sprite* weaponSprite = activeWeapon->getSprite()) {
            if (QSharedPointer<QImage> image = weaponSprite->getImage()) {
                Vector2 weaponDims = activeWeapon->getDims();
                painter->drawImage(QRectF(
                    0,
                    getDims().getY()/2 - weaponDims.getY()/2,
                    weaponDims.getX(),
                    weaponDims.getY()),
                    *image);
            }
        }
    }
}

/**
 * Redefine shape of player
 * Player colliding zone does not include weapon
 * 
 * @return shape of the player
 */
QPainterPath Player::shape() const {
    QPainterPath path;
    Vector2 dims = getDims();
    path.addRect(0, 0, dims.getX(), dims.getY());
    return path;
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
        qint64 consumption = heldWeapon->getConsumption();
        if (consumption <= getEnergy()) {
            // Get attacking position: player should attack at the tip of the weapon
            Vector2 playerPos = getPos();
            Vector2 playerDims = getDims();
            Vector2 weaponDims = getActiveWeapon()->getDims();
            Vector2 attackPos;
            if (getLookingLeft()) {
                attackPos = Vector2(
                    playerPos.getX() + playerDims.getX() - weaponDims.getX(),
                    playerPos.getY() + playerDims.getY()/2
                );
            }
            else {
                attackPos = Vector2(
                    playerPos.getX() + weaponDims.getX(),
                    playerPos.getY() + playerDims.getY()/2
                );
            }

            // Attack at the correct position and direction
            heldWeapon->attack(attackPos, direction, team);
            consumeEnergy(consumption);
            weaponDelay = heldWeapon->getDelay();
        }
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
    prepareGeometryChange();
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
    update();
}

/**
 * Player action:
 * Set use weapon key press state
 * 
 * @param isUsingWeapon Weapon usage state
 */
void Player::actionSetUsingWeapon(const bool isUsingWeapon) {
    useWeaponKeyPressed = isUsingWeapon;
}

/**
 * Player action:
 * Set direction to fire to.
 * 
 * @param direction Target direction. Does not need to be normalized.
 */
void Player::actionSetTargetDirection(const Vector2 direction) {
    targetDir = direction.normalized();
}