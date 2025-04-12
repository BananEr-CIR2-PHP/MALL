#include "../include/livingEntity.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
LivingEntity::LivingEntity() {
    life = 1;
    isDead = false;
    initEffects();
}

/**
 * Copy constructor
 * 
 * @param other Another LivingEntity
 */
LivingEntity::LivingEntity(const LivingEntity& other) : Entity(other), burning(other.burning), poisoned(other.poisoned), frozen(other.frozen) {
    life = 1;
    isDead = false;
}

/**
 * Constructor
 * 
 * @param life Starting life of entity
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
LivingEntity::LivingEntity(qreal life, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite) : Entity(position, dimensions, sprite) {
    this->life = life > 0 ? life : 1;   // Do not start with 0 HP
    isDead = false;
    initEffects();
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

/**
 * Get speed multiplier of this living entity
 * 
 * @return Speed multiplier of this entity
 */
qreal LivingEntity::getSpeedMultiplier() const {
    if (frozen.hasDied()) {
        return 1;
    }
    else {
        // The closer frozen strength is to 0, the stronger the freeze
        return frozen.getStrength();
    }
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
        isDead = true;
        this->onDeath();
    }
    else {
        life -= damage;
    }
}

// ---METHODS --

/**
 * Initialize living entity effects
 */
void LivingEntity::initEffects() {
    burning = Effect(Effects::EffectType::Burning, 0, 0);
    poisoned = Effect(Effects::EffectType::Poisoned, 0, 0);
    frozen = Effect(Effects::EffectType::Frozen, 0, 0);
}

/**
 * Give an effect to this living entity
 * 
 * @param effect The effect to give. If effect is frozen, this method will take effect duration. For all other effect types, duration is handled by this method
 */
void LivingEntity::giveEffect(const Effect& effect) {
    switch (effect.getType()) {
        // Burning/poisoned/frozen : make effect last longer if new duration is bigger
        // And set new strength if new strength is bigger
        case Effects::EffectType::Burning:
            burning.setDuration(burningTime);

            if (effect.getStrength() > burning.getStrength()) {
                burning.setStrength(effect.getStrength());
            }
            break;

        case Effects::EffectType::Poisoned:
            poisoned.setDuration(poisonedTime);

            if (effect.getStrength() > poisoned.getStrength()) {
                poisoned.setStrength(effect.getStrength());
            }
            break;

        case Effects::EffectType::Frozen:
            if (effect.getDurationLeft() > frozen.getDurationLeft()) {
                frozen.setDuration(effect.getDurationLeft());
            }
            if (effect.getStrength() < frozen.getStrength()) {
                frozen.setStrength(effect.getStrength());
            }
            break;

        case Effects::EffectType::Boom:
            takeDamage(effect.getStrength());
            break;
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity or not
 */
bool LivingEntity::onUpdate(qint64 deltaTime) {
    // Update burning effect
    if (burning.hasDied()) {
        burning.setStrength(0);
    }
    else {
        takeDamage(deltaTime*burning.getStrength());
        burning.decreaseDuration(deltaTime);
    }

    // Update poisoned effect
    if (poisoned.hasDied()) {
        poisoned.setStrength(0);
    }
    else {
        takeDamage(deltaTime*poisoned.getStrength());
        poisoned.decreaseDuration(deltaTime);
    }

    // Update frozen effect
    if (frozen.hasDied()) {
        frozen.setStrength(1);      // Less strong freeze possible
    }
    else {
        frozen.decreaseDuration(deltaTime);
    }

    return false;
}