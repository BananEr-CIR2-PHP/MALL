#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <QtGlobal>
#include <QString>
#include "effectType.hpp"

class Effect {
private:
    Effects::EffectType type;
    qreal strength;
    qint64 durationLeft;

public:
    // Constructor/Destructor
    Effect();
    Effect(const Effect& other);
    Effect(Effects::EffectType type, qreal strength, qint64 durationLeft);
    Effect(const QString& type, qreal strength, qint64 durationLeft);
    ~Effect();

    // Getters
    Effects::EffectType getType() const;
    qreal getStrength() const;
    qint64 getDurationLeft() const;
    bool hasDied() const;

    // Setters
    void setDuration(const qint64 duration);
    void setStrength(const qreal strength);
    void decreaseDuration(const qint64 decrement);
};

#endif   // EFFECT_HPP