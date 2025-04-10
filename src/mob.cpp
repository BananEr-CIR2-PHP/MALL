#include "../include/mob.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Mob::Mob() { }

/** Copy constructor
 * 
 * @param other Another Mob
 */
Mob::Mob(const Mob& other) : LivingEntity(other) { }

/**
 * Constructor
 * 
 * @param life Starting life of entity
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Mob::Mob(qreal life, const Vector2 position, const Vector2 dimensions, Sprite* sprite) : LivingEntity(life, position, dimensions, sprite) { }

/**
 * Destructor
 */
Mob::~Mob() { }

// --- INHERITED METHODS ---

/**
 * Called on death of this entity
 */
void Mob::onDeath() { }

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void Mob::onCollide(Entity* other) { }

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 */
void Mob::onUpdate(qint64 deltaTime) { }