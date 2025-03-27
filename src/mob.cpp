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
Mob::Mob(double life, const Vector2 position, const Vector2 dimensions, const Sprite* sprite) : LivingEntity(life, position, dimensions, sprite) { }

/**
 * Destructor
 */
Mob::~Mob() { }