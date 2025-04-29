#include "../../include/entity/mob.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Mob::Mob() {
    damage = 0;
    target = nullptr;
}

/** Copy constructor
 * 
 * @param other Another Mob
 */
Mob::Mob(const Mob& other) : LivingEntity(other), damage(other.damage), target(other.target) { }

/**
 * Constructor
 * 
 * @param life Starting life of entity
 * @param damage Melee damage this mob deals
 * @param speed Speed of mob
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 * @param team The team this entity belongs to
 * @param playerTarget The target of this mob
 */
Mob::Mob(const qreal life, const qreal damage, const qreal speed, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite, Teams::Team team, Player* playerTarget) :
    LivingEntity(life, speed, position, dimensions, sprite, team), damage(damage), target(playerTarget)
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
void Mob::onCollide(Entity* other) {
    // If colliding with a player, change target
    if (Player* player = dynamic_cast<Player*>(other)) {
        target = player;
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity or not
 */
bool Mob::onUpdate(qint64 deltaTime) {
    if (target) {
        Vector2 movement = (target->getPos() - getPos()).normalized();
        movement = movement * getSpeedMultiplier() * getSpeed() * deltaTime;
        setPos(getPos() + movement);
    }
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