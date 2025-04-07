#include "../include/livingEntity.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
LivingEntity::LivingEntity() {
    life = 1;
}

/**
 * Copy constructor
 * 
 * @param other Another LivingEntity
 */
LivingEntity::LivingEntity(const LivingEntity& other) : Entity(other) {
    life = 1;
}

/**
 * Constructor
 * 
 * @param life Starting life of entity
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
LivingEntity::LivingEntity(qreal life, const Vector2 position, const Vector2 dimensions, Sprite* sprite) : Entity(position, dimensions, sprite) {
    this->life = life > 0 ? life : 1;   // Do not start with 0 HP
}

/**
 * Destructor
 */
LivingEntity::~LivingEntity() { }

// -- GETTERS ---

/**
 * Get life of the living entity
 * 
 * @return Life of the living entity
 */
qreal LivingEntity::getLife() const {
    return life;
}

// --- SETTERS ---

/**
 * Set life of the living entity. This method does not take armor into account.
 * 
 * @param life New life of the entity
 */
void LivingEntity::setLife(const qreal life) {
    if (life > 0) {
        this->life = life;
    }
    else {
        this->life = 0;
        this->onDeath();
    }
}

/**
 * Remove life based on given damage. This method takes armor into account.
 * 
 * @param damage Damage the living entity should take
 */
void LivingEntity::takeDamage(const qreal damage) {
    if (damage > life) {
        life = 0;
        this->onDeath();
    }
    else {
        life -= damage;
    }
}