#include "../../include/entity/missile.hpp"
#include "../../include/entity/livingEntity.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
Missile::Missile() {
    velocity = Vector2::zero;
    lifetime = 0;
    damage = 0;
    pierceEntities = false;
}

/**
 * Copy constructor
 * 
 * @param other The missile to copy
 */
Missile::Missile(const Missile& other) : Entity(other), velocity(other.velocity), lifetime(other.lifetime), damage(other.damage), pierceEntities(other.pierceEntities) { }

/**
 * Constructor
 * 
 * @param velocity Speed and direction this missile moves to
 * @param range Max distance to travel before despawn
 * @param damage Damage this missile deals when hitting a living entity
 * @param pierceEntities Whether this missile despawns on first entity hit or not
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 * @param team The team this entity belongs to
 */
Missile::Missile(const Vector2 velocity, const qreal range, const qreal damage, const bool pierceEntities, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite, Teams::Team team) : 
    Entity(position, dimensions, sprite, team), velocity(velocity), lifetime(range), damage(damage), pierceEntities(pierceEntities) { }

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
void Missile::onCollide(Entity* other) {
    if (LivingEntity* entity = dynamic_cast<LivingEntity*>(other)) {
        if (entity->getTeam() != getTeam()) {
            entity->takeDamage(damage);

            // If does not pierce entities, delete this entity
            if (!pierceEntities) {
                setDeleted(true);
            }
        }
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity or not
 */
bool Missile::onUpdate(qint64 deltaTime) {
    Vector2 travel = velocity*deltaTime;
    setPos(getPos() + travel);

    // Handle max missile travel distance
    lifetime -= travel.magnitude();
    if (lifetime < 0) {
        setDeleted(true);
    }
    return false;
}

/**
 * Get next Entity this entity wants to spawn
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* Missile::getSpawned() {
    return nullptr;
}