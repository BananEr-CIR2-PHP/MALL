#include "../include/player.hpp"
#include "../include/item.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
Item::Item() {
    entityType = EntityType::Item;
}

/**
 * Copy constructor
 * 
 * @param other The entity to copy
 */
Item::Item(const Item& other) : Entity(other), itemType(other.itemType) {
    entityType = EntityType::Item;
}

/**
 * Constructor
 * 
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Item::Item(const Vector2 position, const Vector2 dimensions, Sprite* sprite) : Entity(position, dimensions, sprite) {
    entityType = EntityType::Item;
}

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
void Item::onCollide(Entity& other) {
    if (other.getType() == EntityType::Player) {
        // IMPORTANT: Safe as long as ALL copy constructors of classes above in hierarchy are protected
        Player* player = static_cast<Player*>(&other);
        player->gatherItem(itemType);
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 */
void Item::onUpdate(qint64 deltaTime) { }