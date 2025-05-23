#include <QtGlobal>
#include "../../include/entity/effectZone.hpp"
#include "../../include/entity/livingEntity.hpp"
#include "../../include/entity/missile.hpp"

#define MIN_FORCE_STRENGTH 0.1
#define MAX_FORCE_STRENGTH 10.0
#define FORCE_FACTOR 5000

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
 * @param position Starting central position of effect zone
 * @param range Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
EffectZone::EffectZone(Effect effect, const Vector2 position, const qreal range, const QString& sprite) :
    Entity(position-Vector2(range, range), Vector2(2*range, 2*range), sprite), range(range), effect(effect)
{

}

/**
 * Constructor
 * 
 * @param effect Effect of the zone
 * @param position Starting central position of effect zone
 * @param range Collision box dimensions. Box is centered on position.
 */
EffectZone::EffectZone(Effect effect, const Vector2 position, const qreal range) :
    Entity(position-Vector2(range, range), Vector2(2*range, 2*range), getEffectSprite(effect.getType())), range(range), effect(effect)
{

}

/**
 * Destructor
 */
EffectZone::~EffectZone() { }

// --- INHERITED METHODS ---

/**
 * Redefine shape of effect zone (circle)
 * 
 * @return shape of the effect zone
 */
QPainterPath EffectZone::shape() const {
    QPainterPath path;
    path.addEllipse(0, 0, 2*range, 2*range);
    return path;
}

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 * @param deltaTime Time elapsed since last frame, in milliseconds
 */
void EffectZone::onCollide(Entity* other, qint64 deltaTime) {
    switch (effect.getType()) {
        case Effects::EffectType::None:
            break;

        case Effects::EffectType::Repel:
            // Do not repel missiles or other effect zones
            if (! (dynamic_cast<Missile*>(other) || dynamic_cast<EffectZone*>(other))) {
                repelEntity(other);
            }
            break;

        case Effects::EffectType::Boom:
            // Do not repel missiles or other effect zones
            if (! (dynamic_cast<Missile*>(other) || dynamic_cast<EffectZone*>(other))) {
                repelEntity(other);
            }
            // Do not break here, we also want to give the boom effect

        case Effects::EffectType::Burning:
        case Effects::EffectType::Poisoned:
        case Effects::EffectType::Frozen:
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
    // Decrease effect duration left and vanish if no time left
    effect.decreaseDuration(deltaTime);

    if (effect.hasDied()) {
        setDeleted(true);
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

// --- METHODS ---

/**
 * Get sprite of effect zone from the effect type
 */
QString EffectZone::getEffectSprite(Effects::EffectType effectType) {
    QString img;
    switch (effectType) {
        case Effects::EffectType::Boom:
            img = "boom.png";
            break;

        case Effects::EffectType::Burning:
            img = "fire_zone.png";
            break;

        case Effects::EffectType::Frozen:
            img = "ice_zone.png";
            break;

        case Effects::EffectType::Poisoned:
            img = "poison_zone.png";
            break;

        default:
            img = "";
            break;
    }
    return img;
}

/**
 * Repel entity depending on distance to the center of effect zone
 * 
 * @param entity The entity to repel
 */
void EffectZone::repelEntity(Entity* entity) {
    Vector2 vectorDistance = entity->getCenterPos() - getCenterPos();
    qreal squaredDistance = vectorDistance.sqrMagnitude();

    qreal force = FORCE_FACTOR * effect.getStrength() / squaredDistance;       // Formula: strength / distance²
    force = qMin(qMax(force, MIN_FORCE_STRENGTH), MAX_FORCE_STRENGTH);      // Clamp force

    entity->setPos(entity->getPos() + vectorDistance.normalized()*force);
}