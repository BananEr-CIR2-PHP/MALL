#include "../include/entity.hpp"

// --- Constructors/Destructors ---

/**
 * Default constructor
 */
Entity::Entity() {
    position = Vector2::zero;
}

/**
 * Copy constructor
 * 
 * @param other The entity to copy
 */
Entity::Entity(const Entity& other) {
    position = other.position;
}

/**
 * Constructor
 * 
 * @param pos Starting position of entity
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Entity::Entity(const Vector2 position, const Sprite* sprite) : position(position), sprite(sprite) { }

/**
 * Destructor
 */
Entity::~Entity() {}

/**
 * Get position of entity
 * 
 * @return Position of the entity
 */
Vector2 Entity::getPos() const {
    return position;
}

/**
 * Set position of entity
 * 
 * @param pos New position of the entity
 */
void Entity::setPos(const Vector2 pos) {
    position = pos;
}