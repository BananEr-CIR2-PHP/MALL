#include "../../include/entity/effect.hpp"
#include <iostream>

// --- CONSTRUCTOR/DESTRUCTOR

/**
 * Default constructor
 */
Effect::Effect() {
    type = Effects::EffectType::None;
    strength = 0;
    durationLeft = 0;
}

/**
 * Copy constructor
 * 
 * @param other Another effect
 */
Effect::Effect(const Effect& other) : type(other.type), strength(other.strength), durationLeft(other.durationLeft) { }

/**
 * Constructor
 * 
 * @param type Type of effect
 * @param strength Strength of effect
 * @param durationLeft Duration left for the effect (ms)
 */
Effect::Effect(Effects::EffectType type, qreal strength, qint64 durationLeft) : type(type), strength(strength), durationLeft(durationLeft) {

}

/**
 * Constructor
 * 
 * @param type Type of effect
 * @param strength Strength of effect
 * @param durationLeft Duration left for the effect (ms)
 */
Effect::Effect(const QString& type, qreal strength, qint64 durationLeft) {
    this->strength = strength;
    this->durationLeft = durationLeft;

    if (type == "Burning") {
        this->type = Effects::EffectType::Burning;
    }
    else if (type == "Poisoned") {
        this->type = Effects::EffectType::Poisoned;
    }
    else if (type == "Frozen") {
        this->type = Effects::EffectType::Frozen;
    }
    else if (type == "Repel") {
        this->type = Effects::EffectType::Repel;
    }
    else if (type == "Boom") {
        this->type = Effects::EffectType::Boom;
    }
    else {
        this->type = Effects::EffectType::None;
    }
}

/**
 * Destructor
 */
Effect::~Effect() { }

// --- GETTERS ---

/**
 * Get the type of effect
 * 
 * @return The type of effect
 */
Effects::EffectType Effect::getType() const {
    return type;
}

/**
 * Get strength of the effect
 * 
 * @return Strength of the effect
 */
qreal Effect::getStrength() const {
    return strength;
}

/**
 * Get alive time left for the effect
 * 
 * @return Effect duration left
 */
qint64 Effect::getDurationLeft() const {
    return durationLeft;
}

/**
 * Get whether this effect should vanish or not
 * 
 * @return Whether if effect duration has exceed limit or not
 */
bool Effect::hasDied() const {
    return (durationLeft <= 0);
}

// --- SETTERS ---

/**
 * Set new duration for the effect
 * 
 * @param duration New duration left for the effect
 */
void Effect::setDuration(const qint64 duration) {
    durationLeft = duration;
}

/**
 * Set new strength for the effect
 * 
 * @param strength New strength for the effect
 */
void Effect::setStrength(const qreal strength) {
    if (type != Effects::EffectType::Frozen || (strength >= 0 && strength <= 1)) {
        this->strength = strength;
    }
}

/**
 * Decrease duration left of this effect by the given decrement
 * 
 * @param decrement Number to decrement the duration to (must be > 0)
 */
void Effect::decreaseDuration(const qint64 decrement) {
    if (decrement > 0) {
        durationLeft -= decrement;
    }
}