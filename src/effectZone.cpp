#include "../include/effectZone.hpp"
#include "../include/livingEntity.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
EffectZone::EffectZone() {
    range = 0;
    effect = Effect();
}

/**
 * Copy constructor
 * 
 * @param other The effect zone to copy
 */
EffectZone::EffectZone(const EffectZone& other) : Entity(other), range(other.range), effect(other.effect) { }

/**
 * Constructor
 * 
 * @param effect Effect of the zone
 * @param position Starting position of entity
 * @param range Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
EffectZone::EffectZone(Effect effect, const Vector2 position, const qreal range, Sprites::SpriteImage sprite) :
    Entity(position, Vector2(2*range, 2*range), sprite), range(range), effect(effect)
{

}

/**
 * Destructor
 */
EffectZone::~EffectZone() { }

// --- INHERITED METHODS ---

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void EffectZone::onCollide(Entity* other) {
    switch (effect.getType()) {
        case Effects::EffectType::None:
            break;

        case Effects::EffectType::Repel:
            repelEntity(other);
            break;

        case Effects::EffectType::Burning:
        case Effects::EffectType::Poisoned:
        case Effects::EffectType::Frozen:
        case Effects::EffectType::Boom:
            if (LivingEntity* ent = dynamic_cast<LivingEntity*>(other)) {
                // Effect duration is handled by the living entity. If frozen, keep effect zone duration left
                ent->giveEffect(effect);
            }
            break;
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity or not
 */
bool EffectZone::onUpdate(qint64 deltaTime) {
    // Boom effect only lasts one frame
    if (effect.getType() == Effects::EffectType::Boom) {
        if (effect.hasDied()) {
            setDeleted(true);
        }
        else {
            effect.setDuration(0);
        }
    }
    else {
        // Other effects: Decrease effect duration left and vanish if no time left
        effect.decreaseDuration(deltaTime);

        if (effect.hasDied()) {
            setDeleted(true);
        }
    }

    return false;
}

/**
 * Get next Entity this entity wants to spawn
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* EffectZone::getSpawned() {
    return nullptr;
}

/**
 * Repel entity depending on distance to the center of effect zone
 * 
 * @param entity The entity to repel
 */
void EffectZone::repelEntity(Entity* entity) {
    Vector2 vectorDistance = entity->getPos() - getPos();
    qreal realDistance = vectorDistance.magnitude();
    qreal force = effect.getStrength() / (realDistance*realDistance);       // Formula: strength / distance²

    entity->setPos(entity->getPos() + vectorDistance.normalized()*force);
}