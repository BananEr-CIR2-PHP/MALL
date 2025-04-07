#include "../include/missile.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
Missile::Missile() {
    velocity = Vector2::zero;
    entityType = EntityType::Missile;
}

/**
 * Copy constructor
 * 
 * @param other The missile to copy
 */
Missile::Missile(const Missile& other) : Entity(other), velocity(other.velocity) {
    entityType = EntityType::Missile;
}

/**
 * Constructor
 * 
 * @param velocity Speed and direction this missile moves to
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Missile::Missile(const Vector2 velocity, const Vector2 position, const Vector2 dimensions, Sprite* sprite) : Entity(position, dimensions, sprite), velocity(velocity) {
    entityType = EntityType::Missile;
}

/**
 * Destructor
 */
Missile::~Missile() { }

// --- GETTERS ---

/**
 * Get velocity of missile
 * 
 * @return Velocity of the missile
 */
Vector2 Missile::getSpeed() const {
    return velocity;
}

// --- SETTERS ---

/**
 * Set velocity of missile
 * 
 * @param speed Velocity of the missile
 */
void Missile::setSpeed(const Vector2 speed) {
    velocity = speed;
}

// --- INHERITED METHODS ---

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void Missile::onCollide(Entity& other) { }

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 */
void Missile::onUpdate(qint64 deltaTime) {
    setPos(getPos() + velocity*deltaTime);
}