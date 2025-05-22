#include "../../include/entity/rocket.hpp"
#include "../../include/entity/livingEntity.hpp"

// --- CONSTRUCTORS/DESTRUCTOR ---

/**
 * Default constructor
 */
Rocket::Rocket() {
    effect = nullptr;
    effectRange = 0;
}

/**
 * Copy constructor
 * 
 * @param other Another Rocket
 */
Rocket::Rocket(const Rocket& other) : Missile(other), effectRange(other.effectRange) {
    effect = new Effect(*(other.effect));
}

/**
 * Constructor
 * 
 * @param effect Effect to spawn when rocket dies
 * @param effectRange Range of the effect to spawn when the rocket dies
 * @param velocity Speed and direction this rocket moves to
 * @param range Max distance to travel before despawn
 * @param damage Damage this rocket deals when hitting a living entity
 * @param pierceEntities Whether this rocket despawns on first entity hit or not
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite Sprite image name (should look like "foo.png")
 * @param team The team this entity belongs to
 */
Rocket::Rocket(const Effect effect, const qreal effectRange, const Vector2 velocity, const qreal range, const qreal damage, const bool pierceEntities, const Vector2 position, const Vector2 dimensions, const QString& sprite, Teams::Team team)
    : Missile(velocity, range, damage, pierceEntities, position, dimensions, sprite, team), effectRange(effectRange)
{
    this->effect = new Effect(effect);
}

/**
 * Constructor
 * 
 * @param effect Effect to spawn when rocket dies
 * @param effectRange Range of the effect to spawn when the rocket dies
 * @param velocity Speed and direction this rocket moves to
 * @param range Max distance to travel before despawn
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite Sprite image name (should look like "foo.png")
 * @param team The team this entity belongs to
 */
Rocket::Rocket(const Effect effect, const qreal effectRange, const Vector2 velocity, const qreal range, const Vector2 position, const Vector2 dimensions, const QString& sprite, Teams::Team team)
    : Missile(velocity, range, 0, false, position, dimensions, sprite, team), effectRange(effectRange)
{
    this->effect = new Effect(effect);
}

/**
 * Destructor
 */
Rocket::~Rocket() {
    delete effect;
}

/**
 * Copy rocket on a new pointer
 * 
 * @return This rocket, copied in a new pointer
 */
Missile* Rocket::copy() const {
    return new Rocket(*this);
}

// --- Methods ---

/**
 * Make this rocket explode, creating an effect zone corresponding to the rocket effect
 */
void Rocket::explode() {
    setDeleted(true);
}

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 * @param deltaTime Time elapsed since last frame, in milliseconds
 */
void Rocket::onCollide(Entity* other, qint64 deltaTime) {
    if (LivingEntity* entity = dynamic_cast<LivingEntity*>(other)) {
        if (entity->getTeam() != getTeam()) {
            entity->takeDamage(damage*deltaTime*60/1000);       // values in json are in dmg per frame (60 fps)

            // If does not pierce entities, make it explode
            if (!pierceEntities) {
                explode();
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
bool Rocket::onUpdate(qint64 deltaTime) {
    Vector2 travel = velocity*deltaTime;
    setPos(getPos() + travel);

    // Handle max missile travel distance
    lifetime -= travel.magnitude();
    if (lifetime < 0) {
        explode();
    }

    return getDeleted();
}

/**
 * Get next Entity this entity wants to spawn
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* Rocket::getSpawned() {
    // If rocket is about to be deleted, spawn effect zone
    if (getDeleted() && effect != nullptr) {
        EffectZone* effectZone;

        // Spawn an effect
        effectZone = new EffectZone(*effect, getCenterPos(), effectRange);
        delete effect;
        effect = nullptr;

        return effectZone;
    }
    else {
        return nullptr;
    }
}