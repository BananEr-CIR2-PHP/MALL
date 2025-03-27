#include "../include/entity.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Entity::Entity() {
    position = Vector2::zero;
    dimensions = Vector2::zero;
    sprite = nullptr;

    updateCollisionBox();
}

/**
 * Copy constructor
 * 
 * @param other The entity to copy
 */
Entity::Entity(const Entity& other) {
    position = other.position;
    dimensions = other.dimensions;
    sprite = other.sprite;      // No copy here because Entities don't own sprite management/deletion
    entityType = other.entityType;

    updateCollisionBox();
}

/**
 * Constructor
 * 
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Entity::Entity(const Vector2 position, const Vector2 dimensions, const Sprite* sprite) : position(position), dimensions(dimensions), sprite(sprite) {
    updateCollisionBox();
}

/**
 * Destructor
 */
Entity::~Entity() {}

// --- PRIVATE METHODS ---

/**
 * Updates collision box. Should be called whenever position or dimension is modified.
 */
void Entity::updateCollisionBox() {
    collisionTL = position - dimensions/2;
    collisionBR = position + dimensions/2;
}

// --- GETTERS ---

/**
 * Get position of entity
 * 
 * @return Position of the entity
 */
Vector2 Entity::getPos() const {
    return position;
}

/**
 * Get dimensions of collision box.
 * Collision box is centered on entity position
 * 
 * @return Dimensions of the collision box
 */
Vector2 Entity::getDims() const {
    return dimensions;
}

/**
 * Get the type of entity
 * 
 * @return Type of entity
 */
Entity::EntityType Entity::getType() const {
    return entityType;
}

// --- SETTERS ---

/**
 * Set position of entity
 * 
 * @param pos New position of the entity
 */
void Entity::setPos(const Vector2 pos) {
    position = pos;
    updateCollisionBox();
}

void Entity::setDims(const Vector2 dims) {
    if (dims.getX() >= 0 && dims.getY() >= 0) {
        dimensions = dims;
        updateCollisionBox();
    }
    else {
        throw std::runtime_error("Dimensions cannot be negative!");
    }
}

// --- METHODS ---

/**
 * Detects collision with given entity
 * 
 * @param other The entity to detect collision with
 * @return Whether collision occured or not.
 */
bool Entity::collidesWith(const Entity& other) const {
    // Is true if this collision box is NOT: at right of this other box or at right AND is NOT: above other box or below
    return !(
        ( collisionTL.getX() > other.collisionBR.getX() || collisionBR.getX() < other.collisionTL.getX() ) &&
        ( collisionTL.getY() < other.collisionBR.getY() || collisionBR.getY() > other.collisionTL.getY() )
    );
}