#include "../../include/entity/mob.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Mob::Mob() {
    damage = 0;
}

/** Copy constructor
 * 
 * @param other Another Mob
 */
Mob::Mob(const Mob& other) : LivingEntity(other), damage(other.damage) { }

/**
 * Constructor
 * 
 * @param life Starting life of entity
 * @param damage Melee damage this mob deals
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 * @param team The team this entity belongs to
 */
Mob::Mob(const qreal life, const qreal damage, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite, Teams::Team team) :
    LivingEntity(life, position, dimensions, sprite, team), damage(damage)
{

}

/**
 * Destructor
 */
Mob::~Mob() { }

// --- INHERITED METHODS ---

/**
 * Called on death of this entity
 */
void Mob::onDeath() {
    setDeleted(true);
}

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
 * @return Whether this entity wants to spawn another entity or not
 */
bool Mob::onUpdate(qint64 deltaTime) {
    return LivingEntity::onUpdate(deltaTime);
}

/**
 * Get next Entity this entity wants to spawn
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* Mob::getSpawned() {
    return nullptr;
}

/**
 * Get melee damage of this mob
 * 
 * @return Melee damage of this mob
 */
qreal Mob::getDamage() const {
    return damage;
}