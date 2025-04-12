#include "../../include/entity/item.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
Item::Item() { }

/**
 * Copy constructor
 * 
 * @param other The entity to copy
 */
Item::Item(const Item& other) : Entity(other), itemType(other.itemType) { }

/**
 * Constructor
 * 
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Item::Item(const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite) : Entity(position, dimensions, sprite) { }

/**
 * Destructor
 */
Item::~Item() { }

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