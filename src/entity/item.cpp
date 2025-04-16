#include "../../include/entity/item.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
Item::Item() {
    itemType = ItemType::None;
}

/**
 * Copy constructor
 * 
 * @param other The entity to copy
 */
Item::Item(const Item& other) : Entity(other), itemType(other.itemType) {
    itemWeapon = other.itemWeapon->clone();
}

/**
 * Constructor
 * 
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param itemType Type of item 
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Item::Item(const Vector2 position, const Vector2 dimensions, ItemType::ItemType itemType, Sprites::SpriteImage sprite) :
    Entity(position, dimensions, sprite), itemType(itemType) 
{
    
}

/**
 * Destructor
 */
Item::~Item() {
    delete itemWeapon;
}

// --- INHERITED METHODS ---

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void Item::onCollide(Entity* other) {
    // Nothing to do here.
    // Pickup item by Player is handled by Player
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity
 */
bool Item::onUpdate(qint64 deltaTime) {
    return false;
}

/**
 * Get next Entity this entity wants to spawn
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* Item::getSpawned() {
    return nullptr;
}

// --- GETTERS ---

/**
 * Get the type of this item
 * 
 * @return Type of this item
 */
ItemType::ItemType Item::getType() const {
    return itemType;
}

/**
 * Know whether this item can give a weapon or not.
 * If true, this indicates that this item has a weapon it can give when calling giveWeapon()
 * 
 * @return Whether this item can give a weapon or not.
 */
bool Item::hasWeapon() const {
    return (itemWeapon!=nullptr);
}

/**
 * Get the weapon contained in this item.
 * The weapon becomes managed by the caller of this method.
 * 
 * @return The weapon contained in this item.
 */
Weapon* Item::takeWeapon() {
    Weapon* givenWeapon = itemWeapon;
    itemWeapon = nullptr;
    return givenWeapon;
}

// --- SETTERS ---

/**
 * Set a new weapon for this item to contain.
 * If this item already contained a weapon, the previous weapon is deleted.
 * The given weapon becomes managed by this item.
 * 
 * @param newWeapon The new weapon to contain in this item
 */
void Item::setWeapon(Weapon* newWeapon) {
    if (itemWeapon) {
        delete itemWeapon;
    }
    itemWeapon = newWeapon;
}