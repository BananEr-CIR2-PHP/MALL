#include "../include/rocket.hpp"

// --- CONSTRUCTORS/DESTRUCTOR ---

/**
 * Default constructor
 */
Rocket::Rocket() { }

/**
 * Copy constructor
 * 
 * @param other Another Rocket
 */
Rocket::Rocket(const Rocket& other) : Missile(other) { }

/**
 * Constructor
 * 
 * @param velocity Speed and direction this rocket moves to
 * @param range Max distance to travel before despawn
 * @param damage Damage this rocket deals when hitting a living entity
 * @param pierceEntities Whether this rocket despawns on first entity hit or not
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Rocket::Rocket(const Vector2 velocity, const qreal range, const qreal damage, const bool pierceEntities, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite)
    : Missile(velocity, range, damage, pierceEntities, position, dimensions, sprite)
{

}

/**
 * Constructor
 * 
 * @param velocity Speed and direction this rocket moves to
 * @param range Max distance to travel before despawn
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Rocket::Rocket(const Vector2 velocity, const qreal range, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite)
    : Missile(velocity, range, 0, false, position, dimensions, sprite)
{

}

/**
 * Destructor
 */
Rocket::~Rocket() { }

// --- Methods ---

/**
 * Make this rocket explode, creating an effect zone corresponding to the rocket effect
 */
void Rocket::explode() {
    // TODO: Generate effect zone
    setDeleted(true);
}

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void Rocket::onCollide(Entity* other) {
    // If does not pierce entities, make it explode
    if (!pierceEntities) {
        explode();
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 */
void Rocket::onUpdate(qint64 deltaTime) {
    Vector2 travel = velocity*deltaTime;
    setPos(getPos() + travel);

    // Handle max missile travel distance
    lifetime -= travel.magnitude();
    if (lifetime < 0) {
        explode();
    }
}