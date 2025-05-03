#ifndef LIVING_ENTITY_HPP
#define LIVING_ENTITY_HPP

#include <QtGlobal>
#include "entity.hpp"
#include "effect.hpp"

// Abstract class
class LivingEntity : public Entity {
private:
    Effect burning;
    Effect poisoned;
    Effect frozen;

    qreal speed;
    bool isLookingLeft = false;

    void initEffects();

protected:
    qreal life;
    qreal maxLife;
    bool isDead;
    const qint64 burningTime = 3000;
    const qint64 poisonedTime = 10000;
    
    LivingEntity(const LivingEntity& other);
    qreal getSpeedMultiplier() const;
    bool getLookingLeft() const;
    void setLookingLeft(const bool lookingLeft);

public:
    // Constructors/destructors
    LivingEntity();
    LivingEntity(qreal life, const qreal speed, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None, Teams::Team team = Teams::None);
    ~LivingEntity();

    // Getters
    qreal getLife() const;
    qreal getMaxLife() const;
    qreal getSpeed() const;
    bool getIsDead() const;

    // Setters
    void setLife(const qreal life);
    void setMaxLife(const qreal maxLife);
    void takeDamage(const qreal damage);
    void setSpeed(const qreal speed);

    // Virtual methods
    virtual void onDeath() = 0;
    virtual bool onUpdate(qint64 deltaTime);

    // Methods
    void giveEffect(const Effect& effect);
};

#endif   // LIVING_ENTITY_HPP